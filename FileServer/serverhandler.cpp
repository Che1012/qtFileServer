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
    qDebug() << "Received:" << QTextCodec::codecForMib(106)->toUnicode(barray);
    m_tcpServerConnection->close();
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
    QTextStream stream(m_input);
    QString cmd;
    QString value;
    stream >> cmd >> value;
    Command command = toCommand(cmd);

    switch (command) {
    case SendValue:
        startTransfer(value);
        break;
    case NotCommand:
        qDebug() << "Not a command, try another one..";
        break;
    }
}

ServerHandler::Command ServerHandler::toCommand(const QString &cmd)
{
    if (cmd == "--send")
        return SendValue;
    return NotCommand;
}

void ServerHandler::startTransfer(const QString &value)
{
    m_tcpServerConnection->write(value.toUtf8());
}

ServerHandler::ServerHandler(QObject* parent) : QObject(parent)
{
    m_tcpServer = new QTcpServer();
    connect(m_tcpServer, &QTcpServer::newConnection,
            this, &ServerHandler::acceptConnection);

    QFile* m_input = new QFile();
    m_input ->open(stdin, QFile::ReadOnly);

    QObject::connect(m_input , &QIODevice::readyRead,
                     this, &ServerHandler::checkCommand);
}

ServerHandler::~ServerHandler()
{
    delete  m_tcpServer;

    m_input->close();
    delete  m_input;
    qDebug() << "ServerHandler deleted";
}
