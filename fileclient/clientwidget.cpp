#include <QTreeWidget>
#include <QDir>
#include <QMenu>


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

    m_ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->treeWidget, &QWidget::customContextMenuRequested,
            this,             &ClientWidget::showContextMenu);
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
    /////TO-DO Replace
    m_client.setWorkingDirName(m_ui->dirLineEdit->text());
    /////
    FileInfo::getFilesList(currFileInfoList, m_ui->dirLineEdit->text(), "");

    QTreeWidgetItem* treeRoot = m_ui->treeWidget->takeTopLevelItem(0);
    if (treeRoot != nullptr)
        delete treeRoot;
    treeRoot = new QTreeWidgetItem(m_ui->treeWidget);

    for (int i = 0; i < fileList->size(); i++)
            if (!addTreeNode(treeRoot, fileList->at(i), 0))
                qDebug() << "Error at creating tree";
    treeRoot->setExpanded(true);
    delete fileList;
}

// returns true if file from the list is up to date compare to node
bool ClientWidget::checkTreeNode(QList<FileInfo> *list, const FileInfo &node)
{
    if (list == nullptr)
        return false;
    for (FileInfo temp : *list)
        if (temp == node)
            return temp.isUpToDate(node);
    return false;
}

bool ClientWidget::addTreeNode(QTreeWidgetItem* parent, const FileInfo &node, int level)
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

int ClientWidget::getTreeChild(QTreeWidgetItem *parent, const QString &nodeName)
{
    for (int i = 0; i < parent->childCount(); i++) {
        if (parent->child(i)->text(Column::Name) == nodeName)
            return i;
    }
    return -1;
}

void ClientWidget::updateTreeNode(QTreeWidgetItem *node)
{
    QStringList fileNameList;
    while (node->parent() != nullptr) {
        fileNameList.push_back(node->text(Column::Name));
        node = node->parent();
    }
    QString fileName = FileInfo::getNameFromQList(fileNameList);
    qDebug() << "updating tree node" << fileName;
    emit sendFile(fileName);
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

void ClientWidget::showContextMenu(const QPoint &pos)
{
    // for most widgets
        QPoint globalPos = m_ui->treeWidget->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        QMenu myMenu;
        myMenu.addAction("Update");

        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            if (selectedItem->text() == "Update") {
                qDebug() << "selected tree widget item"
                         << m_ui->treeWidget->currentItem()->text(Column::Name);
                updateTreeNode(m_ui->treeWidget->currentItem());
            }
        }
        else {
            // nothing was chosen
        }
}
