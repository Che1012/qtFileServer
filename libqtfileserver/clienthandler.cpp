#include <QtNetwork/QHostAddress>
#include <QTextCodec>
#include <QDataStream>
#include <QDir>

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
    tcp::Command tcpCmd = recieveCmd(m_tcpSocket);
    qDebug() << "tcp request:" << tcpCmd;
    switch (tcpCmd) {
    case tcp::StringValue: {
        QString str;
        recieveString(m_tcpSocket, str);
        emit received(str);
        break;
    }
    case tcp::Echo: {
        QString str;
        recieveString(m_tcpSocket, str);
        sendStringPacket(m_tcpSocket, str);
        break;
    }
    case tcp::StartFilePacket: {
        QString fileName;
        quint64 fileSize;
        recieveFileInfo(m_tcpSocket, fileName, fileSize);
        fileReceiving = new FileInfo(fileName, fileSize);
        remainingSize = fileSize;

        QFile file(workingDirName + "/" + fileName);
        file.open(QFile::WriteOnly);
        file.close();

        disconnect(m_tcpSocket, &QIODevice::readyRead,
                   this, &ClientHandler::receiveData);
        connect(m_tcpSocket, &QIODevice::readyRead,
                this, &ClientHandler::receiveFile);
        break;
    }
    case tcp::FilesList: {
        QList<FileInfo> *fileList = new QList<FileInfo>();
        recieveFilesList(m_tcpSocket, fileList);
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
    QFile file(workingDirName + "/" + fileReceiving->getName());

    if (!file.open(QFile::Append))
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
    sendEchoPacket(m_tcpSocket, value);
}

void ClientHandler::sendFileListReq()
{
    sendFilesListRequest(m_tcpSocket);
}

void ClientHandler::sendFileReq(QString fileName)
{
    sendFileRequest(m_tcpSocket, fileName);
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
    : TCPHandler(parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}

