#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QList>
#include <QIODevice>
#include <QFile>

#include "fileinfo.h"

namespace tcp {

enum Command {
    SendFilesList = 208,
    Echo,
    SendFile,

    FilesList,
    StringValue,
    StartFilePacket,
    FilePacket
};
}

class TCPHandler : public QObject
{
Q_OBJECT

public:
    TCPHandler(QObject *parent = nullptr) : QObject(parent){};

tcp::Command recieveCmd(QIODevice *dev);

bool recieveFilesList(QIODevice *dev, QList<FileInfo> *list);
bool recieveString(QIODevice *dev, QString &str);
bool recieveFileInfo(QIODevice *dev, QString &fileName, quint64 &fileSize);
bool recieveFileName(QIODevice *dev, QString &name);

bool sendEchoPacket(QIODevice *dev, const QString &value);
bool sendStringPacket(QIODevice *dev, const QString &value);
bool sendFilesListRequest(QIODevice *dev);
bool sendFilesList(QIODevice *dev, const QList<FileInfo> *list);
bool sendFileRequest(QIODevice *dev, const QString &name);
bool sendFile(QIODevice *dev, QFile *file, const qint64 payLoadSize);
};

#endif // TCPHANDLER_H
