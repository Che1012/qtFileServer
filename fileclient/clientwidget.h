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

    void saveSettings();
    void loadSettings();

signals:
    void sendData(QString data);
    void sendFile(QString name);
    void sendFilesList();

private slots:
    void recievedfilePacket(qint64 fileSize, qint64 recievedDataSize);

    // UI
    void updateConnectionStatus(bool status);
    void updateTreeWidget(QList<FileInfo> *fileList);
    void showContextMenu(const QPoint &pos);

    void on_connectBtn_clicked();
    void on_syncBtn_clicked();
    void on_updateFilesBtn_clicked();

private:
    Ui::ClientWidget *m_ui;
    ClientHandler m_client;
    QList<FileInfo>* currFileInfoList = nullptr;
    QList<FileInfo>* filesAtClientList = nullptr;

    // FileTree
    bool checkTreeNode(QList<FileInfo> *list, const FileInfo &node);
    bool addTreeNode(QTreeWidgetItem *parent, const FileInfo &node, int level);
    int  getTreeChild(QTreeWidgetItem *parent, const QString &nodeName);

    void updateTreeNode(QTreeWidgetItem *node);
    void updateTreeNode(const QString &nodeName);
};
#endif // CLIENTWIDGET_H
