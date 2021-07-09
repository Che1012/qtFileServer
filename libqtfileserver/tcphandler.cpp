#include "tcphandler.h"
#include "QDebug"

TCPHandler::TCPHandler(QObject *parent, QString workingDirName)
    : QObject(parent),
      m_workingDirName(workingDirName)
{

}

TCPHandler::TCPCommand TCPHandler::recieveCmd(QIODevice *dev)
{
    int cmd;
    QDataStream stream(dev);
    stream >> cmd;
    return static_cast<TCPCommand>(cmd);
}

TCPHandler::CommandType TCPHandler::takeCmdFromQueue()
{
    if (!m_cmdQueue.isEmpty())
        return m_cmdQueue.takeFirst();
    return TCPHandler::CommandType();
}

bool TCPHandler::startNextCmd()
{
    if (isCmdHandling())
        return false;
    TCPHandler::CommandType cmdType = takeCmdFromQueue();
    if (cmdType.cmd == NotCommand)
        return false;
    handleCmd();
// Add custom behavior like this at inheritors
    switch (cmdType.cmd) {
//    case SendFilesList:
//        Send FileList here
//        break;
//    case Echo:
//        Send EchoPacket here
//        break;
    default:
        break;
    }
    return true;
};

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
    dataStream << static_cast<int>(Echo) << value;
    return true;
}

bool TCPHandler::sendStringPacket(QIODevice *dev, const QString &value)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(StringValue) << value;
    return true;
}

bool TCPHandler::sendFilesListRequest(QIODevice *dev)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(SendFilesList);
    return true;
}

bool TCPHandler::sendFilesList(QIODevice *dev, const QList<FileInfo> *list)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(FilesList) << *list;
    return true;
}

bool TCPHandler::sendFileRequest(QIODevice *dev, const QString& name)
{
    QDataStream dataStream(dev);
    dataStream << static_cast<int>(SendFile) << name;
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
    QFile file(m_workingDirName + "/" + m_fileReceiving->getName());

    if (!file.open(QFile::Append))
        return false;

    QDataStream stream(dev);
    qint64 packetSize;
    QByteArray byteArray;
    stream >> packetSize >> byteArray;
    qDebug() << "bytes available" << dev->bytesAvailable();
    m_remainRFileSize -= byteArray.size();
    qDebug() << "received filePacket";
    qDebug() << "expected" << packetSize << "received" << byteArray.size()
             << "remaining" << m_remainRFileSize;
    file.write(byteArray);
    emit filePacketReceived(m_fileReceiving->getSize(),
                            m_fileReceiving->getSize() - m_remainRFileSize);
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
    dataStream << static_cast<int>(StartFilePacket)
                << fileShortName
                << file->size();
    m_remainTFileSize = file->size();
    while (!file->atEnd()) {
        qint64 size = m_remainTFileSize < m_payLoadSize ? m_remainTFileSize : m_payLoadSize;
        m_remainTFileSize -= size;
        dataStream << FilePacket << size << file->read(size);
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
    m_fileReceiving = new FileInfo(fileName, fileSize);
    m_remainRFileSize = fileSize;

    qDebug() << "recieveFileInfo:" << fileName << fileSize;
    return true;
}
