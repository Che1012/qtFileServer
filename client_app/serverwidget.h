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

    void saveSettings();
    void loadSettings();

    void on_pathLine_editingFinished();

    void on_browseBtn_clicked();

private:
    Ui::ServerWidget *m_ui;
    ServerHandler *m_server = nullptr;

};

#endif // SERVERWIDGET_H
