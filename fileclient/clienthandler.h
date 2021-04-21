#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTreeWidgetItem>

#include "fileinfo.h"

static const int PayloadSize = 64 * 1024; // 64 KB

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
    void startTransfer(QString &data);
    void receiveData();

private:
    QTcpSocket *m_tcpSocket;
};


#endif // CLIENTHANDLER_H
