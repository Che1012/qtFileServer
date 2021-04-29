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
    enum TermCommand {
        SendValue,
        NotCommand,
        Exit
    };
    TermCommand toCommand(const QString &cmd);

    explicit ServerHandler(QObject* parent = nullptr,
                           QString filesDirPath = QDir::currentPath());
    void init();
    ~ServerHandler();

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

private:
    QTcpServer      *m_tcpServer = nullptr;
    QTcpSocket      *m_tcpServerConnection = nullptr;
    QSocketNotifier *m_input = nullptr;
};



#endif // SERVERHANDLER_H
