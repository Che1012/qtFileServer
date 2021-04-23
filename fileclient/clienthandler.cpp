#include <QtNetwork/QHostAddress>
#include <QTextCodec>
#include <QDataStream>

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

void ClientHandler::receiveData()
{
    tcp::Command tcpCmd = tcp::recieveCmd(m_tcpSocket);
    qDebug() << "tcp request:" << tcpCmd;
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
        tcp::sendStringPacket(m_tcpSocket, &str);
        break;
    }
    case tcp::StartFilePacket: {
        QString fileName;
        quint64 fileSize;
        tcp::recieveFileInfo(m_tcpSocket, &fileName, &fileSize);
        fileReceiving = new FileInfo(fileName, fileSize);
        remainingSize = fileSize;
        disconnect(m_tcpSocket, &QIODevice::readyRead,
                   this, &ClientHandler::receiveData);
        connect(m_tcpSocket, &QIODevice::readyRead,
                this, &ClientHandler::receiveFile);
        break;
    }
    case tcp::FilesList: {
        QList<FileInfo> *fileList = new QList<FileInfo>();
        tcp::recieveFilesList(m_tcpSocket, fileList);
        emit filesReceived(fileList);
        break;
    }
    default:
        emit received("Tcp request is wrong");
        break;
    }
}

void ClientHandler::receiveFile()
{
    QFile file(workingDirName + fileReceiving->getName());
    if (!file.exists()) {
        if (!file.open(QFile::WriteOnly))
            return;
    }
    else if (!file.open(QFile::Append))
        return;
    QByteArray byteArray = m_tcpSocket->readAll();

    remainingSize -= byteArray.size();
    file.write(byteArray);
    file.close();
    if (remainingSize <= 0) {
        disconnect(m_tcpSocket, &QIODevice::readyRead,
                this, &ClientHandler::receiveFile);
        connect(m_tcpSocket, &QIODevice::readyRead,
                   this, &ClientHandler::receiveData);
    }
}

void ClientHandler::sendEcho(QString value)
{
    tcp::sendEchoPacket(m_tcpSocket, &value);
}

void ClientHandler::sendFileListReq()
{
    tcp::sendFilesListRequest(m_tcpSocket);
}

void ClientHandler::sendFileReq(QString fileName)
{
    tcp::sendFileRequest(m_tcpSocket, &fileName);
}

QString ClientHandler::getWorkingDirName() const
{
    return workingDirName;
}

void ClientHandler::setWorkingDirName(const QString &value)
{
    workingDirName = value;
}

ClientHandler::ClientHandler(QObject *parent)
    : QObject(parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}

