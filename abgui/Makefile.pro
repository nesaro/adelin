LIBS += -labackup -lssl 
INCLUDEPATH += /usr/share/adelinbackup/include/  include/ common/
#INCLUDEPATH += /usr/share/adelinbackup/include/boxbackup-0.11rc2/lib/common/ 
#INCLUDEPATH += /usr/share/adelinbackup/include/boxbackup-0.11rc2/lib/backupclient 
#INCLUDEPATH += /usr/share/adelinbackup/include/boxbackup-0.11rc2/lib/server
QMAKE_LIBDIR = bin
QMAKE_CXXFLAGS_DEBUG += -DQTTEST_DEBUG
DEFINES = MYFLAGS
TEMPLATE = app
FORMS +=  MainWindow.ui ConfigurationWindow.ui FileManagementWidget.ui FileManagementWindow.ui FileRestoreWidget.ui FMWLocationInfoPanel.ui FMWLostLocationWindow.ui FRWFileRestorePanel.ui StatusWidget.ui

HEADERS = BBBInterfaceSocket.h BBSInterface.h BBSInterfacePoolThread.h BBSNode.h \
BOSException.h ConfigurationWindow.h \
FileManagementWidget.h FileManagementWindow.h FileRestoreWidget.h FMWLocationInfoPanel.h FMWLostLocationWindow.h \
FRWFileRestorePanel.h FRWLocationItem.h FRWLocationModel.h MainWindow.h \
 StatusWidget.h UserInterface.h 

SOURCES += main.cpp BBBInterfaceSocket.cpp BBSInterface.cpp BBSInterfacePoolThread.cpp BBSNode.cpp \
BOSException.cpp \
ConfigurationWindow.cpp FileManagementWidget.cpp FileManagementWindow.cpp FileRestoreWidget.cpp FMWLocationInfoPanel.cpp\
FMWLostLocationWindow.cpp FRWFileRestorePanel.cpp FRWLocationItem.cpp FRWLocationModel.cpp \
MainWindow.cpp StatusWidget.cpp UserInterface.cpp 

target.path += $${DESTDIR}/usr/bin/
INSTALLS += target

CONFIG += thread
QT += network
DESTDIR = ""
TARGET = abgui

win32{
DEFINES += WIN32
DEFINES -= HAVE_NETINET_IN_H
LIBS +=  -lcrypto -lws2_32 -lpcre
CONFIG += qt windows
}
unix{
CONFIG += debug warn_on rtti
}

TRANSLATIONS   = adelinbackup_es.ts
RESOURCES = adelinbackup.qrc

#include(modeltest.pri)
