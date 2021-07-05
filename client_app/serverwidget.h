#ifndef SERVERWIDGET_H
#define SERVERWIDGET_H

#include <QWidget>

#include "serverhandler.h"

namespace Ui {
class ServerWidget;
}

class ServerWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ServerWidget(QWidget *parent = nullptr);
    ~ServerWidget();

private slots:
    void on_startBtn_clicked();

    void addConnectionInfo(const QString &info);
    void removeConnectionInfo(const QString &info);

private:
    Ui::ServerWidget *m_ui;
    ServerHandler *m_server;

};

#endif // SERVERWIDGET_H
