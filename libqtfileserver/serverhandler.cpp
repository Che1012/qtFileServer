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
    TCPCommand cmd = recieveCmd(m_tcpServerConnection);
    qDebug() << "tcp request:" << cmd;
    switch (cmd) {
    case SendFilesList: {
        QList<FileInfo> fileList;
        FileInfo::getFilesList(&fileList, getWorkingDirName(), "");
        sendFilesList(m_tcpServerConnection, &fileList);
        break;
    }
    case Echo: {
        QString str;
        recieveString(m_tcpServerConnection, str);
        sendStringPacket(m_tcpServerConnection, str);
        break;
    }
    case StringValue: {
        QString str;
        recieveString(m_tcpServerConnection, str);
        QTextStream outStream(stdout);
        outStream << str;
        break;
    }
    case SendFile: {
        QString fileName;
        recieveFileName(m_tcpServerConnection, fileName);
        qDebug() << "preparing to find file:" << fileName;
        QFile file(getWorkingDirName() + "/" + fileName);
        sendFile(m_tcpServerConnection, &file, fileName);
        break;
    }
    default:
        qDebug("Tcp request is wrong");
        break;
    }
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
    case TermCommand::SendValue:
        stream >> value;
        sendStringPacket(m_tcpServerConnection, value);
        break;
    case TermCommand::Exit:
        stop();
        break; 
    case TermCommand::NotCommand:
        qDebug() << "Not a command, try another one..";
        break;
    }

    qDebug() << "Command" << cmd << value;
}

void ServerHandler::init()
{
    m_tcpServer = new QTcpServer();
    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &ServerHandler::acceptConnection);

    m_input = new QSocketNotifier(fileno(stdin), QSocketNotifier::Read, this);

    QObject::connect(m_input, &QSocketNotifier::activated,
                     this, &ServerHandler::checkCommand);
}

ServerHandler::TermCommand ServerHandler::toCommand(const QString &cmd)
{
    if (cmd == "send")
        return TermCommand::SendValue;
    if (cmd == "exit")
        return TermCommand::Exit;
    return TermCommand::NotCommand;
}

ServerHandler::ServerHandler(QObject *parent, QString workingDirName)
    : TCPHandler(parent, workingDirName)
{
}

ServerHandler::~ServerHandler()
{
    delete  m_tcpServer;

    delete  m_input;
    qDebug() << "ServerHandler deleted";
}
