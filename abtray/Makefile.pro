QMAKE_LIBDIR = bin
QMAKE_CXXFLAGS_DEBUG += -DQTTEST_DEBUG
TEMPLATE = app
FORMS += LogWindow.ui InstanceSelectorWindow.ui #SplashWindow.ui
INCLUDEPATH += common/
INCLUDEPATH += /usr/share/adelinbackup/include/ include
#INCLUDEPATH += include/boxbackup-0.11rc2/lib/common/
LIBS += -labackup

QT+=network xml

HEADERS +=  Event.h EventHandler.h BBBInterfaceSystem.h TrayIconHandler.h LogWindow.h ABBSDClientListener.h BBInterfaceStatus.h BBLogHandler.h EventStore.h common/abbsd.h ProcessControl.h BBInstance.h BBRunningInstanceList.h ABBSDStatus.h InstanceSelectorWindow.h SingleApplication.h ServiceRegistrationHelper.h 
 
SOURCES += main.cpp  TrayIconHandler.cpp ABBSDClientListener.cpp Event.cpp EventHandler.cpp BBLogHandler.cpp BBBInterfaceSystem.cpp  BBInterfaceStatus.cpp LogWindow.cpp EventStore.cpp ProcessControl.cpp BBInstance.cpp BBRunningInstanceList.cpp ABBSDStatus.cpp InstanceSelectorWindow.cpp SingleApplication.cpp ServiceRegistrationHelper.cpp 


CONFIG += thread 
DESTDIR = ""
TARGET = abtray
target.path += $${DESTDIR}/usr/bin/
INSTALLS += target
RESOURCES = abbsdserver.qrc

win32{
DEFINES += WIN32
CONFIG += qt windows
LIBS += -lpcre -lpsapi
INCLUDEPATH += libabackup
}
unix{
CONFIG += debug warn_on rtti
}


