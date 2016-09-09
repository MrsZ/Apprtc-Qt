#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include "directrtcclient.h"
#include "peerconnectionclient.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow,
                   public AppRTCClient::SignalingEvents,
                   public PeerConnectionEvents
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void InitUI();
    void Msgbox(QString* log);

    //
    // SignalingEvetns
    //
    void OnConnectedToRoom(AppRTCClient::SignalingParameters* params);
    void OnRemoteDescription(SessionDescription *sdp);
    void OnRemoteIceCandidate(IceCandidate* candidate);
    void OnRemoteIceCandidatesRemoved(const std::vector<IceCandidate*>& candidates);
    void OnChannelClose();
    void OnChannelError(const QString & description);

    //
    // PeerConnectionEvents
    //
    void OnLocalDescription(SessionDescription *sdp);
    void OnIceCandidate(const IceCandidate* candidate);
    void OnIceCandidatesRemoved(std::vector<IceCandidate*> candidates);
    void OnIceConnected();
    void OnIceDisConnected();
    void OnPeerConnectionClosed();
    void OnPeerConnectionError(QString description);
    void OnStartRenderer(VideoWindow* render);

private:
    void OnConnectedToRoomInternal(AppRTCClient::SignalingParameters *params);
    void Disconnect();

private:
    Ui::MainWindow *ui;
    DirectRTCClient* direct_rtc_client_;
    AppRTCClient::RoomConnectionParameters* room_connection_parameters_;
    AppRTCClient::SignalingParameters* signaling_parameters_;
    PeerConnectionClient::PeerConnectionParamters* peer_connection_parameters_;
    PeerConnectionClient* peer_connection_client_;

private slots:
    void OnConnect();
};

#endif // MAINWINDOW_H
