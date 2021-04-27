#ifndef FILEINFO_H
#define FILEINFO_H

#include <QString>
#include <QDataStream>
#include <QDateTime>

class FileInfo
{
public:
    FileInfo();
    FileInfo(QString name, qint64 size, QDateTime date);
    FileInfo(QString name, qint64 size);
    QString toStr() const;

    static void getFilesList(QList<FileInfo> *fileList,
                             QString path, QString root);

    static QString string(QList<FileInfo> *fileList);

    static QString getNameFromQList(QStringList &separatedName);

    friend QDataStream& operator<<(QDataStream& stream, const FileInfo &fileInfo);
    friend QDataStream& operator>>(QDataStream& stream, FileInfo &fileInfo);

    friend bool operator==(const FileInfo& left, const FileInfo& right);

    bool isUpToDate(const FileInfo &compared) const;

    QString getName() const;
    void setName(const QString &value);

    qint64 getSize() const;
    void setSize(qint64 value);

    QDateTime getDate() const;
    void setDate(const QDateTime &value);

private:
    QString name;
    qint64 size;
    QDateTime date;
};

#endif // FILEINFO_H
