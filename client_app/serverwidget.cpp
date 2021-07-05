#include <QDebug>

#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ServerWidget)
{
    m_ui->setupUi(this);

    qDebug() << "server widget created";
}

ServerWidget::~ServerWidget()
{
    delete m_ui;
    qDebug() << "server widget deleted";
}

void ServerWidget::on_startBtn_clicked()
{
    QString path = m_ui->pathLine->text();
    if (!QDir(path).exists())
        path = QDir::currentPath();

    m_server = new ServerHandler(this, path);
    m_server->init();
    m_server->start();

    connect(m_server, &ServerHandler::connected,
            this, &ServerWidget::addConnectionInfo);
    connect(m_server, &ServerHandler::disconnected,
            this, &ServerWidget::removeConnectionInfo);

    qDebug() << "server started";
}

void ServerWidget::addConnectionInfo(const QString &info)
{
    new QListWidgetItem(info, m_ui->listWidget);
}

void ServerWidget::removeConnectionInfo(const QString &info)
{
    auto list = m_ui->listWidget->findItems(info, Qt::MatchExactly);

    if (list.isEmpty()) {
        qDebug() << "can not find connection info to remove";
        return;
    }
    m_ui->listWidget->takeItem(m_ui->listWidget->row(list[0]));
}
