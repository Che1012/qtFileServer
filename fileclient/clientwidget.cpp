#include <QTreeWidget>
#include <QDir>


#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ClientWidget)
{
    m_ui->setupUi(this);
    connect(&m_client, &ClientHandler::received,
            this,      &ClientWidget::receivedFromClient);

    connect(this,      &ClientWidget::sendFile,
            &m_client, &ClientHandler::sendFileReq);

    connect(&m_client, &ClientHandler::filesReceived,
            this,      &ClientWidget::updateTreeWidget);

    connect(this,      &ClientWidget::sendFilesList,
            &m_client, &ClientHandler::sendFileListReq);

    connect(this,      &ClientWidget::sendData,
            &m_client, &ClientHandler::sendEcho);
}

ClientWidget::~ClientWidget()
{
    delete m_ui;
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
    qDebug() << "updating treeWidget";
    if (currFileInfoList != nullptr)
        delete currFileInfoList;
    currFileInfoList = new QList<FileInfo>();
    FileInfo::getFilesList(currFileInfoList, QDir::currentPath(), "");

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

// returns true if file is up to date
bool ClientWidget::checkTreeNode(QList<FileInfo> *prevList, FileInfo &node)
{
    if (prevList == nullptr)
        return false;
    for (FileInfo temp : *prevList)
        if (temp == node) {
            if (temp.isUpToDate(node))
                return true;
            else
                return false;
        }
    return false;
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
        item->setText(Column::Name, nodeSubName);
        item->setText(Column::Size, QString::number(node.getSize()));
        item->setText(Column::Date, node.getDate().toString());
        if (checkTreeNode(currFileInfoList, node))
            item->setText(Column::Status, "Up to date");
        else
            item->setText(Column::Status, "Need update");
        return true;
    }
    // if node has more levels(subdirs)
    else if (level < nodeNameLevels - 1) {
        if (childPos != CHILD_NOT_FOUND)
            return addTreeNode(parent->child(childPos), node, level + 1);
        QTreeWidgetItem* item = new QTreeWidgetItem(parent);
        item->setText(Column::Name, nodeSubName);
        return addTreeNode(item, node, level + 1);
    }
    return false;
}

int ClientWidget::getTreeChild(QTreeWidgetItem *parent, QString nodeName)
{
    for (int i = 0; i < parent->childCount(); i++) {
        if (parent->child(i)->text(Column::Name) == nodeName)
            return i;
    }
    return -1;
}

void ClientWidget::on_fileBtn_clicked()
{
    emit sendFile(m_ui->fileLineEdit->text());
}

void ClientWidget::on_syncBtn_clicked()
{
    emit sendFilesList();
}

void ClientWidget::on_echoBtn_clicked()
{
    emit sendData(m_ui->echoLineEdit->text());
}
