#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>

class ServerHandler : public QObject
{
    Q_OBJECT

signals:
    void finished();
public slots:
    void start();
    void stop();

    void acceptConnection();
    void startTransfer();
    void updateServer();
    void displayError(QAbstractSocket::SocketError socketError);
public:
    explicit ServerHandler(QObject* parent = nullptr);
    ~ServerHandler();

private:
    QTcpServer *tcpServer = nullptr;
    QTcpSocket *tcpServerConnection = nullptr;
};



#endif // SERVERHANDLER_H
