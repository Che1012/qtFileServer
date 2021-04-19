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

void ClientHandler::startTransfer(QString data)
{
    m_tcpSocket->write(data.toUtf8());
}

void ClientHandler::receiveData()
{
//    QByteArray barray = m_tcpSocket->readAll();
    //codecForMib(106) - UTF-8
    QDataStream dataStream(m_tcpSocket);

    QList<FileInfo> *fileList = new QList<FileInfo>();

    QString res;
    dataStream >> *fileList;

    for(FileInfo info: *fileList)
        res += info.toStr() + "\n";
//    QList
//    if (createFileTree(rootItem))
//        emit treeCreated(rootItem);
//    else
//        delete
    emit received(res);
    emit filesReceived(fileList);
}

ClientHandler::ClientHandler(QObject *parent)
{
    m_tcpSocket = new QTcpSocket();
}

ClientHandler::~ClientHandler()
{
    delete m_tcpSocket;
}

