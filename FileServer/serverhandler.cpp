#include "QTextCodec"

#include "serverhandler.h"

void ServerHandler::start()
{
    qDebug() << "Started to do things";

    tcpServer->listen(QHostAddress::LocalHost, 6060);
//    stop();
}

void ServerHandler::stop()
{
    qDebug() << "ServerHandler stopped";
    emit finished();
    deleteLater();
}

void ServerHandler::acceptConnection()
{
    tcpServerConnection = tcpServer->nextPendingConnection();
    if (!tcpServerConnection) {
        qDebug("Error: got invalid pending connection!");
        return;
    }
    connect(tcpServerConnection, &QIODevice::readyRead,
            this,                &ServerHandler::updateServer);

    qDebug("Accepted connection");
    tcpServer->close();
}

void ServerHandler::startTransfer()
{
}

void ServerHandler::updateServer()
{
    int bytesReceived = int(tcpServerConnection->bytesAvailable());
    qDebug() << "bytesReceived" << bytesReceived;

    QByteArray barray = tcpServerConnection->readAll();

    //codecForMib(106) - UTF-8
    qDebug() << "Received:" << QTextCodec::codecForMib(106)->toUnicode(barray);
    tcpServerConnection->close();
}

void ServerHandler::displayError(QAbstractSocket::SocketError socketError)
{

}

ServerHandler::ServerHandler(QObject* parent) : QObject(parent)
{
    tcpServer = new QTcpServer();
    connect(tcpServer, &QTcpServer::newConnection,
            this,      &ServerHandler::acceptConnection);
}

ServerHandler::~ServerHandler()
{
    delete tcpServer;
    tcpServer->listen();
    qDebug() << "ServerHandler deleted";
}
