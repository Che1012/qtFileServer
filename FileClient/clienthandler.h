#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QObject>
#include <QtNetwork/QTcpSocket>
#include <QTreeWidgetItem>

static const int PayloadSize = 64 * 1024; // 64 KB

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject* parent = nullptr);
            ~ClientHandler();

    bool createFileTree(QTreeWidgetItem *rootItem);

signals:
    void finished();
    void received(QString data);
    void treeCreated(QTreeWidgetItem *root);

public slots:
    void start();
    void stop();
    void startTransfer(QString data);
    void receiveData();

private:
    QTcpSocket *m_tcpSocket;
};


#endif // CLIENTHANDLER_H
