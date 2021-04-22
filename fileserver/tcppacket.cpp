#include "tcppacket.h"
#include "QDebug"

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
    QDataStream dataStream(dev);
    dataStream >> *str;
    qDebug() << "recieveString:" << *str;
    return true;
}

bool tcp::recieveFilesList(QIODevice *dev, QList<FileInfo> *list)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream stream(dev);
    stream >> *list;
    qDebug() << "recieveFileList: list size =" << list->size();
    return true;
}

bool tcp::sendEchoPacket(QIODevice *dev, QString *value)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::Echo) << *value;
    return true;
}

bool tcp::sendStringPacket(QIODevice *dev, QString *value)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::StringValue) << *value;
    return true;
}

bool tcp::sendFilesListRequest(QIODevice *dev)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::SendFilesList);
    return true;
}

bool tcp::sendFilesList(QIODevice *dev, QList<FileInfo> *list)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::FilesList) << *list;
    return true;
}

bool tcp::sendFileRequest(QIODevice *dev, QString* name)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::SendFile) << *name;
    return true;
}

bool tcp::recieveFileName(QIODevice *dev, QString *name)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream stream(dev);
    stream >> *name;
    qDebug() << "recieveFileName:" << *name;
    return true;
}

bool tcp::sendFile(QIODevice *dev, QFile *file, qint64 payLoad)
{
    if (!file->open(QFile::ReadOnly))
        return false;
    QDataStream *dataStream = new QDataStream(dev);
    *dataStream << static_cast<int>(tcp::StartFilePacket)
                << file->fileName()
                << file->size();
    delete dataStream;
    while (!file->atEnd())
        dev->write(file->read(payLoad));
    return true;
}

bool tcp::recieveFileInfo(QIODevice *dev, QString *fileName, quint64 *fileSize)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream dataStream(dev);
    dataStream >> *fileName >> *fileSize;
    qDebug() << "recieveFileInfo:" << *fileName << *fileSize;
    return true;
}
