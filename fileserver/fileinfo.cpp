#include <QDir>
#include <QDebug>

#include "fileinfo.h"

FileInfo::FileInfo()
    : name(""), size(0), date()
{
}

FileInfo::FileInfo(QString name, qint64 size, QDateTime date)
    : name(name), size(size), date(date)
{
}
FileInfo::FileInfo(QString name, qint64 size)
    : name(name), size(size)
{
}

QString FileInfo::getName() const
{
    return name;
}

void FileInfo::setName(const QString &value)
{
    name = value;
}

qint64 FileInfo::getSize() const
{
    return size;
}

void FileInfo::setSize(qint64 value)
{
    size = value;
}

QDateTime FileInfo::getDate() const
{
    return date;
}

void FileInfo::setDate(const QDateTime &value)
{
    date = value;
}

QDataStream& operator<<(QDataStream& stream, const FileInfo &fileInfo)
{
    stream << fileInfo.name
           << fileInfo.size
           << fileInfo.date;
    return stream;
}
QDataStream& operator>>(QDataStream& stream, FileInfo &fileInfo)
{
    QString name;
    qint64 size;
    QDateTime date;
    stream >> name >> size >> date;
    fileInfo.setName(name);
    fileInfo.setSize(size);
    fileInfo.setDate(date);
    return stream;
}
bool operator==(const FileInfo& left, const FileInfo& right)
{
    if (left.name == right.name &&
        left.size == right.size)
        return true;
    return false;
}
// writes info about all files from the path to the fileList
// !! before calling this function need to create QList<FileInfo> *fileList
// as separator takes QString separator
// QString root used to set root dir to display in result string
//
// example: usr/bin/first
//          usr/bin/sec
//          usr/bin/third
//          usr/bin/test/four
// arg: path = usr/bin root = "" separator = " "
// out: first sec third test/four
//
// arg: path = usr/bin root = "bin/" separator = " "
// out: bin/first bin/sec bin/third test/four
void FileInfo::getFilesList(QList<FileInfo> *fileList, QString path, QString root)
{
    QDir workingDir(path);
    workingDir.setFilter(QDir::Files | QDir::NoSymLinks |
                         QDir::Dirs  | QDir::NoDotAndDotDot);
    QFileInfoList list = workingDir.entryInfoList();
    qDebug() << "amount of files at" << path << list.size();
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).isDir())
            getFilesList(fileList, path + "/" + list. at(i).fileName(),
                        list.at(i).fileName() + "/");
        else
        {
            FileInfo fileInfo(root + list.at(i).fileName(),
                              list.at(i).size(),
                              list.at(i).lastModified());
            fileList->push_back(fileInfo);
        }
    }
}

QString FileInfo::string(QList<FileInfo> *fileList)
{
    QString res;
    for (int i = 0; i < fileList->size(); i++)
        res += fileList->at(i).toStr() + "\n";
    return res;
}

QString FileInfo::toStr() const
{
    return name + " " + QString::number(size) + " " + date.toString();
}

bool FileInfo::isUpToDate(const FileInfo &compared)
{
    if (this->date > compared.date)
        return true;
    return false;
}
