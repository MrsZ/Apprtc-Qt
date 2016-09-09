#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "peerconnectionclient.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    InitUI();
    //ui->stackedWidget->setCurrentIndex(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::OnConnect()
{

    int pos = 0;
    if (QValidator::Acceptable == ui->lineEdit->validator()->validate(ui->lineEdit->text(), pos)) {

        // Connect to TCP server/client
        // Create DirectRTCClient for TCP communication
        // add port number automatically
        QString ip = ui->lineEdit->text();
        room_connection_parameters_ = new AppRTCClient::RoomConnectionParameters(
                    ip, ip, false); // not loopback
        direct_rtc_client_ = new DirectRTCClient(&ip, this);
        direct_rtc_client_->ConnectToRoom(room_connection_parameters_);

        peer_connection_parameters_ = new PeerConnectionClient::PeerConnectionParamters(
                    false,
                    1280,
                    720,
                    30,
                    1000,
                    "H264",
                    "OPUS");
    } else {
        Msgbox(new QString("recieved invalid ip/port.\nplease check your input\nthen try again."));
    }
}

void MainWindow::Msgbox(QString* log)
{
    QMessageBox msg_box(this);
    msg_box.setIcon(QMessageBox::Critical);
    msg_box.setWindowIconText("ERROR");
    msg_box.setText(*log);
    msg_box.exec();
}

void MainWindow::InitUI()
{
    connect(ui->connectButton, SIGNAL(clicked(bool)), this, SLOT(OnConnect()));
    // Set Reguler expression for ip address
    QRegExp regex(
                "("
                // IPv4
                "((\\d+\\.){3}\\d+)|"
                // IPv6
                "\\[((([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})?::"
                "(([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})?)\\]|"
                "\\[(([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4})\\]|"
                // IPv6 without []
                "((([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})?::(([0-9a-fA-F]{1,4}:)*[0-9a-fA-F]{1,4})?)|"
                "(([0-9a-fA-F]{1,4}:){7}[0-9a-fA-F]{1,4})|"
                // Literals
                "localhost"
                ")"
                // Optional port number
                "(:(\\d+))?");
    QRegExpValidator *regexv = new QRegExpValidator(regex, ui->centralWidget);
    ui->lineEdit->setValidator(regexv);
}

void MainWindow::Disconnect()
{
    direct_rtc_client_->DisconnectFromRoom();
    direct_rtc_client_ = nullptr;
    //peer_connection_client_->
}

// -----Implementation of AppRTCClient.AppRTCSignalingEvents ---------------
// All callbacks are invoked from websocket signaling looper thread and
// are routed to UI thread.
void MainWindow::OnConnectedToRoomInternal(AppRTCClient::SignalingParameters* params)
{
    peer_connection_client_ = PeerConnectionClient::GetInstance();
    peer_connection_client_->CreatePeerConnectionFactory(peer_connection_parameters_, this);
    signaling_parameters_ = params;
    peer_connection_client_->CreatePeerConnection(signaling_parameters_);

    if (signaling_parameters_->initiator_) {
        std::cout << "Creating OFFER..." << std::endl;
        // Create offer. Offer SDP will be sent to answering client in
        // PeerConnectionEvents.onLocalDescription event.
        peer_connection_client_->CreateOffer();
    } else {
        if (params->offer_sdp_ != nullptr) {
            peer_connection_client_->SetRemoteDescription(params->offer_sdp_);
            std::cout << "Creating ANSWER..." << std::endl;
            peer_connection_client_->CreateAnswer();
        }
        if (!params->ice_candidates_.empty()) {
            // Add remote ICE candidates from room
            for (IceCandidate* iceCandidate : params->ice_candidates_) {
                peer_connection_client_->AddRemoteIceCandidate(iceCandidate);
            }
        }
    }
}

void MainWindow::OnConnectedToRoom(AppRTCClient::SignalingParameters *params)
{
    std::cout << "OnConnectedToRoom" << std::endl;
    OnConnectedToRoomInternal(params);
}

void MainWindow::OnRemoteDescription(SessionDescription* sdp)
{
    std::cout << "OnRemoteDescription" << std::endl;
    if (peer_connection_client_ == nullptr) {
        std::cout << "Received remote SDP for non-initilized peer connection." << std::endl;
        return;
    }

    peer_connection_client_->SetRemoteDescription(sdp);
    if (!signaling_parameters_->initiator_) {
        std::cout << "Creating ANSWER..." << std::endl;
        // Create answer. Answer SDP will be sent to offering client in
        // PeerConnectionEvents.onLocalDescription event.
        peer_connection_client_->CreateAnswer();
    }
}

void MainWindow::OnRemoteIceCandidate(IceCandidate* candidate)
{
    std::cout << "OnRemoteIceCandidate" << std::endl;
    if (peer_connection_client_ == nullptr) {
        std::cout << "Received ICE candidate for a non-initialized peer connection." << std::endl;
        return;
    }
    peer_connection_client_->AddRemoteIceCandidate(candidate);
}

void MainWindow::OnRemoteIceCandidatesRemoved(const std::vector<IceCandidate*>& candidates)
{
    std::cout << "OnRemoteIceCandidatesRemoved" << std::endl;
    if (peer_connection_client_ == nullptr) {
        std::cout << "Received ICE candidate removals for a non-initialized peer connection." << std::endl;
        return;
    }
    peer_connection_client_->RemoveRemoteIceCandidates(candidates);
}

void MainWindow::OnChannelClose()
{
    std::cout << "OnChannelClose()" << std::endl;
    Disconnect();
}

void MainWindow::OnChannelError(const QString &description)
{
    std::cout << description.toStdString() << std::endl;
}

// -----Implementation of PeerConnectionClient.PeerConnectionEvents.---------
 // Send local peer connection SDP and ICE candidates to remote party.
 // All callbacks are invoked from peer connection client looper thread and
 // are routed to UI thread.
void MainWindow::OnLocalDescription(SessionDescription* sdp)
{
    std::cout << "OnLocalDescription()" << std::endl;
    if (direct_rtc_client_ != nullptr) {
        if (signaling_parameters_->initiator_) {
            direct_rtc_client_->SendOfferSdp(sdp);
        } else {
            direct_rtc_client_->SendAnswerSdp(sdp);
        }
    }
}

void MainWindow::OnIceCandidate(const IceCandidate *candidate)
{
    std::cout << "OnIceCandidate()" << std::endl;
    direct_rtc_client_->SendLocalIceCandidate(candidate);
}

void MainWindow::OnIceCandidatesRemoved(std::vector<IceCandidate*> candidates)
{
    std::cout << "OnIceCandidatesRemoved()" << std::endl;
    direct_rtc_client_->SendLocalIceCandidateRemovals(candidates);
}

void MainWindow::OnIceConnected()
{
    std::cout << "OnIceConnected()" << std::endl;

}

void MainWindow::OnIceDisConnected()
{
    std::cout << "OnIceDisConnected()" << std::endl;
}

void MainWindow::OnPeerConnectionClosed()
{
    std::cout << "OnPeerConnectionClosed()" << std::endl;
}

void MainWindow::OnPeerConnectionError(QString description)
{
    std::cout << "OnPeerConnectionError()" << std::endl;
}

// Renderer
void MainWindow::OnStartRenderer(VideoWindow* render)
{
    std::cout << "OnStartRenderer" << std::endl;
    ui->stackedWidget->addWidget(render);
    ui->stackedWidget->setCurrentIndex(2);
}
