#ifndef PEERCONNECTIONCLIENT_H
#define PEERCONNECTIONCLIENT_H

#include <vector>
#include <string>
#include <memory>
#include <iostream>

#include <QObject>
#include <QString>

#include "webrtc/api/jsep.h"
#include "webrtc/api/mediaconstraintsinterface.h"
#include "webrtc/api/mediastreaminterface.h"
#include "webrtc/api/peerconnectioninterface.h"

#include "webrtc/api/test/fakeconstraints.h"

#include "webrtc/media/engine/webrtcvideocapturerfactory.h"
#include "webrtc/modules/video_capture/video_capture_factory.h"

#include "webrtc/base/thread.h"

#include "apprtcclient.h"
#include "videowindow.h"

const char kAudioLabel[] = "audio_label";
const char kVideoLabel[] = "video_label";
const char kVideoLabel2[] = "video_label2";
const char kStreamLabel[] = "stream_label";
const uint16_t kDefaultServerPort = 8888;

struct PeerConnectionEvents {
    virtual void OnLocalDescription(SessionDescription* sdp) = 0;
    virtual void OnIceCandidate(const IceCandidate* candidate) = 0;
    virtual void OnIceCandidatesRemoved(std::vector<IceCandidate*> candidate) = 0;
    virtual void OnIceConnected() = 0;
    virtual void OnIceDisConnected() = 0;
    virtual void OnPeerConnectionClosed() = 0;
    virtual void OnPeerConnectionError(QString description) = 0;
    //
    virtual void OnStartRenderer(VideoWindow* render) = 0;
};

class DummySetSessionDescriptionObserver
    : public webrtc::SetSessionDescriptionObserver {
 public:
  static DummySetSessionDescriptionObserver* Create() {
    return
        new rtc::RefCountedObject<DummySetSessionDescriptionObserver>();
  }
  virtual void OnSuccess() {
    LOG(INFO) << __FUNCTION__;
  }
  virtual void OnFailure(const std::string& error) {
    LOG(INFO) << __FUNCTION__ << " " << error;
  }

 protected:
  DummySetSessionDescriptionObserver() {}
  ~DummySetSessionDescriptionObserver() {}
};

class PeerConnectionClient
{
public:
    static PeerConnectionClient* GetInstance();

    class PeerConnectionParamters {
    public:
        bool loopback_;
        int video_width_;
        int video_height_;
        int video_fps_;
        int video_start_bitrate_;
        QString video_codec_;
        QString audio_codec_;
        PeerConnectionParamters(bool loopback, int video_width, int video_height, int video_fps,
                                int video_start_bitrate, QString video_codec, QString audio_codec) :
                            loopback_(loopback), video_width_(video_width), video_height_(video_height),
                            video_fps_(video_fps), video_start_bitrate_(video_start_bitrate),
                            video_codec_(video_codec), audio_codec_(audio_codec) {}
    };

    void CreatePeerConnectionFactory(PeerConnectionParamters* parameters, PeerConnectionEvents* event);
    void CreatePeerConnection(AppRTCClient::SignalingParameters *parameters);
    void CreateMediaConstraints();
    void CreateOffer();
    void CreateAnswer();
    void AddRemoteIceCandidate(IceCandidate* candidate);
    void RemoveRemoteIceCandidates(std::vector<IceCandidate*> candidate);
    void SetRemoteDescription(SessionDescription* sdp);
    void DrainCandidates();

private:

    PeerConnectionClient();
    PeerConnectionClient(const PeerConnectionClient& other);
    ~PeerConnectionClient();

    static PeerConnectionClient* instance;

    cricket::VideoCapturer* OpenVideoCaptureDevice(int device_id);
    void SwitchCamera(int id);
    //QString PreferCodec(webrtc::SessionDescriptionInterface* sdp, QString codec, bool is_audio);

    // PeerConnection Observer
    class PCObserver : public webrtc::PeerConnectionObserver
    {
        void OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream);
        void OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream);
        void OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel);
        void OnRenegotiationNeeded();
        void OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state);
        void OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state);
        void OnIceCandidate(const webrtc::IceCandidateInterface* candidate);
        void OnIceConnectionReceivingChange(bool receiving);
        void OnIceCandidatesRemoved(const std::vector<cricket::Candidate>& candidates);
        void OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state);
    };

    // SdpObserver
    class SdpObserver : public webrtc::CreateSessionDescriptionObserver
    {
        // Create Observer
        virtual void OnSuccess(webrtc::SessionDescriptionInterface* desc);
        virtual void OnFailure(const std::string& error);
        virtual void OnSetSuccess();
        virtual void OnSetFailure(const std::string& error);

        int AddRef() const { return 0;}
        int Release() const { return 0;}
    };

    // Observers
    PCObserver* peer_connection_observer_;
    SdpObserver* sdp_observer_;
    rtc::scoped_refptr<webrtc::PeerConnectionInterface> peer_connection_;
    rtc::scoped_refptr<webrtc::PeerConnectionFactoryInterface> peer_connection_factory_;
    rtc::scoped_refptr<webrtc::MediaStreamInterface> media_streams_;
    PeerConnectionEvents* events_;
    AppRTCClient::SignalingParameters* signaling_parameters_;
    PeerConnectionParamters* peer_connection_parameters_;
    webrtc::FakeConstraints* pc_constraints_;
    webrtc::FakeConstraints* audio_constraints_;
    webrtc::FakeConstraints* sdp_media_constraints_;

    std::vector<IceCandidate*> queued_remote_candidates_;
    SessionDescription* local_sdp_;

    bool video_call_enabled_;
    bool enable_audio_;
    bool prefer_isac_;
    bool is_error_;
    bool is_initiator_;

    QString preffered_video_codec_;

    int video_width_;
    int video_height_;
    int video_fps_;

    const QString VIDEO_CODEC_VP8 = "VP8";
    const QString VIDEO_CODEC_VP9 = "VP9";
    const QString VIDEO_CODEC_H264 = "H264";
    const QString AUDIO_CODEC_OPUS = "opus";
    const QString AUDIO_CODEC_ISAC = "ISAC";
    const QString VIDEO_CODEC_PARAM_START_BITRATE = "x-google-start-bitrate";

    // renderers
    //std::unique_ptr<VideoRenderer> local_renderer_;
    //std::unique_ptr<VideoRenderer> remote_renderer_;
    //VideoWindow* window_;

    // Signaling and worker threads
    //std::unique_ptr<rtc::Thread> signaling_thread_;
    //std::unique_ptr<rtc::Thread> worker_thread_;
    //VideoWindow* video_window_;
    std::vector<std::string> device_names_;
    std::unique_ptr<VideoWindow> video_window_;
    std::unique_ptr<VideoWindow> remote_video_;
};


#endif // PEERCONNECTIONCLIENT_H
