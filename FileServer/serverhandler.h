#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QSocketNotifier>
#include <QFile>
#include "QDir"

class ServerHandler : public QObject
{
    Q_OBJECT
public:
    enum TCPRequest {
        SendFilesList = 200,
        Echo
    };
    enum Command {
        SendValue,
        NotCommand,
        Exit
    };


    explicit ServerHandler(QObject* parent = nullptr);
            ~ServerHandler();

    Command toCommand(const QString &cmd);
    void    startTransfer(const QString &value);

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
    void checkRequest(TCPRequest request);

private:
    QTcpServer      *m_tcpServer = nullptr;
    QTcpSocket      *m_tcpServerConnection = nullptr;
    QSocketNotifier *m_input = nullptr;
};



#endif // SERVERHANDLER_H
