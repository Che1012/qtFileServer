#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>

static const int PayloadSize = 64 * 1024; // 64 KB

class ClientHandler : public QObject
{
    Q_OBJECT

signals:
    void finished();
    void received(QString data);
public slots:
    void start();
    void stop();
    void startTransfer(QString data);
    void receiveData();
public:
    explicit ClientHandler(QObject* parent = nullptr);
    ~ClientHandler();

private:
    QTcpSocket *m_tcpSocket;
};


#endif // CLIENTHANDLER_H
