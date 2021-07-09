#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QSocketNotifier>
#include <QFile>
#include "QDir"

#include "fileinfo.h"
#include "tcphandler.h"

class ServerHandler : public TCPHandler
{
    Q_OBJECT
public:
    enum class TermCommand {
        SendValue,
        NotCommand,
        Exit
    };
    TermCommand toCommand(const QString &cmd);

    explicit ServerHandler(QObject* parent = nullptr,
                           const QString &filesDirPath = QDir::currentPath());
    ~ServerHandler();

    void init();

signals:
    void finished();

    void connected(const QString &info);
    void disconnected(const QString &info);

public slots:
    void start();
    void stop();

    void acceptConnection();
    void updateServer();
    void displayError(QAbstractSocket::SocketError socketError);
    void onClientDisconnect();

//    void checkCommand();

private:
    QTcpServer      *m_tcpServer = nullptr;
    QTcpSocket      *m_tcpServerConnection = nullptr;
    QSocketNotifier *m_input = nullptr;

    QString formatConnectionInfo(const QString& name,
                                 const QString& ip,
                                 quint16 port);
};



#endif // SERVERHANDLER_H
