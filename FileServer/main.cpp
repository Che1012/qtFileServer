#include <QCoreApplication>
#include <QObject>
#include <QTimer>

#include "serverhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ServerHandler *netHandler = new ServerHandler(&a);
    QObject::connect(netHandler, &ServerHandler::finished,
                     &a,          &QCoreApplication::quit);

    QTimer::singleShot(0, netHandler, SLOT(start()));
    return a.exec();
}
