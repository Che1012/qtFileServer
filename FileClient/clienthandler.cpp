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

void ClientHandler::startTransfer(QString data)
{
    m_tcpSocket->write(data.toUtf8());
}

void ClientHandler::receiveData()
{
    QByteArray barray = m_tcpSocket->readAll();
    //codecForMib(106) - UTF-8
    QString data = QTextCodec::codecForMib(106)->toUnicode(barray);
    qDebug() << "Received size:" << data.size();
    qDebug() << "Received:" << data;
    emit received(data);
    QList
    if (createFileTree(rootItem))
        emit treeCreated(rootItem);
    else
        delete
}

ClientHandler::ClientHandler(QObject *parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}

bool ClientHandler::createFileTree(QTreeWidgetItem *rootItem)
{

}
