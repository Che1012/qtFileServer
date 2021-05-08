#ifndef TCPHANDLER_H
#define TCPHANDLER_H

#include <QList>
#include <QIODevice>
#include <QFile>
#include <QDir>

#include "fileinfo.h"

class TCPHandler : public QObject
{
Q_OBJECT

signals:
     void filePacketReceived(qint64 fileSize, qint64 recievedDataSize);

public:
     enum TCPCommand {
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
         CommandType(TCPCommand cmd = NotCommand, const QString& value = "")
             :cmd(cmd), value(value) {};
         TCPCommand cmd;
         QString value;
     };

    TCPHandler(QObject *parent = nullptr,
               QString workingDirName = QDir::currentPath());

    QString getWorkingDirName() const { return  workingDirName; };
    void    setWorkingDirName(const QString &value) { workingDirName = value; };

protected:
    FileInfo *fileReceiving = nullptr;
    qint64    remainRFileSize;
    qint64    remainTFileSize;
    int       payLoadSize = 64 * 1024; //64Kb

    // API for sending/recieving data through tcp
    //
    // Packet type
    TCPCommand recieveCmd(QIODevice *dev);
    // String
    bool sendStringPacket(QIODevice *dev, const QString &value);
    bool recieveString(QIODevice *dev, QString &str);
    // Echo request
    bool sendEchoPacket(QIODevice *dev, const QString &value);
    // FileList at working directory
    bool sendFilesListRequest(QIODevice *dev);
    bool sendFilesList(QIODevice *dev, const QList<FileInfo> *list);
    bool recieveFilesList(QIODevice *dev, QList<FileInfo> *list);
    // File info
    bool sendFileRequest(QIODevice *dev, const QString &name);
    bool recieveFileInfo(QIODevice *dev);
    bool recieveFileName(QIODevice *dev, QString &name);
    // File data
    bool sendFile(QIODevice *dev, QFile *file, QString &fileShortName);
    bool receiveFile(QIODevice *dev);

    // Commands are stored at queue and performed one command at time
    // to lock command queue is using latch
    CommandType takeCmdFromQueue();
    void putCmdToQueue(CommandType cmd) { cmdQueue.push_back(cmd); };
    // Latch for command queue
    bool isCmdHandling() { return commandHandling; };
    void handleCmd() { commandHandling = true; };
    void unhandleCmd() { commandHandling = false; };
    // Can be overrided to add custom behavior
    virtual bool startNextCmd();

private:
    QString workingDirName;

    QList<CommandType> cmdQueue;
    bool commandHandling = false;
};

#endif // TCPHANDLER_H
