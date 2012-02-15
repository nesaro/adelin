TEMPLATE = lib
LIBS += -lssl -lcrypto -lz
INCLUDEPATH += . boxbackup-0.11.1/lib/common/ boxbackup-0.11.1/lib/backupclient boxbackup-0.11.1/lib/server  boxbackup-0.11.1/lib/crypto/  boxbackup-0.11.1/lib/compress/ /usr/share/adelinbackup/include/ /usr/include
SOURCES += AB.cpp BBCInterface.cpp Location.cpp Property.cpp  ABConfiguration.cpp BBBInterface.cpp BBDaemonInterfaces.cpp CheckBoxDirModel.cpp
HEADERS += AB.h BBCInterface.h Location.h Listeners.h Property.h ABConfiguration.h BBBInterface.h BBDaemonInterfaces.h CheckBoxDirModel.h
CONFIG += qt dll release thread build_all
TARGET = abackup
VERSION = 1


###ABTRAY

HEADERS += boxbackup-0.11.1/lib/common/Box.h
HEADERS += boxbackup-0.11.1/lib/common/BoxException.h
HEADERS += boxbackup-0.11.1/lib/common/Configuration.h
HEADERS += boxbackup-0.11.1/lib/common/autogen_CommonException.h
HEADERS += boxbackup-0.11.1/lib/common/Utils.h
HEADERS += boxbackup-0.11.1/lib/common/FdGetLine.h
HEADERS += boxbackup-0.11.1/lib/common/MemLeakFinder.h
SOURCES += boxbackup-0.11.1/lib/common/Configuration.cpp
SOURCES += boxbackup-0.11.1/lib/common/Utils.cpp
SOURCES += boxbackup-0.11.1/lib/common/BoxException.cpp
SOURCES += boxbackup-0.11.1/lib/common/autogen_CommonException.cpp
SOURCES += boxbackup-0.11.1/lib/common/DebugPrintf.cpp
SOURCES += boxbackup-0.11.1/lib/common/FdGetLine.cpp
SOURCES += boxbackup-0.11.1/lib/common/DebugAssertFailed.cpp


###ABGUI

HEADERS += boxbackup-0.11.1/lib/server/autogen_ServerException.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupStoreDirectory.h
HEADERS += boxbackup-0.11.1/lib/backupclient/autogen_BackupStoreException.h
HEADERS += boxbackup-0.11.1/lib/server/SSLLib.h
HEADERS += boxbackup-0.11.1/lib/crypto/CipherContext.h
HEADERS += boxbackup-0.11.1/lib/common/ReadLoggingStream.h
HEADERS += boxbackup-0.11.1/lib/common/Logging.h
HEADERS += boxbackup-0.11.1/lib/server/Socket.h
HEADERS += boxbackup-0.11.1/lib/server/SocketStreamTLS.h
HEADERS += boxbackup-0.11.1/lib/server/TLSContext.h
HEADERS += boxbackup-0.11.1/lib/common/BoxPlatform.h
HEADERS += boxbackup-0.11.1/lib/common/BoxTime.h
HEADERS += boxbackup-0.11.1/lib/common/BoxConfig.h
HEADERS += boxbackup-0.11.1/lib/common/StreamableMemBlock.h
HEADERS += boxbackup-0.11.1/lib/backupclient/autogen_BackupProtocolClient.h
HEADERS += boxbackup-0.11.1/lib/server/Protocol.h
HEADERS += boxbackup-0.11.1/lib/server/ProtocolObject.h
HEADERS += boxbackup-0.11.1/lib/server/ServerException.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupStoreFilenameClear.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupStoreFilename.h
HEADERS += boxbackup-0.11.1/lib/common/MemLeakFindOn.h
HEADERS += boxbackup-0.11.1/lib/common/MemLeakFindOff.h
HEADERS += boxbackup-0.11.1/lib/common/CommonException.h
HEADERS += boxbackup-0.11.1/lib/backupclient/autogen_BackupProtocolClient.h
HEADERS += boxbackup-0.11.1/lib/server/autogen_ConnectionException.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupClientCryptoKeys.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupStoreConstants.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupStoreFile.h
HEADERS += boxbackup-0.11.1/lib/common/BoxPortsAndFiles.h
HEADERS += boxbackup-0.11.1/lib/backupclient/BackupClientFileAttributes.h
HEADERS += boxbackup-0.11.1/lib/win32/emu.h
HEADERS += boxbackup-0.11.1/lib/common/FileModificationTime.h

SOURCES += boxbackup-0.11.1/lib/common/ReadLoggingStream.cpp
SOURCES += boxbackup-0.11.1/lib/common/Logging.cpp
SOURCES += boxbackup-0.11.1/lib/server/SSLLib.cpp\
boxbackup-0.11.1/lib/server/autogen_ServerException.cpp\
boxbackup-0.11.1/lib/server/autogen_ConnectionException.cpp\
boxbackup-0.11.1/lib/server/Socket.cpp\
boxbackup-0.11.1/lib/server/Protocol.cpp\
boxbackup-0.11.1/lib/server/ProtocolObject.cpp\
boxbackup-0.11.1/lib/server/TLSContext.cpp\
boxbackup-0.11.1/lib/server/SocketStreamTLS.cpp\
boxbackup-0.11.1/lib/server/ProtocolUncertainStream.cpp\
boxbackup-0.11.1/lib/common/BoxTime.cpp\
boxbackup-0.11.1/lib/common/StreamableMemBlock.cpp\
boxbackup-0.11.1/lib/common/PartialReadStream.cpp\
boxbackup-0.11.1/lib/common/FileStream.cpp\
boxbackup-0.11.1/lib/common/CollectInBufferStream.cpp\
boxbackup-0.11.1/lib/common/ReadGatherStream.cpp\
boxbackup-0.11.1/lib/common/FileModificationTime.cpp\
boxbackup-0.11.1/lib/compress/CompressStream.cpp\
boxbackup-0.11.1/lib/compress/autogen_CompressException.cpp\
boxbackup-0.11.1/lib/backupclient/autogen_BackupProtocolClient.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreFile.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreFileCryptVar.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreFilename.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreFilenameClear.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreDirectory.cpp\
boxbackup-0.11.1/lib/backupclient/autogen_BackupStoreException.cpp\
boxbackup-0.11.1/lib/backupclient/BackupStoreFileEncodeStream.cpp\
boxbackup-0.11.1/lib/backupclient/BackupClientCryptoKeys.cpp\
boxbackup-0.11.1/lib/backupclient/BackupClientFileAttributes.cpp\
boxbackup-0.11.1/lib/crypto/autogen_CipherException.cpp\
boxbackup-0.11.1/lib/crypto/CipherBlowfish.cpp\
boxbackup-0.11.1/lib/crypto/CipherAES.cpp\
boxbackup-0.11.1/lib/crypto/CipherDescription.cpp\
boxbackup-0.11.1/lib/crypto/Random.cpp\
boxbackup-0.11.1/lib/crypto/MD5Digest.cpp\
boxbackup-0.11.1/lib/crypto/RollingChecksum.cpp\
boxbackup-0.11.1/lib/crypto/CipherContext.cpp\


### ABGUI -> boxbackup

#HEADERS += BOSIOStream.h\
#BOSSocketStream.h
HEADERS += boxbackup-0.11.1/lib/common/IOStream.h
HEADERS += boxbackup-0.11.1/lib/common/IOStreamGetLine.h\
boxbackup-0.11.1/lib/server/SocketStream.h\

#SOURCES += BOSIOStream.cpp \
#BOSSocketStream.cpp
SOURCES += boxbackup-0.11.1/lib/common/IOStream.cpp
SOURCES += boxbackup-0.11.1/lib/common/IOStreamGetLine.cpp\
boxbackup-0.11.1/lib/server/SocketStream.cpp\

DEPENDPATH += boxbackup-0.11.1/lib/common/ \
boxbackup-0.11.1/lib/backupclient \
../boxbackup-0.11.1/release/lib/common/common.a

DEFINES += BOX_VERSION="\"0.11\""



unix {
DESTDIR = /usr/lib
libs.files += $$HEADERS
libs.path += /usr/share/adelinbackup/include/
INSTALLS += libs
}

INCLUDEPATH += boxbackup-0.11.1/lib/compress/ boxbackup-0.11.1/lib/win32/

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
