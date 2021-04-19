#include <QDir>
#include <QDebug>

#include "fileinfo.h"


FileInfo::FileInfo()
{

}

// creates string with all files presented
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
QString FileInfo::getAllFiles(QString path, QString root)
{
    QDir workingDir(path);
    workingDir.setFilter(QDir::Files | QDir::NoSymLinks |
                         QDir::Dirs  | QDir::NoDotAndDotDot);
    QString res;
    QFileInfoList list = workingDir.entryInfoList();
    qDebug() << "files size" << list.size();
    for (int i = 0; i < list.size(); i++) {
        if (list.at(i).isDir())
            res += getAllFiles(path + "/" + list.at(i).fileName(),
                               list.at(i).fileName() + "/");
        else
            res += root + list.at(i).fileName() + "\n";
    }
    return res;
}
