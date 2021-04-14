#include <QtNetwork/QHostAddress>
#include "QTextCodec"

#include "clienthandler.h"


void ClientHandler::start()
{
    m_tcpSocket->connectToHost(QHostAddress::LocalHost, 6060);
    connect(m_tcpSocket, &QIODevice::readyRead,
            this, &ClientHandler::receiveData);
}

void ClientHandler::stop()
{

}

void ClientHandler::startTransfer()
{
    m_tcpSocket->write(QByteArray("Hello World!"));
}

void ClientHandler::receiveData()
{
    QByteArray barray = m_tcpSocket->readAll();
    //codecForMib(106) - UTF-8
    QString data = QTextCodec::codecForMib(106)->toUnicode(barray);
    qDebug() << "Received:" << data;
    qDebug() << sizeof(data);
    emit received(data);
}

ClientHandler::ClientHandler(QObject *parent)
{
    m_tcpSocket = new QTcpSocket();
    connect(m_tcpSocket, &QAbstractSocket::connected,
            this,      &ClientHandler::startTransfer);
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}
