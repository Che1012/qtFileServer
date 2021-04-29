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

bool TCPHandler::receiveFile(QIODevice *dev)
{
    // TO-DO rework this terrible file receiving
    QFile file(workingDirName + "/" + fileReceiving->getName());

    if (!file.open(QFile::Append))
        return false;

    QDataStream stream(dev);
    qint64 packetSize;
    QByteArray byteArray;
    stream >> packetSize >> byteArray;
    qDebug() << "bytes available" << dev->bytesAvailable();
    remainRFileSize -= byteArray.size();
    qDebug() << "received filePacket";
    qDebug() << "expected" << packetSize << "received" << byteArray.size()
             << "remaining" << remainRFileSize;
    file.write(byteArray);
    emit filePacketReceived(fileReceiving->getSize(),
                            fileReceiving->getSize() - remainRFileSize);

    if (dev->bytesAvailable() > remainRFileSize)
    {
        tcp::Command cmd;
        while (dev->bytesAvailable() > 0) {
            stream >> cmd >> packetSize >> byteArray;
            remainRFileSize -= byteArray.size();
            qDebug() << "bytes available" << dev->bytesAvailable();
            qDebug() << "received filePacket";
            qDebug() << "expected" << packetSize << "received" << byteArray.size()
                     << "remaining" << remainRFileSize;
            file.write(byteArray);
            emit filePacketReceived(fileReceiving->getSize(),
                                    fileReceiving->getSize() - remainRFileSize);
        }
    }
    file.close();
    return true;
}

bool TCPHandler::sendFile(QIODevice *dev, QFile *file, QString &fileShortName)
{
    if (!file->open(QFile::ReadOnly)) {
        qDebug() << "Can not open file to read";
        return false;
    }
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(tcp::StartFilePacket)
                << fileShortName
                << file->size();
    remainTFileSize = file->size();
    while (!file->atEnd()) {
        qint64 size = remainTFileSize < payLoadSize ? remainTFileSize : payLoadSize;
        remainTFileSize -= size;
        dataStream << tcp::FilePacket << size << file->read(size);
        qDebug() << "sended file packet" << size;
    }
    qDebug() << "File" << file->fileName() << "sended" << file->size();
    return true;
}

bool TCPHandler::recieveFileInfo(QIODevice *dev)
{    
    if (dev == nullptr || !dev->isOpen() || !dev->isReadable())
        return false;
    QString fileName;
    qint64 fileSize;
    QDataStream dataStream(dev);
    dataStream >> fileName >> fileSize;
    fileReceiving = new FileInfo(fileName, fileSize);
    remainRFileSize = fileSize;

    qDebug() << "recieveFileInfo:" << fileName << fileSize;
    return true;
}
