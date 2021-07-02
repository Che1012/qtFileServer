#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "serverwidget.h"
#include "clientwidget.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_actionSwitch_to_server_triggered();

    void on_actionSwitch_to_client_triggered();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
