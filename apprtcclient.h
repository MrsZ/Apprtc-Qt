#ifndef APPRTCCLIENT_H
#define APPRTCCLIENT_H

#include <vector>

#include <QObject>
#include <QString>

#include "webrtc/api/jsep.h"
#include "webrtc/api/peerconnectioninterface.h"

typedef webrtc::PeerConnectionInterface::IceServers IceServers;
typedef webrtc::SessionDescriptionInterface SessionDescription;
typedef webrtc::IceCandidateInterface IceCandidate;

class AppRTCClient : public QObject
{
    Q_OBJECT
public:
    explicit AppRTCClient(QObject *parent = 0);

    class RoomConnectionParameters {
    public:
        QString room_url_;
        QString room_id_;
        bool loopback_;

        RoomConnectionParameters(QString room_url, QString room_id, bool loopback) :
            room_url_(room_url), room_id_(room_id), loopback_(loopback) {

        }
    };

    virtual void ConnectToRoom(RoomConnectionParameters* connection_parameters) = 0;
    virtual void SendOfferSdp(const SessionDescription* sdp) = 0;
    virtual void SendAnswerSdp(const SessionDescription* sdp) = 0;
    virtual void SendLocalIceCandidate(const IceCandidate* candidate) = 0;
    virtual void SendLocalIceCandidateRemovals(const std::vector<IceCandidate*>& candidates) = 0;
    virtual void DisconnectFromRoom() = 0;

    class SignalingParameters {
    public:
        IceServers servers_;
        bool initiator_;
        QString client_id_;
        QString wss_url_;
        QString wss_post_url_;
        SessionDescription* offer_sdp_;
        std::vector<IceCandidate*> ice_candidates_;

        SignalingParameters (IceServers servers,
                             bool initiator, QString client_id,
                             QString wss_url, QString wss_post_url,
                             SessionDescription* offer_sdp,
                             std::vector<IceCandidate*> const & ice_candidates) :
                            servers_(servers), initiator_(initiator), client_id_(client_id),
                            wss_url_(wss_url), wss_post_url_(wss_post_url), offer_sdp_(offer_sdp),
                            ice_candidates_(ice_candidates)
        {

        }
    };

    struct SignalingEvents {
        virtual void OnConnectedToRoom(AppRTCClient::SignalingParameters* params) = 0;
        virtual void OnRemoteDescription(SessionDescription* sdp) = 0;
        virtual void OnRemoteIceCandidate(IceCandidate* candidate) = 0;
        virtual void OnRemoteIceCandidatesRemoved(const std::vector<IceCandidate*>& candidates) = 0;
        virtual void OnChannelClose() = 0;
        virtual void OnChannelError(const QString & description) = 0;
    };

signals:

public slots:
};



#endif // APPRTCCLIENT_H
