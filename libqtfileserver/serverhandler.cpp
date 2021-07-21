#include "QTextCodec"
#include "serverhandler.h"


void ServerHandler::start()
{
    m_tcpServer->listen(QHostAddress::LocalHost, 6060);
    QTextStream(stdout) << "Server started to listen for a connection\n";
}

void ServerHandler::stop()
{
   QTextStream(stdout) << "ServerHandler stopped\n";
   if (m_tcpServerConnection->isOpen())
       m_tcpServerConnection->close();
   emit finished();
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

   QString info = formatConnectionInfo(m_tcpServerConnection->peerName(),
                                       m_tcpServerConnection->peerAddress().toString(),
                                       m_tcpServerConnection->peerPort());
   emit connected(info);
   QTextStream(stdout) << "Accepted connection\n";
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
    QString info = formatConnectionInfo(m_tcpServerConnection->peerName(),
                                        m_tcpServerConnection->peerAddress().toString(),
                                        m_tcpServerConnection->peerPort());
    emit disconnected(info);
    m_tcpServerConnection->deleteLater();
    m_tcpServerConnection = nullptr;
    QTextStream(stdout) << "Client disconnected\n";
    m_tcpServer->listen(QHostAddress::LocalHost, 6060);
    QTextStream(stdout) << "Server started to listen for a connection\n";
}

void ServerHandler::checkCommand()
{
    QTextStream stream(stdin);
    QString cmd;
    QString value;
    stream >> cmd;
    TermCommand command = toCommand(cmd);

    switch (command) {
    case TermCommand::Help:
        QTextStream(stdout) << "Commands:\n"
                            << "/h /help - show help\n"
                            << "/echo [value] - send echo to client\n"
                            << "/exit - stop the server and exit from the program\n";
        break;
    case TermCommand::Echo:
        stream >> value;
        sendEchoPacket(m_tcpServerConnection, value);
        break;
    case TermCommand::Exit:
        stop();
        break;
    case TermCommand::NotCommand:
        QTextStream(stdout) << "Not a command, try another one..\n";
        break;
    }

    qDebug() << "Command from terminal:" << cmd << value;
}

QString ServerHandler::formatConnectionInfo(const QString &name, const QString &ip, quint16 port)
{
    QString info = QString("%1 %2:%3")
            .arg(name)
            .arg(ip)
            .arg(port);
    return info;
}

void ServerHandler::init()
{
    m_tcpServer = new QTcpServer();
    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &ServerHandler::acceptConnection);
}

ServerHandler::TermCommand ServerHandler::toCommand(const QString &cmd)
{
    if (cmd == "\echo")
        return TermCommand::Echo;
    if (cmd == "\help" || cmd == "\h")
        return TermCommand::Help;
    if (cmd == "\exit")
        return TermCommand::Exit;

    return TermCommand::NotCommand;
}

ServerHandler::ServerHandler(QObject *parent, const QString &workingDirName)
    : TCPHandler(parent, workingDirName)
{
}

ServerHandler::~ServerHandler()
{
    if (m_tcpServer)
        delete  m_tcpServer;
    if (m_input)
        delete  m_input;

    qDebug() << "ServerHandler deleted";
}
