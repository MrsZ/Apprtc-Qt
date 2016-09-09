#ifndef DIRECTRTCCLIENT_H
#define DIRECTRTCCLIENT_H

#include <QObject>
#include <QString>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "apprtcclient.h"
#include "tcpchannelclient.h"

class TCPChannelClient;

class DirectRTCClient : public AppRTCClient, public TCPChannelEvents
{
    Q_OBJECT
public:
    explicit DirectRTCClient();
    DirectRTCClient(QString *ip, SignalingEvents* event);
    virtual ~DirectRTCClient();

    IceCandidate* ToCandidate(QJsonObject json);

    /**
       * Asynchronously connect to an AppRTC room URL using supplied connection
       * parameters. Once connection is established onConnectedToRoom()
       * callback with room parameters is invoked.
       */
    void ConnectToRoom(RoomConnectionParameters* connectionParameters);

    /**
    * Send offer SDP to the other participant.
    */
    void SendOfferSdp(const SessionDescription* sdp);

    /**
    * Send answer SDP to the other participant.
    */
    void SendAnswerSdp(const SessionDescription *sdp);

    /**
    * Send Ice candidate to the other participant.
    */
    void SendLocalIceCandidate(const IceCandidate* candidate);

    /**
    * Send removed ICE candidates to the other participant.
    */
    void SendLocalIceCandidateRemovals(const std::vector<IceCandidate*>& candidates);

    /**
    * Disconnect from room.
    */
    void DisconnectFromRoom();


    //
    // TCP Callbacks
    //
    virtual void OnTCPConnected(bool is_server);
    virtual void OnTCPClose();
    virtual void OnTCPMessage(QString message);
    virtual void OnTCPError(QString description);

private:
    enum class ConnectionState {
        NEW, CONNECTED, CLOSED, CONNECTION_ERROR
    };

private:
    void ConnectToRoomInternal();
    void DisconnectFromRoomInternal();
    void DSendMessage(QString message);
    void JsonPut(QJsonObject &json, QString key, QJsonValue value);
    QJsonValue ToJsonCandidate(IceCandidate* candidate);
    QJsonObject ObjectFromString(const QString& in);

private:
    TCPChannelClient* tcp_client_;
    SignalingEvents* events_;
    QString ip_;
    const int DEFAULT_PORT = 8888;
    RoomConnectionParameters* connection_parameters_;
    ConnectionState room_state_;
};

#endif // DIRECTRTCCLIENT_H
