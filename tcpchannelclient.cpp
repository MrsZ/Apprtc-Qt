#include "tcpchannelclient.h"

TCPChannelClient::TCPChannelClient()
{

}

TCPChannelClient::TCPChannelClient(QString ip, int port)
{
    // check whther it local address or not
    QHostAddress address;
    address.setAddress(ip);
       std::cout << (address.toString()).toStdString() << std::endl;
    if (address.isLoopback() || address == QHostAddress::AnyIPv4) {
        // server
        std::cout << "server" << std::endl;
        is_server_ = true;
        server_ = new QTcpServer();
        server_->listen(address, port);
    } else {
        // client
        is_server_ = false;
        std::cout << "client" << std::endl;
        client_ = new QTcpSocket();
        client_->connectToHost(address, port);
    }
    RegisterObserver(is_server_);
}

void TCPChannelClient::Connect(QString ip, int port)
{

}

void TCPChannelClient::Disconnect()
{

}

void TCPChannelClient::Send(QString message)
{
    QByteArray block;
    block = message.toUtf8() + '\n';

    QDataStream out(&block, QIODevice::WriteOnly);

    if (is_server_) {
        std::cout << "server message sent: " << message.toStdString() << std::endl;
        send_socket_->write(block);
    } else {
        std::cout << "client message sent: " << message.toStdString() << std::endl;
        client_->write(block);
    }
}

void TCPChannelClient::RegisterTCPObserver(TCPChannelEvents* event)
{
    events_ = event;
}

void TCPChannelClient::RegisterObserver(bool is_server)
{
    if (is_server) {
        server_->newConnection();
        connect(server_, &QTcpServer::newConnection, this, &TCPChannelClient::OnConnected);
    } else {
        connect(client_, &QTcpSocket::readyRead, this, &TCPChannelClient::OnReadyRead);
        connect(client_, &QTcpSocket::connected, this, &TCPChannelClient::OnConnected);
        connect(client_, &QTcpSocket::disconnected, this, &TCPChannelClient::OnDisConnected);
        typedef void (QAbstractSocket::*QAbstractSocketErrorSignal)(QAbstractSocket::SocketError);
        connect(client_, static_cast<QAbstractSocketErrorSignal>(&QAbstractSocket::error), this, &TCPChannelClient::OnError);
    }
}

// slots
void TCPChannelClient::OnConnected()
{
    if (is_server_) {
        send_socket_ = server_->nextPendingConnection();
        // connect for tcpserver
        connect(send_socket_, &QTcpSocket::readyRead, this, &TCPChannelClient::OnServerReadyRead);
    }
    events_->OnTCPConnected(is_server_);
}

void TCPChannelClient::OnDisConnected()
{
    events_->OnTCPClose();
}

void TCPChannelClient::OnError(QAbstractSocket::SocketError error)
{
    /*
    switch (socketError) {
        case QAbstractSocket::RemoteHostClosedError:
            break;
        case QAbstractSocket::HostNotFoundError:

            break;
        case QAbstractSocket::ConnectionRefusedError:

            break;
        default:
           events_->OnTCPError(client_->errorString());
        }
    */
    events_->OnTCPError(client_->errorString());
}

void TCPChannelClient::OnReadyRead()
{
    std::cout << "OnReadyRead()" << std::endl;
    while(true) {
        char buf[4096];
        qint64 line_length = client_->readLine(buf, sizeof(buf));
        if (line_length != -1) {
            QString message(buf);
            std::cout << "client read msg: " << message.toStdString() << std::endl;
            // No data received
            if (message == NULL) {
                 break;
            }
            events_->OnTCPMessage(message);
        } else {
            break;
        }
    }
}

// Reading client messages for server
void TCPChannelClient::OnServerReadyRead()
{
    std::cout << "OnServerReadyRead()" << std::endl;
    while(true) {
        char buf[4096];
        qint64 line_length = send_socket_->readLine(buf, sizeof(buf));
        if (line_length != -1) {
            QString message(buf);
            std::cout << "server read msg: " << message.toStdString() << std::endl;
            // No data recevided
            if (message == NULL) {
                break;
            }
            events_->OnTCPMessage(message);
        } else {
            break;
        }
    }
}
