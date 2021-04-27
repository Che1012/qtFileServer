#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>

#include "fileinfo.h"
#include "tcphandler.h"

class ClientHandler : public TCPHandler
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject* parent = nullptr);
            ~ClientHandler();

    bool createFileTree(QList<FileInfo> *fileList);

    QString getWorkingDirName() const;
    void setWorkingDirName(const QString &value);

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
    void sendFileReq(QString fileName);

private:
    QTcpSocket *m_tcpSocket;
    FileInfo *fileReceiving = nullptr;
    qint64 remainingSize;
    QString workingDirName;
};


#endif // CLIENTHANDLER_H
