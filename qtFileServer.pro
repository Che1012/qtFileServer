TEMPLATE = subdirs

CONFIG += testcase

SUBDIRS = client_app    \
          client_cmd    \
          lib       \
          test      \

client_app.subdir = client_app
client_cmd.subdir = client_cmd
lib.subdir = libqtfileserver
test.subdir = test

client_app.depends = lib
client_cmd.depends = lib

test.depends = lib client_app client_cmd

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
