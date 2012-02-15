QMAKE_LIBDIR = bin
QMAKE_CXXFLAGS_DEBUG += -DQTTEST_DEBUG
TEMPLATE = app

QT+=network

HEADERS = ABBSDClientConnectionHandler.h common/abbsd.h

SOURCES += main.cpp ABBSDClientConnectionHandler.cpp

target.path += $${DESTDIR}/usr/bin/
INSTALLS += target
INCLUDEPATH += common

CONFIG += thread 
DESTDIR = ""
TARGET = abtraynotify

win32{
DEFINES += WIN32
CONFIG += qt windows
}
unix{
CONFIG += debug warn_on rtti
}

