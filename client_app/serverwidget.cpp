#include <QDebug>
#include <QSettings>
#include <QFileDialog>

#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    m_ui(new Ui::ServerWidget)
{
    m_ui->setupUi(this);
    loadSettings();
    qDebug() << "server widget created";
}

ServerWidget::~ServerWidget()
{
    saveSettings();
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

void ServerWidget::saveSettings()
{
    QSettings settings("OpenSoft", "QtFileServer");

    settings.setValue("server/pathLine", m_ui->pathLine->text());
}

void ServerWidget::loadSettings()
{
    QSettings settings("OpenSoft", "QtFileServer");

    m_ui->pathLine->setText(settings.value("server/pathLine").toString());
}

void ServerWidget::on_pathLine_editingFinished()
{
    m_server->setWorkingDirName(m_ui->pathLine->text());
}

void ServerWidget::on_browseBtn_clicked()
{
    QString path = m_ui->pathLine->text();
    if (path.isEmpty() || !QDir(path).exists())
    {
        if (m_server)
            path = m_server->getWorkingDirName();
        else
            path = "";
    }
    QFileDialog dialog(this, tr("Updated directory"), path);
    dialog.setFileMode(QFileDialog::DirectoryOnly);
    QString dirName;
    if (dialog.exec())
        dirName = dialog.selectedFiles()[0];
    m_ui->pathLine->setText(dirName);
}

