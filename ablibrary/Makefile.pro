TEMPLATE = lib
LIBS += -lssl -lcrypto -lz
BOXLIBPATH = boxbackup-0.11.1/lib/
INCLUDEPATH += . $$BOXLIBPATH/common/ $$BOXLIBPATH/backupclient $$BOXLIBPATH/server  $$BOXLIBPATH/crypto/  $$BOXLIBPATH/compress/ /usr/share/adelinbackup/include/ /usr/include
SOURCES += AB.cpp BBCInterface.cpp Location.cpp Property.cpp  ABConfiguration.cpp BBBInterface.cpp BBDaemonInterfaces.cpp CheckBoxDirModel.cpp
HEADERS += AB.h BBCInterface.h Location.h Listeners.h Property.h ABConfiguration.h BBBInterface.h BBDaemonInterfaces.h CheckBoxDirModel.h
CONFIG += qt dll release thread build_all
TARGET = abackup
VERSION = 1


###ABTRAY

HEADERS += $$BOXLIBPATH/common/Box.h
HEADERS += $$BOXLIBPATH/common/BoxException.h
HEADERS += $$BOXLIBPATH/common/Configuration.h
HEADERS += $$BOXLIBPATH/common/autogen_CommonException.h
HEADERS += $$BOXLIBPATH/common/Utils.h
HEADERS += $$BOXLIBPATH/common/FdGetLine.h
HEADERS += $$BOXLIBPATH/common/MemLeakFinder.h
SOURCES += $$BOXLIBPATH/common/Configuration.cpp
SOURCES += $$BOXLIBPATH/common/Utils.cpp
SOURCES += $$BOXLIBPATH/common/BoxException.cpp
SOURCES += $$BOXLIBPATH/common/autogen_CommonException.cpp
SOURCES += $$BOXLIBPATH/common/DebugPrintf.cpp
SOURCES += $$BOXLIBPATH/common/FdGetLine.cpp
SOURCES += $$BOXLIBPATH/common/DebugAssertFailed.cpp


###ABGUI

HEADERS += $$BOXLIBPATH/server/autogen_ServerException.h
HEADERS += $$BOXLIBPATH/backupclient/BackupStoreDirectory.h
HEADERS += $$BOXLIBPATH/backupclient/autogen_BackupStoreException.h
HEADERS += $$BOXLIBPATH/server/SSLLib.h
HEADERS += $$BOXLIBPATH/crypto/CipherContext.h
HEADERS += $$BOXLIBPATH/common/ReadLoggingStream.h
HEADERS += $$BOXLIBPATH/common/Logging.h
HEADERS += $$BOXLIBPATH/server/Socket.h
HEADERS += $$BOXLIBPATH/server/SocketStreamTLS.h
HEADERS += $$BOXLIBPATH/server/TLSContext.h
HEADERS += $$BOXLIBPATH/common/BoxPlatform.h
HEADERS += $$BOXLIBPATH/common/BoxTime.h
HEADERS += $$BOXLIBPATH/common/BoxConfig.h
HEADERS += $$BOXLIBPATH/common/StreamableMemBlock.h
HEADERS += $$BOXLIBPATH/backupclient/autogen_BackupProtocolClient.h
HEADERS += $$BOXLIBPATH/server/Protocol.h
HEADERS += $$BOXLIBPATH/server/ProtocolObject.h
HEADERS += $$BOXLIBPATH/server/ServerException.h
HEADERS += $$BOXLIBPATH/backupclient/BackupStoreFilenameClear.h
HEADERS += $$BOXLIBPATH/backupclient/BackupStoreFilename.h
HEADERS += $$BOXLIBPATH/common/MemLeakFindOn.h
HEADERS += $$BOXLIBPATH/common/MemLeakFindOff.h
HEADERS += $$BOXLIBPATH/common/CommonException.h
HEADERS += $$BOXLIBPATH/backupclient/autogen_BackupProtocolClient.h
HEADERS += $$BOXLIBPATH/server/autogen_ConnectionException.h
HEADERS += $$BOXLIBPATH/backupclient/BackupClientCryptoKeys.h
HEADERS += $$BOXLIBPATH/backupclient/BackupStoreConstants.h
HEADERS += $$BOXLIBPATH/backupclient/BackupStoreFile.h
HEADERS += $$BOXLIBPATH/common/BoxPortsAndFiles.h
HEADERS += $$BOXLIBPATH/backupclient/BackupClientFileAttributes.h
HEADERS += $$BOXLIBPATH/win32/emu.h
HEADERS += $$BOXLIBPATH/common/FileModificationTime.h

SOURCES += $$BOXLIBPATH/common/ReadLoggingStream.cpp
SOURCES += $$BOXLIBPATH/common/Logging.cpp
SOURCES += $$BOXLIBPATH/server/SSLLib.cpp\
$$BOXLIBPATH/server/autogen_ServerException.cpp\
$$BOXLIBPATH/server/autogen_ConnectionException.cpp\
$$BOXLIBPATH/server/Socket.cpp\
$$BOXLIBPATH/server/Protocol.cpp\
$$BOXLIBPATH/server/ProtocolObject.cpp\
$$BOXLIBPATH/server/TLSContext.cpp\
$$BOXLIBPATH/server/SocketStreamTLS.cpp\
$$BOXLIBPATH/server/ProtocolUncertainStream.cpp\
$$BOXLIBPATH/common/BoxTime.cpp\
$$BOXLIBPATH/common/StreamableMemBlock.cpp\
$$BOXLIBPATH/common/PartialReadStream.cpp\
$$BOXLIBPATH/common/FileStream.cpp\
$$BOXLIBPATH/common/CollectInBufferStream.cpp\
$$BOXLIBPATH/common/ReadGatherStream.cpp\
$$BOXLIBPATH/common/FileModificationTime.cpp\
$$BOXLIBPATH/compress/CompressStream.cpp\
$$BOXLIBPATH/compress/autogen_CompressException.cpp\
$$BOXLIBPATH/backupclient/autogen_BackupProtocolClient.cpp\
$$BOXLIBPATH/backupclient/BackupStoreFile.cpp\
$$BOXLIBPATH/backupclient/BackupStoreFileCryptVar.cpp\
$$BOXLIBPATH/backupclient/BackupStoreFilename.cpp\
$$BOXLIBPATH/backupclient/BackupStoreFilenameClear.cpp\
$$BOXLIBPATH/backupclient/BackupStoreDirectory.cpp\
$$BOXLIBPATH/backupclient/autogen_BackupStoreException.cpp\
$$BOXLIBPATH/backupclient/BackupStoreFileEncodeStream.cpp\
$$BOXLIBPATH/backupclient/BackupClientCryptoKeys.cpp\
$$BOXLIBPATH/backupclient/BackupClientFileAttributes.cpp\
$$BOXLIBPATH/crypto/autogen_CipherException.cpp\
$$BOXLIBPATH/crypto/CipherBlowfish.cpp\
$$BOXLIBPATH/crypto/CipherAES.cpp\
$$BOXLIBPATH/crypto/CipherDescription.cpp\
$$BOXLIBPATH/crypto/Random.cpp\
$$BOXLIBPATH/crypto/MD5Digest.cpp\
$$BOXLIBPATH/crypto/RollingChecksum.cpp\
$$BOXLIBPATH/crypto/CipherContext.cpp\


### ABGUI -> boxbackup

#HEADERS += BOSIOStream.h\
#BOSSocketStream.h
HEADERS += $$BOXLIBPATH/common/IOStream.h
HEADERS += $$BOXLIBPATH/common/IOStreamGetLine.h\
$$BOXLIBPATH/server/SocketStream.h\

#SOURCES += BOSIOStream.cpp \
#BOSSocketStream.cpp
SOURCES += $$BOXLIBPATH/common/IOStream.cpp
SOURCES += $$BOXLIBPATH/common/IOStreamGetLine.cpp\
$$BOXLIBPATH/server/SocketStream.cpp\

DEPENDPATH += $$BOXLIBPATH/common/ \
$$BOXLIBPATH/backupclient \
../boxbackup-0.11.1/release/lib/common/common.a

DEFINES += BOX_VERSION="\"0.11\""



unix {
DESTDIR = /usr/lib
libs.files += $$HEADERS
libs.path += /usr/share/adelinbackup/include/
INSTALLS += libs
}

INCLUDEPATH += $$BOXLIBPATH/compress/ $$BOXLIBPATH/win32/

win32{
DEFINES += WIN32
DEFINES -= HAVE_NETINET_IN_H UNICODE #Da problemas en windows con la version 4.3.4
SOURCES += boxbackup-0.11.1\lib\win32\emu.cpp
SOURCES += BOSWinNamedPipeStream.cpp
HEADERS += boxbackup-0.11.1\lib\win32\emu.h
HEADERS += BOSWinNamedPipeStream.h
LIBS +=  -lws2_32 -lpcre
CONFIG += qt windows

}
