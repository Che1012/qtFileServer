#include <QCoreApplication>
#include <QObject>
#include <QTimer>

#include "serverhandler.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QString dirPath;
    for(int i = 0; i < argc; i++)
        qDebug() << QString(argv[i]);
    if (argc == 2)
        dirPath = QString(argv[1]);
    else
        dirPath = QDir::currentPath();

    ServerHandler *netHandler = new ServerHandler(&a, dirPath);
    netHandler->init();
    QObject::connect(netHandler, &ServerHandler::finished,
                     &a, &QCoreApplication::quit);


    QTimer::singleShot(0, netHandler, SLOT(start()));
    return a.exec();
}
