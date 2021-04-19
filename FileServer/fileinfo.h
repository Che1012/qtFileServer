#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>
#include <QDataStream>
#include <QDateTime>

struct FileInfo
{
public:
    FileInfo();
    FileInfo(QString name, int size, QDateTime date);

    static void getFilesList(QList<FileInfo> *fileList,
                             QString path, QString root);

    QString toStr();

    friend QDataStream& operator<<(QDataStream& stream, const FileInfo &fileInfo);
    friend QDataStream& operator>>(QDataStream& stream, FileInfo &fileInfo);

    QString getName() const;
    void setName(const QString &value);

    int getSize() const;
    void setSize(int value);

    QDateTime getDate() const;
    void setDate(const QDateTime &value);

private:
    QString name;
    int     size;
    QDateTime date;
};

#endif // FILEINFO_H
