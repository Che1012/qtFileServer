#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>
#include <QTreeWidgetItem>

#include "clienthandler.h"
#include "fileinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    enum Column {
        Name,
        Size,
        Date,
        Status
    };
    const int CHILD_NOT_FOUND = -1;

    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

signals:
    void sendData(QString data);
    void sendFile(QString name);
    void sendFilesList();

private slots:
    void receivedFromClient(QString data);
    void on_connectBtn_clicked();

    void updateTreeWidget(QList<FileInfo> *fileList);

    void on_fileBtn_clicked();

    void on_syncBtn_clicked();

    void on_echoBtn_clicked();

    void showContextMenu(const QPoint &pos);

private:
    Ui::ClientWidget *m_ui;
    ClientHandler m_client;
    QList<FileInfo>* currFileInfoList = nullptr;

    bool checkTreeNode(QList<FileInfo> *list, const FileInfo &node);
    bool addTreeNode(QTreeWidgetItem *parent, const FileInfo &node, int level);
    int  getTreeChild(QTreeWidgetItem *parent, const QString &nodeName);

    void updateTreeNode(QTreeWidgetItem *node);
};
#endif // CLIENTWIDGET_H
