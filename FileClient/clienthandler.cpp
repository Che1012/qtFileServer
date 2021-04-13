#include <QtNetwork/QHostAddress>

#include "clienthandler.h"


void ClientHandler::start()
{
    tcpSocket->connectToHost(QHostAddress::LocalHost, 6060);
}

void ClientHandler::stop()
{

}

void ClientHandler::startTransfer()
{
    tcpSocket->write(QByteArray("Hello World!"));
}

ClientHandler::ClientHandler(QObject *parent)
{
    tcpSocket = new QTcpSocket();
    connect(tcpSocket, &QAbstractSocket::connected,
            this,      &ClientHandler::startTransfer);
}

ClientHandler::~ClientHandler()
{
    delete tcpSocket;
}
