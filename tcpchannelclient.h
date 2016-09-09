#ifndef TCPCHANNELCLIENT_H
#define TCPCHANNELCLIENT_H

#include <iostream>

#include <QObject>
#include <QTcpSocket>
#include <QTcpServer>
#include <QDataStream>
#include <QThread>
#include <QString>

// TCP callbacks
struct TCPChannelEvents {
    virtual void OnTCPConnected(bool is_server) = 0;
    virtual void OnTCPMessage(QString message) = 0;
    virtual void OnTCPError(QString description) = 0;
    virtual void OnTCPClose() = 0;

    virtual ~TCPChannelEvents() {}
};

// Create Tcp Sockets, this calss mimics AppRTC TCPChannelClient class
// If the IP is local, create a server otherwise create a client
class TCPChannelClient : public QObject
{
    Q_OBJECT
public:
    TCPChannelClient();
    TCPChannelClient(QString ip, int port);

    void Connect(QString ip, int port);
    void Disconnect();
    void Send(QString message);
    void RegisterObserver(bool is_server);
    void RegisterTCPObserver(TCPChannelEvents* event);
    //void ReportError(QString message);

public slots:
    void OnConnected();
    void OnDisConnected();
    void OnError(QAbstractSocket::SocketError);
    void OnReadyRead();
    void OnServerReadyRead();


private:
    QTcpServer* server_;
    QTcpSocket* client_;
    QTcpSocket* send_socket_;
    TCPChannelEvents* events_;
    QString ip_address_;
    bool is_server_;
};

#endif // TCPCHANNELCLIENT_H
