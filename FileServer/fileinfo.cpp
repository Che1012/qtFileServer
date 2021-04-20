#include <QDir>
#include <QDebug>

#include "fileinfo.h"

FileInfo::FileInfo()
    : name(""), size(0), date()
{
}

FileInfo::FileInfo(QString name, int size, QDateTime date)
    : name(name), size(size), date(date)
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

int FileInfo::getSize() const
{
    return size;
}

void FileInfo::setSize(int value)
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
}
QDataStream& operator>>(QDataStream& stream, FileInfo &fileInfo)
{
    QString name;
    int     size;
    QDateTime date;
    stream >> name >> size >> date;
    fileInfo.setName(name);
    fileInfo.setSize(size);
    fileInfo.setDate(date);
}
bool operator==(const FileInfo& left, const FileInfo& right)
{
    if (left.date == right.date &&
        left.name == right.name &&
        left.size == right.size)
        return true;
    return false;
}
// writes all files from path to the stream
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
    qDebug() << "files size" << list.size();
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

QString FileInfo::toStr()
{
    return name + " " + QString::number(size) + " " + date.toString();
}
