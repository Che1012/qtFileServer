#include <QtNetwork/QHostAddress>
#include <QTextCodec>
#include <QDataStream>

#include "clienthandler.h"
#include "tcppacket.h"

void ClientHandler::start()
{
    m_tcpSocket->connectToHost(QHostAddress::LocalHost, 6060);
    connect(m_tcpSocket, &QIODevice::readyRead,
            this, &ClientHandler::receiveData);
}

void ClientHandler::stop()
{

}

void ClientHandler::startTransfer(QString &data)
{
    m_tcpSocket->write(data.toUtf8());
}

void ClientHandler::receiveData()
{
    tcp::Command tcpCmd = tcp::recieveCmd(m_tcpSocket);
    switch (tcpCmd) {
    case tcp::StringValue: {
        QString str;
        tcp::recieveString(m_tcpSocket, &str);
        emit received(str);
        break;
    }
    case tcp::Echo: {
        QString str;
        tcp::recieveString(m_tcpSocket, &str);
        QDataStream dataStream(m_tcpSocket);
        dataStream << static_cast<int>(tcp::StringValue) << str;
        }
        break;
    case tcp::StartFilePacket:
        break;
    case tcp::FilePacket:
        break;
    case tcp::FilesList: {
        QList<FileInfo> *fileList = new QList<FileInfo>();
        tcp::recieveFileList(m_tcpSocket, fileList);
        emit filesReceived(fileList);
    }
        break;
    default:
        emit received("Tcp request is wrong");
        break;
    }
}

ClientHandler::ClientHandler(QObject *parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}

