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
    updateTreeWidget(QStringList());
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

void ClientWidget::updateTreeWidget(const QStringList &list)
{
//    QTreeWidgetItem *treeItem = new QTreeWidgetItem(m_ui->treeWidget);
//    treeItem->setText(0, "Name");
//    treeItem->setText(1, "Size");
//    treeItem->setText(2, "Status");
}
