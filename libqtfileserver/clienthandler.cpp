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
            this, &ClientHandler::acceptConnection);
    connect(m_tcpSocket, &QAbstractSocket::disconnected,
            this, &ClientHandler::closeConnection);
    connect(&m_timeoutTimer, &QTimer::timeout,
            this, &ClientHandler::onTimeout);
}

void ClientHandler::stop()
{
    disconnect(m_tcpSocket, &QIODevice::readyRead,
            this, &ClientHandler::receiveData);
    m_tcpSocket->close();
}

void ClientHandler::receiveData()
{
    TCPCommand tcpCmd = recieveCmd(m_tcpSocket);
    qDebug() << "tcp request:" << tcpCmd;
    switch (tcpCmd) {
    case StringValue: {
        QString str;
        recieveString(m_tcpSocket, str);
        emit received(str);
        unhandleCmd();
        startNextCmd();
        break;
    }
    case Echo: {
        QString str;
        recieveString(m_tcpSocket, str);
        sendStringPacket(m_tcpSocket, str);
        break;
    }
    case StartFilePacket: {
        recieveFileInfo(m_tcpSocket);
        fileReceiving->createEmptyFile(getWorkingDirName());
        break;
    }
    case FilesList: {
        QList<FileInfo> *fileList = new QList<FileInfo>();
        recieveFilesList(m_tcpSocket, fileList);
        emit filesReceived(fileList);
        unhandleCmd();
        startNextCmd();
        break;
    }
    case FilePacket: {
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
    m_timeoutTimer.start();
}

void ClientHandler::sendEcho(QString value)
{
    putCmdToQueue(CommandType(Echo, value));
    startNextCmd();
}

void ClientHandler::sendFileListReq()
{
    putCmdToQueue(CommandType(SendFilesList, ""));
    startNextCmd();
}

void ClientHandler::sendFileReq(QString fileName)
{
    putCmdToQueue(CommandType(SendFile, fileName));
    startNextCmd();
}

void ClientHandler::acceptConnection()
{
    emit connectionStatusChanged(true);
}

void ClientHandler::closeConnection()
{
    emit connectionStatusChanged(false);
}

void ClientHandler::onTimeout()
{
    while(m_tcpSocket->bytesAvailable() > 0)
        receiveData();
}

bool ClientHandler::startNextCmd()
{
    if (isCmdHandling())
        return false;
    CommandType cmdType = takeCmdFromQueue();
    if (cmdType.cmd == NotCommand)
        return false;
    handleCmd();
    switch (cmdType.cmd) {
    case SendFilesList:
        sendFilesListRequest(m_tcpSocket);
        break;
    case Echo:
        sendEchoPacket(m_tcpSocket, cmdType.value);
        break;
    case SendFile:
        sendFileRequest(m_tcpSocket, cmdType.value);
        break;
    default:
        break;
    }
    return true;
}

ClientHandler::ClientHandler(QObject *parent)
    : TCPHandler(parent), m_timeoutTimer(this)
{
    m_tcpSocket = new QTcpSocket();

    m_timeoutTimer.setSingleShot(true);
    m_timeoutTimer.setInterval(100);
}

ClientHandler::~ClientHandler()
{
    if (!m_tcpSocket)
        delete m_tcpSocket;
    if (!fileReceiving)
        delete fileReceiving;
}

bool ClientHandler::isConnected()
{
    return  m_tcpSocket->isOpen();
}

