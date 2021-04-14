#include "clientwidget.h"
#include "ui_clientwidget.h"

ClientWidget::ClientWidget(QWidget *parent)
    : QWidget(parent)
    , m_ui(new Ui::ClientWidget)
{
    m_ui->setupUi(this);
    connect(&m_client, &ClientHandler::received,
            this, &ClientWidget::receivedFromClient);
}

ClientWidget::~ClientWidget()
{
    delete m_ui;
}


void ClientWidget::on_sendBtn_clicked()
{
    m_client.start();
}

void ClientWidget::receivedFromClient(QString data)
{
    m_ui->textReceived->setText(data);
}
