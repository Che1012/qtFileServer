#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QList>
#include <QIODevice>
#include <QFile>
#include <QDir>

#include "fileinfo.h"

namespace tcp {

enum Command {
    NotCommand = 0,
    SendFilesList = 208,
    Echo,
    SendFile,

    FilesList,
    StringValue,
    StartFilePacket,
    FilePacket
};
struct CommandType
{
    CommandType(Command cmd = NotCommand, const QString& value = "")
        :cmd(cmd), value(value) {};
    Command cmd;
    QString value;
};
}

class TCPHandler : public QObject
{
Q_OBJECT

signals:
     void filePacketReceived(qint64 fileSize, qint64 recievedDataSize);
public:
    QString getWorkingDirName() const {return  workingDirName;};
    void    setWorkingDirName(const QString &value) {workingDirName = value;};
protected:
    TCPHandler(QObject *parent = nullptr,
               QString workingDirName = QDir::currentPath())
        : QObject(parent),
          workingDirName(workingDirName) {};
    tcp::Command recieveCmd(QIODevice *dev);

    bool recieveFilesList(QIODevice *dev, QList<FileInfo> *list);
    bool recieveString(QIODevice *dev, QString &str);
    bool recieveFileInfo(QIODevice *dev);
    bool recieveFileName(QIODevice *dev, QString &name);
    bool receiveFile(QIODevice *dev);

    bool sendEchoPacket(QIODevice *dev, const QString &value);
    bool sendStringPacket(QIODevice *dev, const QString &value);
    bool sendFilesListRequest(QIODevice *dev);
    bool sendFilesList(QIODevice *dev, const QList<FileInfo> *list);
    bool sendFileRequest(QIODevice *dev, const QString &name);
    bool sendFile(QIODevice *dev, QFile *file, QString &fileShortName);

    bool isCmdHandling() {return commandHandling;};
    void handleCmd() {commandHandling = true;};
    void unhandleCmd() {commandHandling = false;};

    tcp::CommandType takeCmdFromQueue()
    {
        if (!cmdQueue.isEmpty())
            return cmdQueue.takeFirst();
        return tcp::CommandType();
    };
    void putCmdToQueue(tcp::CommandType cmd) {cmdQueue.push_back(cmd);};

    virtual bool startNextCmd() {return false;};
protected:
    FileInfo *fileReceiving = nullptr;
    qint64    remainRFileSize;
    qint64    remainTFileSize;
    int       payLoadSize = 64 * 1024; //64Kb

private:
    QString   workingDirName;

    QList<tcp::CommandType> cmdQueue;
    bool commandHandling = false;
};

#endif // TCPHANDLER_H
