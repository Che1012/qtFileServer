#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setCentralWidget(new ClientWidget);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionSwitch_to_server_triggered()
{
    this->setCentralWidget(new ServerWidget);
}

void MainWindow::on_actionSwitch_to_client_triggered()
{
    this->setCentralWidget(new ClientWidget);
}
