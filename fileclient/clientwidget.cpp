#include <QTreeWidget>
#include <QDir>
#include <QMenu>
#include <QSettings>


#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ClientWidget)
{
    m_ui->setupUi(this);

    connect(this,      &ClientWidget::sendFile,
            &m_client, &ClientHandler::sendFileReq);

    connect(&m_client, &ClientHandler::filesReceived,
            this,      &ClientWidget::updateTreeWidget);

    connect(this,      &ClientWidget::sendFilesList,
            &m_client, &ClientHandler::sendFileListReq);

    connect(this,      &ClientWidget::sendData,
            &m_client, &ClientHandler::sendEcho);

    connect(&m_client, &ClientHandler::filePacketReceived,
            this,      &ClientWidget::recievedfilePacket);

    m_ui->treeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_ui->treeWidget, &QWidget::customContextMenuRequested,
            this,             &ClientWidget::showContextMenu);
    loadSettings();
}

ClientWidget::~ClientWidget()
{
    saveSettings();
    delete m_ui;
}

void ClientWidget::saveSettings()
{
    QSettings settings("OpenSoft", "QtFileServer");

    settings.setValue("client/ipLineEdit", m_ui->ipLineEdit->text());
    settings.setValue("client/portLineEdit", m_ui->portLineEdit->text());
    settings.setValue("client/dirLineEdit", m_ui->dirLineEdit->text());
}

void ClientWidget::loadSettings()
{
    QSettings settings("OpenSoft", "QtFileServer");

    m_ui->ipLineEdit->setText(settings.value("client/ipLineEdit").toString());
    m_ui->portLineEdit->setText(settings.value("client/portLineEdit").toString());
    m_ui->dirLineEdit->setText(settings.value("client/dirLineEdit").toString());
}

void ClientWidget::recievedfilePacket(qint64 fileSize, qint64 recievedDataSize)
{
    float progress = static_cast<float>(recievedDataSize) /
                     static_cast<float>(fileSize) * 100;
    m_ui->progressBar->setValue(static_cast<int>(progress));
}

void ClientWidget::on_connectBtn_clicked()
{
    m_client.start(m_ui->ipLineEdit->text(), m_ui->portLineEdit->text().toInt());
}

void ClientWidget::updateTreeWidget(QList<FileInfo> *fileList)
{
    qDebug() << "updating treeWidget";
    if (currFileInfoList != nullptr)
        delete currFileInfoList;
    currFileInfoList = fileList;
    /////TO-DO Replace
    m_client.setWorkingDirName(m_ui->dirLineEdit->text());
    /////

    if (filesAtClientList != nullptr)
        delete filesAtClientList;
    filesAtClientList = new QList<FileInfo>;
    FileInfo::getFilesList(filesAtClientList, m_ui->dirLineEdit->text(), "");

    QTreeWidgetItem* treeRoot = m_ui->treeWidget->takeTopLevelItem(0);
    if (treeRoot != nullptr)
        delete treeRoot;
    treeRoot = new QTreeWidgetItem(m_ui->treeWidget);

    for (int i = 0; i < currFileInfoList->size(); i++)
            if (!addTreeNode(treeRoot, currFileInfoList->at(i), 0))
                qDebug() << "Error at creating tree";
    treeRoot->setExpanded(true);
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

bool ClientWidget::addTreeNode(QTreeWidgetItem* parent,
                               const FileInfo &node,
                               int level)
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
        if (checkTreeNode(filesAtClientList, node))
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

void ClientWidget::updateTreeNode(const QString &nodeName)
{
    qDebug() << "updating tree node" << nodeName;
    emit sendFile(nodeName);
}

void ClientWidget::on_syncBtn_clicked()
{
    emit sendFilesList();
}

void ClientWidget::showContextMenu(const QPoint &pos)
{
    // for most widgets
        QPoint globalPos = m_ui->treeWidget->mapToGlobal(pos);
        // for QAbstractScrollArea and derived classes you would use:
        // QPoint globalPos = myWidget->viewport()->mapToGlobal(pos);

        QMenu myMenu;
        if (m_ui->treeWidget->itemAt(pos) == nullptr)
            return;
        myMenu.addAction("Update");

        QAction* selectedItem = myMenu.exec(globalPos);
        if (selectedItem) {
            if (selectedItem->text() == "Update") {
                qDebug() << "selected tree widget item"
                         << m_ui->treeWidget->currentItem()->text(Column::Name);
                updateTreeNode(m_ui->treeWidget->currentItem());
            }
            emit sendFilesList();
        }
        else {
            // nothing was chosen
        }
}

void ClientWidget::on_updateFilesBtn_clicked()
{
    if (!currFileInfoList || !filesAtClientList)
        return;
    for (int i = 0; i < currFileInfoList->size(); i++) {
        if (!checkTreeNode(filesAtClientList, currFileInfoList->at(i)))
            updateTreeNode(currFileInfoList->at(i).getName());
    }
    emit sendFilesList();
}

void ClientWidget::connected()
{

}

void ClientWidget::disconnected()
{

}
