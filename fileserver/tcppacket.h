#ifndef TCPPACKET_H
#define TCPPACKET_H

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

Command recieveCmd(QIODevice *dev);

bool recieveFilesList(QIODevice *dev, QList<FileInfo> *list);
bool recieveString(QIODevice *dev, QString *str);
bool recieveFileInfo(QIODevice *dev, QString *fileName, quint64 *fileSize);
bool recieveFileName(QIODevice *dev, QString *name);

bool sendEchoPacket(QIODevice *dev, QString *value);
bool sendStringPacket(QIODevice *dev, QString *value);
bool sendFilesListRequest(QIODevice *dev);
bool sendFilesList(QIODevice *dev, QList<FileInfo> *list);
bool sendFileRequest(QIODevice *dev, QString* name);
bool sendFile(QIODevice *dev, QFile* file, qint64 payLoad);

}


#endif // TCPPACKET_H
