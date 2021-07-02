QT += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TEMPLATE = app

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
    main.cpp \
    clientwidget.cpp \
    mainwindow.cpp \
    serverwidget.cpp

HEADERS += \
    clienthandler.h \
    clientwidget.h \
    mainwindow.h \
    serverwidget.h

FORMS += \
    clientwidget.ui \
    mainwindow.ui \
    serverwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

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
