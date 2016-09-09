#include "peerconnectionclient.h"

#include "webrtc/api/test/fakevideotrackrenderer.h"

PeerConnectionClient* PeerConnectionClient::instance = nullptr;

PeerConnectionClient::PeerConnectionClient()
{
    peer_connection_observer_ = new PCObserver();
    sdp_observer_ = new SdpObserver();
    //signaling_thread_ = std::move(rtc::Thread::Create());
    //worker_thread_ = std::move(rtc::Thread::Create());
    //window_ = new VideoWindow();
}

PeerConnectionClient::~PeerConnectionClient()
{

}

PeerConnectionClient* PeerConnectionClient::GetInstance()
{
    if (instance == NULL) instance = new PeerConnectionClient();
    return instance;
}


void PeerConnectionClient::CreatePeerConnectionFactory(PeerConnectionParamters* parameters,
                                                  PeerConnectionEvents* event)
{
    std::cout << "Create Peerconnection factory" << std::endl;
    //queued_remote_candidates_ = std::vector<IceCandidate*>();

    events_ = event;
    peer_connection_parameters_ = parameters;
    peer_connection_factory_ = webrtc::CreatePeerConnectionFactory();
   // worker_thread_->Start();
   // signaling_thread_->Start();
  //  peer_connection_factory_ = webrtc::CreatePeerConnectionFactory(worker_thread_.get(), signaling_thread_.get(),
  //                                                                 nullptr, nullptr, nullptr);

    // Reset variables to initial states.
    local_sdp_ = nullptr;
    is_error_ = false;
    video_call_enabled_ = true;
    // Check preferred video codec
    // H264 as default

    //
}

void PeerConnectionClient::CreatePeerConnection(AppRTCClient::SignalingParameters* parameters)
{
    CreateMediaConstraints();
    if (peer_connection_factory_ == NULL || is_error_) {
        std::cout << "Peerconnection factory is not created" << std::endl;
        return;
    }
    std::cout << "Create Peer connection" << std::endl;
    queued_remote_candidates_ = std::vector<IceCandidate*>();

    webrtc::PeerConnectionInterface::RTCConfiguration rtc_config;
    webrtc::PeerConnectionInterface::IceServer server;

    rtc_config.tcp_candidate_policy = webrtc::PeerConnectionInterface::kTcpCandidatePolicyDisabled;
    rtc_config.bundle_policy = webrtc::PeerConnectionInterface::kBundlePolicyMaxBundle;
    rtc_config.continual_gathering_policy = webrtc::PeerConnectionInterface::GATHER_CONTINUALLY;

    // Use ECDSA encryption : default ECDSA

    // Create Peerconnection
    peer_connection_ = peer_connection_factory_->CreatePeerConnection(rtc_config, pc_constraints_,
                                                                      NULL, NULL, peer_connection_observer_);

    // AddStreams
    media_streams_ = peer_connection_factory_->CreateLocalMediaStream("ARDAMS");

    rtc::scoped_refptr<webrtc::AudioTrackInterface> audio_track(
                peer_connection_factory_->CreateAudioTrack(
                    kAudioLabel, peer_connection_factory_->CreateAudioSource(NULL)));

    // Camera 1

    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track(
                peer_connection_factory_->CreateVideoTrack(
                    kVideoLabel,
                    peer_connection_factory_->CreateVideoSource(OpenVideoCaptureDevice(0),
                                                                             NULL)));


    /*
    // Camera 2
    rtc::scoped_refptr<webrtc::VideoTrackInterface> video_track2(
                peer_connection_factory_->CreateVideoTrack(
                    kVideoLabel2,
                    peer_connection_factory_->CreateVideoSource(OpenVideoCaptureDevice(2),
                                                                             NULL)));
*/
    video_window_.reset(new VideoWindow(1, 1, video_track));
    video_window_->show();
    media_streams_->AddTrack(audio_track);
    media_streams_->AddTrack(video_track);
  //  media_streams_->AddTrack(video_track2);
    peer_connection_->AddStream(media_streams_);
}

void PeerConnectionClient::CreateOffer()
{
    is_initiator_ = true;
    peer_connection_->CreateOffer(sdp_observer_, sdp_media_constraints_);
}

void PeerConnectionClient::CreateAnswer()
{
    is_initiator_ = false;
    peer_connection_->CreateAnswer(sdp_observer_, sdp_media_constraints_);
}

void PeerConnectionClient::AddRemoteIceCandidate(IceCandidate* candidate)
{
    if (peer_connection_ != NULL && !is_error_) {
        if (!queued_remote_candidates_.empty()) {
            queued_remote_candidates_.push_back(candidate);
        } else {
            peer_connection_->AddIceCandidate(candidate);
        }
    }
}

void PeerConnectionClient::RemoveRemoteIceCandidates(std::vector<IceCandidate*> candidate)
{
    if (peer_connection_ == NULL || is_error_) {
        return;
    }
    // Drain the queued remote candidates if there is any so that
    // they are processed in the proper order.
    DrainCandidates();
    //peer_connection_->RemoveIceCandidates(candidate);
}

void PeerConnectionClient::SetRemoteDescription(SessionDescription* sdp)
{
    if (peer_connection_ == NULL || is_error_) {
        return;
    }
    //std::string sdp_description = sdp->description()->;
    //QString sdp_descirption = sdp->description_;

    //
    peer_connection_->SetRemoteDescription(DummySetSessionDescriptionObserver::Create(), sdp);

}

void PeerConnectionClient::DrainCandidates()
{
    if (!queued_remote_candidates_.empty()) {
        std::cout << "Add " << queued_remote_candidates_.size() << " remote candidates" << std::endl;
        for (int i = 0; i < queued_remote_candidates_.size(); i++) {
            peer_connection_->AddIceCandidate(queued_remote_candidates_.at(i));
        }
        queued_remote_candidates_.clear();
        //queued_remote_candidates_ = NULL;
    }
}

void PeerConnectionClient::CreateMediaConstraints()
{
    std::cout << "CreateMediaConstraints" << std::endl;

    pc_constraints_ = new webrtc::FakeConstraints;
    sdp_media_constraints_ = new webrtc::FakeConstraints;

    if (peer_connection_parameters_->loopback_) {
        pc_constraints_->AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, "false");
    } else {
        pc_constraints_->AddOptional(webrtc::MediaConstraintsInterface::kEnableDtlsSrtp, "true");
    }
    std::cout << "pc_constraints" << std::endl;
    video_width_ = peer_connection_parameters_->video_width_;
    video_height_ = peer_connection_parameters_->video_height_;
    video_fps_ = peer_connection_parameters_->video_fps_;

    // if video resolution is not specified, default to HD
    if (video_width_ == 0 || video_height_ == 0) {
        video_width_ = 1280;
        video_height_ = 720;
    }

    // If fps is not specified, default to 30
    if (video_fps_ == 0) {
        video_fps_ = 30;
    }

    // Create audio constraint
    // TODO

    // Create SDP constraint
    sdp_media_constraints_->AddMandatory(webrtc::MediaConstraintsInterface::kOfferToReceiveAudio, "true");
    sdp_media_constraints_->AddMandatory(webrtc::MediaConstraintsInterface::kOfferToReceiveVideo, "true");

}

cricket::VideoCapturer* PeerConnectionClient::OpenVideoCaptureDevice(int device_id)
{
    //std::vector<std::string> device_names;
    {
        std::unique_ptr<webrtc::VideoCaptureModule::DeviceInfo> info(
                    webrtc::VideoCaptureFactory::CreateDeviceInfo(0));
        if (!info) {
            return nullptr;
        }

        int num_devices = info->NumberOfDevices();
        for (int i = 0; i < num_devices; ++i) {
            const uint32_t kSize = 256;
            char name[kSize] = {0};
            char id[kSize] = {0};
            if (info->GetDeviceName(i, name, kSize, id, kSize) != -1) {
                device_names_.push_back(name);
            }
        }
    }

    cricket::WebRtcVideoDeviceCapturerFactory factory;
    cricket::VideoCapturer* capturer = nullptr;

    capturer = factory.Create(cricket::Device(device_names_.at(device_id), device_id));

    /*for (const auto& name : device_names_) {
        capturer = factory.Create(cricket::Device(name, 0));
        if (capturer) {
            break;
        }
    }*/
    return capturer;
}

void PeerConnectionClient::SwitchCamera(int id)
{

}

/*
QString PeerConnectionClient::PreferCodec(webrtc::SessionDescriptionInterface* sdp, QString codec, bool is_audio)
{
    std::string sdp_str;
    sdp->ToString(sdp_str);
    QString sdp_qstr = QString::fromStdString(sdp_str);
    QStringList lines = sdp_qstr.split(QString("\r\n"));
    int m_line_index = -1;
    QString codec_rtp_map;
    QRegExp regex("^a=rtpmap:(\\d+) " + codec + "(/\\d+)+[\r]?$");
}
*/

// PCObserver: webrtc::PeerConnectionObserver implementation
void PeerConnectionClient::PCObserver::OnAddStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
    std::cout << "OnAddStream()" << std::endl;
    webrtc::VideoTrackVector tracks = stream->GetVideoTracks();

    if (!tracks.empty()) {
        instance->remote_video_.reset(new VideoWindow(1, 1, tracks[0]));
        instance->remote_video_->show();
    }

}

void PeerConnectionClient::PCObserver::OnRemoveStream(rtc::scoped_refptr<webrtc::MediaStreamInterface> stream)
{
    std::cout << "OnRemoveStream()" << std::endl;
}

void PeerConnectionClient::PCObserver::OnIceCandidate(const webrtc::IceCandidateInterface *candidate)
{
    std::cout << "OnIceCandidate()" << std::endl;
    instance->events_->OnIceCandidate(candidate);
}

void PeerConnectionClient::PCObserver::OnIceCandidatesRemoved(const std::vector<cricket::Candidate>& candidates)
{
    std::cout << "OnIceCandidatesRemoved()" << std::endl;

    //events_->OnIceCandidatesRemoved();
}

void PeerConnectionClient::PCObserver::OnIceConnectionChange(webrtc::PeerConnectionInterface::IceConnectionState new_state)
{
    std::cout << "OnIceConnectionChange()" << std::endl;
    if (new_state == webrtc::PeerConnectionInterface::kIceConnected) {
        instance->events_->OnIceConnected();
    } else if (new_state == webrtc::PeerConnectionInterface::kIceConnectionDisconnected) {
        instance->events_->OnIceDisConnected();
    } else if (new_state == webrtc::PeerConnectionInterface::kIceConnectionFailed) {
        std::cout << "Ice connection failed" << std::endl;
    }
}

void PeerConnectionClient::PCObserver::OnIceGatheringChange(webrtc::PeerConnectionInterface::IceGatheringState new_state)
{
    std::cout << "OnIceGatheringChange()" << std::endl;
    std::cout << new_state << std::endl;
}
void PeerConnectionClient::PCObserver::OnIceConnectionReceivingChange(bool receiving)
{
    std::cout << "OnIceConnectionReceivingChange()" << std::endl;
}

void PeerConnectionClient::PCObserver::OnSignalingChange(webrtc::PeerConnectionInterface::SignalingState new_state)
{
    std::cout << "OnSignalingChange()" << std::endl;
}

void PeerConnectionClient::PCObserver::OnDataChannel(rtc::scoped_refptr<webrtc::DataChannelInterface> data_channel)
{
    std::cout << "OnDataChannel()" << std::endl;
}

void PeerConnectionClient::PCObserver::OnRenegotiationNeeded()
{
    std::cout << "OnRenegotiationNeeded()" << std::endl;
}

// SdpObserver implementation
void PeerConnectionClient::SdpObserver::OnSuccess(webrtc::SessionDescriptionInterface *desc)
{
    std::cout << "OnSuccess()" << std::endl;
    if (instance->local_sdp_ != nullptr) {
        std::cout << "Multiple SDP create." << std::endl;
        return;
    }

    instance->peer_connection_->SetLocalDescription(DummySetSessionDescriptionObserver::Create(), desc);

    instance->events_->OnLocalDescription(desc);
}

void PeerConnectionClient::SdpObserver::OnSetSuccess()
{
    std::cout << "OnSetSuccess()" << std::endl;
}

void PeerConnectionClient::SdpObserver::OnFailure(const std::string &error)
{
    std::cout << "OnFailure()" << std::endl;
}

void PeerConnectionClient::SdpObserver::OnSetFailure(const std::string &error)
{
    std::cout << "OnSetFailure()" << std::endl;
}

