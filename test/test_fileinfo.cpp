#include <QtTest>

#include "fileinfo.h"

class Test_FileInfo : public QObject
{
    Q_OBJECT

public:
    Test_FileInfo() {};
    ~Test_FileInfo() {};

private slots:
    void isUpToDate();

};


void Test_FileInfo::isUpToDate()
{
    FileInfo fileInfo("test", 1024, QDateTime(QDate(2005, 11, 15), QTime(12, 5, 10)));

    FileInfo laterMonth("test", 1024, QDateTime(QDate(2005, 12, 15), QTime(12, 5, 10)));
    FileInfo laterDay("test", 1024, QDateTime(QDate(2005, 11, 18), QTime(12, 5, 10)));
    FileInfo laterTime("test", 1024, QDateTime(QDate(2005, 11, 15), QTime(15, 6, 7)));

    FileInfo earlierMonth("test", 1024, QDateTime(QDate(2005, 5, 15), QTime(12, 5, 10)));
    FileInfo earlierDay("test", 1024, QDateTime(QDate(2005, 11, 13), QTime(12, 5, 10)));
    FileInfo earlierTime("test", 1024, QDateTime(QDate(2005, 11, 15), QTime(5, 58, 98)));

    QCOMPARE(fileInfo.isUpToDate(laterMonth), false);
    QCOMPARE(fileInfo.isUpToDate(laterDay), false);
    QCOMPARE(fileInfo.isUpToDate(laterTime), false);

    QCOMPARE(fileInfo.isUpToDate(earlierMonth), true);
    QCOMPARE(fileInfo.isUpToDate(earlierDay), true);
    QCOMPARE(fileInfo.isUpToDate(earlierTime), true);
}

QTEST_APPLESS_MAIN(Test_FileInfo)

#include "test_fileinfo.moc"
