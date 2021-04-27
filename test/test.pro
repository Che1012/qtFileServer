QT += testlib
QT += gui
CONFIG += qt warn_on depend_includepath testcase

TEMPLATE = app

SOURCES +=  \
    test_fileinfo.cpp

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libqtfileserver/release/ -llibqtfileserver
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libqtfileserver/debug/ -llibqtfileserver
else:unix: LIBS += -L$$OUT_PWD/../libqtfileserver/ -llibqtfileserver

INCLUDEPATH += $$PWD/../libqtfileserver
DEPENDPATH += $$PWD/../libqtfileserver

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libqtfileserver/release/liblibqtfileserver.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libqtfileserver/debug/liblibqtfileserver.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libqtfileserver/release/libqtfileserver.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libqtfileserver/debug/libqtfileserver.lib
else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libqtfileserver/liblibqtfileserver.a
