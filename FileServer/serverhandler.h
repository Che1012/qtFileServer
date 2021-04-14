#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QFile>

class ServerHandler : public QObject
{
    Q_OBJECT
public:
    enum Command
    {
        SendValue,
        NotCommand
    };

signals:
    void finished();

public slots:
    void start();
    void stop();

    void acceptConnection();
    void updateServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void onClientDisconnect();

    void checkCommand();
public:
    explicit ServerHandler(QObject* parent = nullptr);
    ~ServerHandler();
    Command toCommand(const QString &cmd);
    void startTransfer(const QString &value);

private:
    QTcpServer *m_tcpServer = nullptr;
    QTcpSocket *m_tcpServerConnection = nullptr;
    QFile* m_input = nullptr;
};



#endif // SERVERHANDLER_H
