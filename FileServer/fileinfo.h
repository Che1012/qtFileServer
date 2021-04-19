#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>

class FileInfo
{
public:
    FileInfo();
    static QString getAllFiles(QString path, QString root);

};

#endif // FILEINFO_H
