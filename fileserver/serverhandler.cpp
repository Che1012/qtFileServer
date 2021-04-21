#include "QTextCodec"
#include "serverhandler.h"


void ServerHandler::start()
{
    qDebug() << "Started to do things";

     m_tcpServer->listen(QHostAddress::LocalHost, 6060);
     //stop();
}

void ServerHandler::stop()
{
    qDebug() << "ServerHandler stopped";
    if (m_tcpServerConnection->isOpen())
        m_tcpServerConnection->close();
    emit finished();
    deleteLater();
}

void ServerHandler::acceptConnection()
{
    m_tcpServerConnection =  m_tcpServer->nextPendingConnection();
    if (!m_tcpServerConnection) {
        qDebug("Error: got invalid pending connection!");
        return;
    }
    connect(m_tcpServerConnection, &QIODevice::readyRead,
            this, &ServerHandler::updateServer);
    connect(m_tcpServerConnection, &QTcpSocket::disconnected,
            this, &ServerHandler::onClientDisconnect);

    qDebug("Accepted connection");
    m_tcpServer->close();
}

void ServerHandler::updateServer()
{    
    int bytesReceived = int(m_tcpServerConnection->bytesAvailable());
    qDebug() << "bytesReceived" << bytesReceived;

    QByteArray barray = m_tcpServerConnection->readAll();

    //codecForMib(106) - UTF-8
    QString receivedStr = QTextCodec::codecForMib(106)->toUnicode(barray);
    qDebug() << "Received:" << receivedStr;

    QTextStream receivedStream(&receivedStr);
    int cmdNum;
    receivedStream >> cmdNum;
    qDebug() << "cmdNum" << cmdNum;
    checkRequest(static_cast<tcp::Command>(cmdNum));
}

void ServerHandler::displayError(QAbstractSocket::SocketError socketError)
{

}

void ServerHandler::onClientDisconnect()
{
    qDebug() << "Client disconnected";
    m_tcpServer->listen(QHostAddress::LocalHost, 6060);
}

void ServerHandler::checkCommand()
{
    QTextStream stream(stdin);
    QString cmd;
    QString value;
    stream >> cmd;
    TermCommand command = toCommand(cmd);

    switch (command) {
    case SendValue:
        stream >> value;
        startTransfer(value);
        break;
    case Exit:
        stop();
        break; 
    case NotCommand:
        qDebug() << "Not a command, try another one..";
        break;
    }

    qDebug() << "Command" << cmd << value;
}

void ServerHandler::checkRequest(tcp::Command request)
{
    switch (request) {
    case tcp::SendFilesList: {
        QList<FileInfo> fileList;
        FileInfo::getFilesList(&fileList, QDir::currentPath(), "");

        QDataStream dataStream(m_tcpServerConnection);
        dataStream << static_cast<int>(tcp::FilesList) << fileList;
        break;
    }
    case tcp::Echo: {
        QString str;
        tcp::recieveString(m_tcpServerConnection, &str);
        QDataStream dataStream(m_tcpServerConnection);
        dataStream << static_cast<int>(tcp::StringValue) << str;
        break;
    }
    case tcp::StringValue: {
        QString str;
        tcp::recieveString(m_tcpServerConnection, &str);
        qDebug() << str;
        break;
    }
    case tcp::SendFile: {
        FileInfo info;
        tcp::recieveFileInfo(m_tcpServerConnection, &info);
        // find file
        // add to files pool
        QDataStream dataStream(m_tcpServerConnection);
        dataStream << static_cast<int>(tcp::StartFilePacket)
                   << info;
        // send file in for loop
        break;
    }
    default:
        qDebug("Tcp request is wrong");
        break;
    }
}

//void ServerHandler::sendFile(FileInfo &fileInfo)
//{
//    if (m_tcpServerConnection == nullptr || m_tcpServerConnection->isOpen())
//        return;

//    m_tcpServerConnection->write();
//}


ServerHandler::TermCommand ServerHandler::toCommand(const QString &cmd)
{
    if (cmd == "send")
        return SendValue;
    if (cmd == "exit")
        return Exit;
    return NotCommand;
}

void ServerHandler::startTransfer(const QString &value)
{
    if (m_tcpServerConnection == nullptr || !m_tcpServerConnection->isOpen()) {
        qDebug() << "Connection is not established";
        return;
    }
    m_tcpServerConnection->write(value.toUtf8());
}

ServerHandler::ServerHandler(QObject* parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer();
    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &ServerHandler::acceptConnection);

    m_input = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);

    QObject::connect(m_input, &QSocketNotifier::activated,
                     this, &ServerHandler::checkCommand);
}

ServerHandler::~ServerHandler()
{
    delete  m_tcpServer;

    delete  m_input;
    qDebug() << "ServerHandler deleted";
}
