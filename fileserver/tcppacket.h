#ifndef TCPPACKET_H
#define TCPPACKET_H

#include <QList>
#include <QIODevice>

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

bool recieveFileList(QIODevice *dev, QList<FileInfo> *list);
bool recieveString(QIODevice *dev, QString *str);
bool recieveFile(QIODevice *dev);
bool recieveFileInfo(QIODevice *dev, FileInfo *info);
}


#endif // TCPPACKET_H
