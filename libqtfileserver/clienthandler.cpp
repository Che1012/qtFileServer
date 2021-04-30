#include <QtNetwork/QHostAddress>
#include <QTextCodec>
#include <QDataStream>
#include <QDir>

#include "clienthandler.h"

void ClientHandler::start(const QString &ip, const int port)
{
    m_tcpSocket->connectToHost(ip, port);
    connect(m_tcpSocket, &QIODevice::readyRead,
            this, &ClientHandler::receiveData);

    connect(m_tcpSocket, &QAbstractSocket::connected,
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
        unhandleCmd();
        startNextCmd();
        break;
    }
    case tcp::Echo: {
        QString str;
        recieveString(m_tcpSocket, str);
        sendStringPacket(m_tcpSocket, str);
        break;
    }
    case tcp::StartFilePacket: {
        recieveFileInfo(m_tcpSocket);
        fileReceiving->createEmptyFile(getWorkingDirName());
        break;
    }
    case tcp::FilesList: {
        QList<FileInfo> *fileList = new QList<FileInfo>();
        recieveFilesList(m_tcpSocket, fileList);
        emit filesReceived(fileList);
        unhandleCmd();
        startNextCmd();
        break;
    }
    case tcp::FilePacket: {
        receiveFile(m_tcpSocket);
        if (remainRFileSize <= 0) {
            delete fileReceiving;
            unhandleCmd();
            startNextCmd();
        }
        break;
    }
    default:
        emit received("Tcp request is wrong");
        break;
    }
}

void ClientHandler::sendEcho(QString value)
{
    putCmdToQueue(tcp::CommandType(tcp::Echo, value));
    startNextCmd();
}

void ClientHandler::sendFileListReq()
{
    putCmdToQueue(tcp::CommandType(tcp::SendFilesList, ""));
    startNextCmd();
}

void ClientHandler::sendFileReq(QString fileName)
{
    putCmdToQueue(tcp::CommandType(tcp::SendFile, fileName));
    startNextCmd();
}

bool ClientHandler::startNextCmd()
{
    if (isCmdHandling())
        return false;
    tcp::CommandType cmdType = takeCmdFromQueue();
    if (cmdType.cmd == tcp::NotCommand)
        return false;
    handleCmd();
    switch (cmdType.cmd) {
    case tcp::SendFilesList:
        sendFilesListRequest(m_tcpSocket);
        break;
    case tcp::Echo:
        sendEchoPacket(m_tcpSocket, cmdType.value);
        break;
    case tcp::SendFile:
        sendFileRequest(m_tcpSocket, cmdType.value);
        break;
    default:
        break;
    }
    return true;
}

ClientHandler::ClientHandler(QObject *parent)
    : TCPHandler(parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    if (!m_tcpSocket)
        delete m_tcpSocket;
    if (!fileReceiving)
        delete fileReceiving;
}

