#include <QTreeWidget>

#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ClientWidget)
{
    m_ui->setupUi(this);
    connect(&m_client, &ClientHandler::received,
            this, &ClientWidget::receivedFromClient);
    connect(this, &ClientWidget::sendData,
            &m_client, &ClientHandler::startTransfer);
    connect(&m_client, &ClientHandler::filesReceived,
            this, &ClientWidget::updateTreeWidget);
}

ClientWidget::~ClientWidget()
{
    delete m_ui;
}


void ClientWidget::on_sendBtn_clicked()
{
    emit sendData(m_ui->textToSend->toPlainText());
}

void ClientWidget::receivedFromClient(QString data)
{
    QString str = m_ui->textReceived->toPlainText() + "\n" + data;
    m_ui->textReceived->setText(str);
}

void ClientWidget::on_connectBtn_clicked()
{
    m_client.start();
}

void ClientWidget::updateTreeWidget(QList<FileInfo> *fileList)
{
    QTreeWidgetItem* treeRoot = m_ui->treeWidget->takeTopLevelItem(0);
    if (treeRoot != nullptr)
        delete treeRoot;
    treeRoot = new QTreeWidgetItem(m_ui->treeWidget);

    for (FileInfo info : *fileList)
        if (!addTreeNode(treeRoot, info, 0))
            qDebug() << "Error at creating tree";
    treeRoot->setExpanded(true);
    delete fileList;
}

bool ClientWidget::addTreeNode(QTreeWidgetItem* parent, FileInfo &node, int level)
{
    if (parent == nullptr)
        return false;
    QString nodeSubName = node.getName().split("/").at(level);
    int     nodeNameLevels = node.getName().split("/").size();
    int     childPos = getTreeChild(parent, nodeSubName);
    // if it is final level of node
    if (level == nodeNameLevels - 1) {
        QTreeWidgetItem* item;
        if (childPos == CHILD_NOT_FOUND)
            item = new QTreeWidgetItem(parent);
        else
            item = parent->child(childPos);
        item->setText(NAME_COLUMN, nodeSubName);
        item->setText(SIZE_COLUMN, QString::number(node.getSize()));
        item->setText(DATE_COLUMN, node.getDate().toString());
        return true;
    }
    // if node has more levels(subdirs)
    else if (level < nodeNameLevels - 1) {
        if (childPos != CHILD_NOT_FOUND)
            return addTreeNode(parent->child(childPos), node, level + 1);
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(NAME_COLUMN, nodeSubName);
        return addTreeNode(item, node, level + 1);
    }
    return false;
}

int ClientWidget::getTreeChild(QTreeWidgetItem *parent, QString nodeName)
{
    for (int i = 0; i < parent->childCount(); i++) {
        if (parent->child(i)->text(NAME_COLUMN) == nodeName)
            return i;
    }
    return -1;
}

//void ClientWidget::updateTreeWidget(const QStringList &list)
//{
////    QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_ui->treeWidget);
////    treeItem->setText(0, "Name");
////    treeItem->setText(1, "Size");
////    treeItem->setText(2, "Status");
//}
