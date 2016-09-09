#include "directrtcclient.h"

DirectRTCClient::DirectRTCClient()
{

}

DirectRTCClient::DirectRTCClient(QString *ip, SignalingEvents *event)
{
    ip_ = *ip;
    events_ = event;
    room_state_ = ConnectionState::NEW;
}

DirectRTCClient::~DirectRTCClient()
{

}

void DirectRTCClient::ConnectToRoom(RoomConnectionParameters *connectionParameters)
{
    connection_parameters_ = connectionParameters;
    ConnectToRoomInternal();
}

void DirectRTCClient::SendOfferSdp(const SessionDescription* sdp)
{
    if (room_state_ != ConnectionState::CONNECTED) {
        std::cout << "Sending offer SDP in non connected state." << std::endl;
        return;
    }

    std::string description;
    sdp->ToString(&description);

    // for debug
    std::cout << sdp->type();
    std::cout << " " << description << std::endl;

    QJsonObject json;
    JsonPut(json, "sdp", QJsonValue(QString::fromStdString(description)));
    JsonPut(json, "type", QJsonValue("offer"));
    std::cout << "json" << json.value("sdp").toString().toStdString() << std::endl;
    std::cout << "json" << json.value("type").toString().toStdString() << std::endl;
    QJsonDocument jdo(json);
    std::cout << "QjsonString: " << jdo.toJson(QJsonDocument::Compact).toStdString() << std::endl;

    DSendMessage(QString::fromStdString(jdo.toJson(QJsonDocument::Compact).toStdString()));
}

void DirectRTCClient::SendAnswerSdp(const SessionDescription* sdp)
{
    std::string description;
    sdp->ToString(&description);

    QJsonObject json;
    JsonPut(json, "sdp", QString::fromStdString((description)));
    JsonPut(json, "type", "answer");
    QJsonDocument jdo(json);
    std::cout << jdo.toJson().toStdString() << std::endl;
    DSendMessage(QString::fromStdString(jdo.toJson(QJsonDocument::Compact).toStdString()));
}

void DirectRTCClient::SendLocalIceCandidate(const IceCandidate* candidate)
{
    QJsonObject json;
    JsonPut(json, "type", "candidate");
    JsonPut(json, "label", candidate->sdp_mline_index());
    JsonPut(json, "id", QString::fromStdString(candidate->sdp_mid()));
    std::string str;
    candidate->ToString(&str);
    JsonPut(json, "candidate", QString::fromStdString(str));

    if (room_state_ != ConnectionState::CONNECTED) {
        std::cout << "Sending ICE candidate in non connected state." << std::endl;
        return;
    }
    QJsonDocument jdo(json);
    std::cout << jdo.toJson().toStdString() << std::endl;
    DSendMessage(QString::fromStdString(jdo.toJson(QJsonDocument::Compact).toStdString()));
}

void DirectRTCClient::SendLocalIceCandidateRemovals(const std::vector<IceCandidate*> &candidates)
{
    QJsonObject json;
    JsonPut(json, "type", "remove-candidates");
    QJsonArray json_array;
    for (int i = 0; candidates.size(); i++) {
        json_array.push_back(ToJsonCandidate(candidates[i]));
    }
    JsonPut(json, "candidates", json_array);

    if (room_state_ != ConnectionState::CONNECTED) {
        std::cout << "Sending ICE candidate removals in non connected state." << std::endl;
        return;
    }
    QJsonDocument jdo(json);
    std::cout << jdo.toJson().toStdString() << std::endl;
    DSendMessage(QString::fromStdString(jdo.toJson(QJsonDocument::Compact).toStdString()));
}

void DirectRTCClient::DisconnectFromRoom()
{
    DisconnectFromRoomInternal();
}

void DirectRTCClient::ConnectToRoomInternal()
{
    room_state_ = ConnectionState::NEW;
    QString endpoint = connection_parameters_->room_id_;
    tcp_client_ = new TCPChannelClient(endpoint, DEFAULT_PORT);
    tcp_client_->RegisterTCPObserver(this);
}

void DirectRTCClient::DisconnectFromRoomInternal()
{
    room_state_ = ConnectionState::CLOSED;
    tcp_client_->Disconnect();
}

void DirectRTCClient::DSendMessage(QString message)
{
    tcp_client_->Send(message);
}

void DirectRTCClient::JsonPut(QJsonObject& json, QString key, QJsonValue value)
{
    json.insert(key, value);
}

QJsonValue DirectRTCClient::ToJsonCandidate(IceCandidate* candidate)
{
    QJsonObject json;
    JsonPut(json, "label", candidate->sdp_mline_index());
    JsonPut(json, "id", QString::fromStdString(candidate->sdp_mid()));
    std::string str;
    candidate->ToString(&str);
    JsonPut(json, "candidate", QString::fromStdString((str)));
    return json;
}


//
// TCP Callbacks
//
void DirectRTCClient::OnTCPConnected(bool is_server)
{
    std::cout << "OnTCPConnected()" << std::endl;
    if (is_server) {
        room_state_ = ConnectionState::CONNECTED;
        SignalingParameters* parameters = new SignalingParameters(
                    IceServers(),
                    is_server,
                    NULL,
                    NULL,
                    NULL,
                    NULL,
                    std::vector<IceCandidate*>());
        events_->OnConnectedToRoom(parameters);
    }
}

void DirectRTCClient::OnTCPClose()
{
    std::cout << "OnTCPClose()" << std::endl;
}

void DirectRTCClient::OnTCPMessage(QString message)
{
    // should enclose with try-catch
    QJsonObject json = ObjectFromString(message);
    QString type = json.value("type").toString();
    //std::cout << "DirectRTCClient: " << type.toStdString() << std::endl;
    if (type == "candidate") {
        IceCandidate* ice_candidate = ToCandidate(json);
        events_->OnRemoteIceCandidate(ice_candidate);
    } else if (type == "remove-candidates") {
        //QJsonArray candidate_array = QJsonArray(json.value("candidates"));
        QJsonArray candidate_array = json.value("candidates").toArray();
        int len = candidate_array.size();
        std::vector<IceCandidate*> ice_candidates(len);
        for(int i = 0; i < candidate_array.size(); ++i) {
            ice_candidates[i] = ToCandidate(candidate_array.at(i).toObject());
        }
        events_->OnRemoteIceCandidatesRemoved(ice_candidates);
    } else if (type == "answer") {
        webrtc::SdpParseError error;
        SessionDescription* sdp = webrtc::CreateSessionDescription(type.toStdString(),
                                                                  json.value("sdp").toString().toStdString(),
                                                                  &error);
        events_->OnRemoteDescription(sdp);
    } else if (type == "offer") {
        webrtc::SdpParseError error;
        SessionDescription* sdp = webrtc::CreateSessionDescription(type.toStdString(),
                                                                  json.value("sdp").toString().toStdString(),
                                                                  &error);
        IceServers ice_servers;
        AppRTCClient::SignalingParameters* parameters = new AppRTCClient::SignalingParameters(
                    ice_servers,
                    false,
                    NULL,
                    NULL,
                    NULL,
                    sdp,
                    std::vector<IceCandidate*>());   // send empty vector
        room_state_ = ConnectionState::CONNECTED;
        events_->OnConnectedToRoom(parameters);
    } else {
        std::cout << "Unexpected TCP message: " << message.toStdString() << std::endl;
    }
    std::cout << "message: " << message.toStdString() << std::endl;
}

void DirectRTCClient::OnTCPError(QString description)
{
    std::cout << "error: " << description.toStdString() << std::endl;
}

IceCandidate* DirectRTCClient::ToCandidate(QJsonObject json)
{
    return webrtc::CreateIceCandidate(json.value("id").toString().toStdString(),
                                      json.value("label").toInt(),
                                      json.value("candidate").toString().toStdString(),
                                      NULL);
}
QJsonObject DirectRTCClient::ObjectFromString(const QString& in)
{
    QJsonObject obj;

    //std::cout << "utf-8: " << in.toUtf8().toStdString() << std::endl;

    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(in.toUtf8(), &error);
    std::cout << "Qjson error: " << error.errorString().toStdString() << std::endl;

    // check validity of the document
    if(!doc.isNull())
    {
        if(doc.isObject())
        {
            obj = doc.object();
        }
        else
        {
            qDebug() << "Document is not an object" << endl;
        }
    }
    else
    {
        qDebug() << "Invalid JSON...\n" << in << endl;
    }

    return obj;
}
