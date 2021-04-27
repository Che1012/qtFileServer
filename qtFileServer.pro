TEMPLATE = subdirs

CONFIG += testcase

SUBDIRS = client    \
          server    \
          lib       \
          test      \

client.subdir = fileclient
server.subdir = fileserver
lib.subdir = libqtfileserver
test.subdir = test

client.depends = lib
server.depends = lib

test.depends = lib client server

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
