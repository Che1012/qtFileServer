#include "tcphandler.h"
#include "QDebug"

tcp::Command TCPHandler::recieveCmd(QIODevice *dev)
{
    int cmd;
    QDataStream stream(dev);
    stream >> cmd;
    return static_cast<tcp::Command>(cmd);
}

bool TCPHandler::recieveString(QIODevice *dev, QString &str)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream dataStream(dev);
    dataStream >> str;
    qDebug() << "recieveString:" << str;
    return true;
}

bool TCPHandler::recieveFilesList(QIODevice *dev, QList<FileInfo> *list)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream stream(dev);
    stream >> *list;
    qDebug() << "recieveFileList: list size =" << list->size();
    return true;
}

bool TCPHandler::sendEchoPacket(QIODevice *dev, const QString &value)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::Echo) << value;
    return true;
}

bool TCPHandler::sendStringPacket(QIODevice *dev, const QString &value)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::StringValue) << value;
    return true;
}

bool TCPHandler::sendFilesListRequest(QIODevice *dev)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::SendFilesList);
    return true;
}

bool TCPHandler::sendFilesList(QIODevice *dev, const QList<FileInfo> *list)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::FilesList) << *list;
    return true;
}

bool TCPHandler::sendFileRequest(QIODevice *dev, const QString& name)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::SendFile) << name;
    return true;
}

bool TCPHandler::recieveFileName(QIODevice *dev, QString &name)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream stream(dev);
    stream >> name;
    qDebug() << "recieveFileName:" << name;
    return true;
}

bool TCPHandler::sendFile(QIODevice *dev, QFile *file,
                          QString &fileShortName,
                          qint64 payLoadSize)
{
    if (!file->open(QFile::ReadOnly)) {
        qDebug() << "Can not open file to read";
        return false;
    }
    QDataStream *dataStream = new QDataStream(dev);
    *dataStream << static_cast<int>(tcp::StartFilePacket)
                << fileShortName
                << file->size();
    delete dataStream;
    while (!file->atEnd())
        dev->write(file->read(file->size() < payLoadSize ? file->size() : payLoadSize));
    qDebug() << "File" << file->fileName() << "sended" << file->size();
    return true;
}

bool TCPHandler::recieveFileInfo(QIODevice *dev,
                                 QString &fileName,
                                 quint64 &fileSize)
{
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QDataStream dataStream(dev);
    dataStream >> fileName >> fileSize;
    qDebug() << "recieveFileInfo:" << fileName << fileSize;
    return true;
}
