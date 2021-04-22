#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTreeWidgetItem>

#include "fileinfo.h"
#include "tcppacket.h"

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject* parent = nullptr);
            ~ClientHandler();

    bool createFileTree(QList<FileInfo> *fileList);

signals:
    void finished();
    void received(QString data);
    void filesReceived(QList<FileInfo> *fileList);

public slots:
    void start();
    void stop();
    void receiveData();
    void receiveFile();
    void sendEcho(QString value);
    void sendFileListReq();
    void sendFileReq(QString name);

private:
    QTcpSocket *m_tcpSocket;
    FileInfo *fileReceiving = nullptr;
    qint64 remainingSize;
};


#endif // CLIENTHANDLER_H
