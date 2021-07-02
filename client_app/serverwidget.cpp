#include <QDebug>

#include "serverwidget.h"
#include "ui_serverwidget.h"

ServerWidget::ServerWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ServerWidget)
{
    ui->setupUi(this);
    qDebug() << "server widget created";
}

ServerWidget::~ServerWidget()
{
    delete ui;
    qDebug() << "server widget deleted";
}
