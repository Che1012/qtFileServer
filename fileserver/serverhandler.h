#ifndef SERVERHANDLER_H
#define SERVERHANDLER_H

#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QObject>
#include <QSocketNotifier>
#include <QFile>
#include "QDir"

#include "fileinfo.h"
#include "tcppacket.h"

class ServerHandler : public QObject
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
    QString         filesDirPath;
    QTcpServer      *m_tcpServer = nullptr;
    QTcpSocket      *m_tcpServerConnection = nullptr;
    QSocketNotifier *m_input = nullptr;
    int              payLoadSize = 64 * 1024; //64Kb

};



#endif // SERVERHANDLER_H
