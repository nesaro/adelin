QMAKE_LIBDIR = bin
QMAKE_CXXFLAGS_DEBUG += -DQTTEST_DEBUG
TEMPLATE = app
FORMS += SplashWindow.ui
FORMS += PIWIntroPage.ui KWIntroPage.ui KWEndPage.ui KWInsertSignedKeysPage.ui CWCommitConfigPage.ui CWConfigureBBPage.ui CWIntroPage.ui CWRequestRegisterPage.ui CWLoginAdelinPage.ui CWSelectDirsPage.ui CWIsNewUserPage.ui CWQueryAllKeysPage.ui CWFinishedRegisteredPage.ui
INCLUDEPATH += /usr/share/adelinbackup/include/ include
LIBS += -labackup

#QT+=network xml


HEADERS += FileUtils.h
SOURCES += main.cpp FileUtils.cpp

HEADERS += SplashWindow.h KeyWizard.h ConfigurationWizard.h
SOURCES += SplashWindow.cpp KeyWizard.cpp ConfigurationWizard.cpp


DESTDIR = ""
TARGET = abwizard
target.path += $${DESTDIR}/usr/bin/
INSTALLS += target
RESOURCES = abwizard.qrc

win32{
DEFINES += WIN32
CONFIG += qt windows
LIBS += -lpcre -lpsapi
INCLUDEPATH += libabackup
}
unix{
    CONFIG += debug warn_on rtti
}


