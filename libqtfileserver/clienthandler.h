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
    bool isConnected();

    bool startNextCmd() override;
signals:
    void finished();
    void received(QString data);
    void filesReceived(QList<FileInfo> *fileList);

    void connectionStatusChanged(bool status);

public slots:
    void start(const QString &ip, const int port);
    void stop();

    void receiveData();
    void sendEcho(QString value);
    void sendFileListReq();
    void sendFileReq(QString fileName);

    void acceptConnection();
    void closeConnection();
private:
    QTcpSocket *m_tcpSocket;
};


#endif // CLIENTHANDLER_H
