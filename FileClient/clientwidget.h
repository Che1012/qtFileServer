#ifndef CLIENTWIDGET_H
#define CLIENTWIDGET_H

#include <QWidget>

#include "clienthandler.h"
#include "../FileServer/fileinfo.h"

QT_BEGIN_NAMESPACE
namespace Ui { class ClientWidget; }
QT_END_NAMESPACE

class ClientWidget : public QWidget
{
    Q_OBJECT

public:
    const int NAME_COLUMN = 0;
    const int SIZE_COLUMN = 1;
    const int DATE_COLUMN = 2;
    const int CHILD_NOT_FOUND = -1;

    ClientWidget(QWidget *parent = nullptr);
    ~ClientWidget();

signals:
    void sendData(QString data);

private slots:
    void on_sendBtn_clicked();
    void receivedFromClient(QString data);
    void on_connectBtn_clicked();
    void updateTreeWidget(QList<FileInfo> *fileList);

private:
    Ui::ClientWidget *m_ui;
    ClientHandler m_client;

    bool addTreeNode(QTreeWidgetItem* parent, FileInfo& node, int level);
    int  getTreeChild(QTreeWidgetItem* parent, QString nodeName);
};
#endif // CLIENTWIDGET_H
