#include "tcppacket.h"

tcp::Command tcp::recieveCmd(QIODevice *dev)
{
    int cmd;
    QDataStream stream(dev);
    stream >> cmd;
    return static_cast<tcp::Command>(cmd);
}

bool tcp::recieveString(QIODevice *dev, QString *str)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    str->fromUtf8(dev->readAll());
    return true;
}

bool tcp::recieveFileList(QIODevice *dev, QList<FileInfo> *list)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream stream(dev);
    stream >> *list;
    return true;
}
