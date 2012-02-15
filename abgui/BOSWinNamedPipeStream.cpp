// distribution boxbackup-0.10 (svn version: 494)
//  
// Copyright (c) 2003 - 2006
//      Ben Summers and contributors.  All rights reserved.
//  
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
// 3. All use of this software and associated advertising materials must 
//    display the following acknowledgment:
//        This product includes software developed by Ben Summers.
// 4. The names of the Authors may not be used to endorse or promote
//    products derived from this software without specific prior written
//    permission.
// 
// [Where legally impermissible the Authors do not disclaim liability for 
// direct physical injury or death caused solely by defects in the software 
// unless it is modified by a third party.]
// 
// THIS SOFTWARE IS PROVIDED BY THE AUTHORS ``AS IS'' AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
// WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.  IN NO EVENT SHALL THE AUTHORS BE LIABLE FOR ANY DIRECT,
// INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
// (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
// HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
// STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//  
//  
//  
// --------------------------------------------------------------------------
//
// File
//		Name:    WinNamedPipeStream.cpp
//		Purpose: I/O stream interface for Win32 named pipes
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------

#include "Box.h"

#ifdef WIN32

#ifdef HAVE_UNISTD_H
	#include <unistd.h>
#endif

#include <sys/types.h>
#include <errno.h>
#include <windows.h>

#include "BOSWinNamedPipeStream.h"
#include "ServerException.h"
#include "CommonException.h"
#include "Socket.h"
#include <QtDebug>

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::WinNamedPipeStream()
//		Purpose: Constructor (create stream ready for Open() call)
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------
BOSWinNamedPipeStream::BOSWinNamedPipeStream()
	: mSocketHandle(NULL),
	  mReadClosed(false),
	  mWriteClosed(false),
	  mIsServer(false),
	  mIsConnected(false)
{
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::~WinNamedPipeStream()
//		Purpose: Destructor, closes stream if open
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------
BOSWinNamedPipeStream::~BOSWinNamedPipeStream()
{
	if (mSocketHandle != NULL)
	{
		Close();
	}
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::Accept(const char* Name)
//		Purpose: Creates a new named pipe with the given name,
//			and wait for a connection on it
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------
void BOSWinNamedPipeStream::Accept(const wchar_t* pName)
{
	if (mSocketHandle != NULL || mIsConnected) 
	{
		THROW_EXCEPTION(ServerException, SocketAlreadyOpen)
	}

	mSocketHandle = CreateNamedPipeW( 
		pName,                     // pipe name 
		PIPE_ACCESS_DUPLEX,        // read/write access 
		PIPE_TYPE_MESSAGE |        // message type pipe 
		PIPE_READMODE_MESSAGE |    // message-read mode 
		PIPE_WAIT,                 // blocking mode 
		1,                         // max. instances  
		4096,                      // output buffer size 
		4096,                      // input buffer size 
		NMPWAIT_USE_DEFAULT_WAIT,  // client time-out 
		NULL);                     // default security attribute 

	if (mSocketHandle == NULL)
	{
		::syslog(LOG_ERR, "CreateNamedPipeW failed: %d", 
			GetLastError());
		THROW_EXCEPTION(ServerException, SocketOpenError)
	}

	bool connected = ConnectNamedPipe(mSocketHandle, (LPOVERLAPPED) NULL);

	if (!connected)
	{
		::syslog(LOG_ERR, "ConnectNamedPipe failed: %d", 
			GetLastError());
		Close();
		THROW_EXCEPTION(ServerException, SocketOpenError)
	}
	
	mReadClosed  = false;
	mWriteClosed = false;
	mIsServer    = true; // must flush and disconnect before closing
	mIsConnected = true;
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::Connect(const char* Name)
//		Purpose: Opens a connection to a listening named pipe
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------
void BOSWinNamedPipeStream::Connect(const wchar_t* pName)
{
	if (mSocketHandle != NULL || mIsConnected) 
	{
		THROW_EXCEPTION(ServerException, SocketAlreadyOpen)
	}
	
	mSocketHandle = CreateFileW( 
		pName,          // pipe name 
		GENERIC_READ |  // read and write access 
		GENERIC_WRITE, 
		0,              // no sharing 
		NULL,           // default security attributes
		OPEN_EXISTING,
		0,              // default attributes 
		NULL);          // no template file 

	if (mSocketHandle == INVALID_HANDLE_VALUE)
	{
		qDebug () << "Failed to connect to server's named pipe: error" << GetLastError();
		THROW_EXCEPTION(ServerException, SocketOpenError)
	}

	mReadClosed  = false;
	mWriteClosed = false;
	mIsServer    = false; // just close the socket
	mIsConnected = true;
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::Read(void *pBuffer, int NBytes)
//		Purpose: Reads data from stream. Maybe returns less than asked for.
//		Created: 2003/07/31
//
// --------------------------------------------------------------------------
int BOSWinNamedPipeStream::Read(void *pBuffer, int NBytes, int Timeout)
{
	// TODO no support for timeouts yet
	qDebug () << "BOSWinNamedPipeStream::Read: Comenzando";
	qDebug () << "BOSWinNamedPipeStream::Read: Valor pasado de Timeout" << Timeout;
	qDebug () << "BOSWinNamedPipeStream::Read: Valor de BOSIOStream::TimeOutInfinite" << BOSIOStream::TimeOutInfinite;
	ASSERT(Timeout == BOSIOStream::TimeOutInfinite)
	qDebug () << "BOSWinNamedPipeStream::Read: Pasado el assert con éxito";
	
	if (mSocketHandle == NULL || !mIsConnected) 
	{
		THROW_EXCEPTION(ServerException, BadSocketHandle)
	}

	DWORD NumBytesRead;
	
	bool Success = ReadFile( 
		mSocketHandle, // pipe handle 
		pBuffer,       // buffer to receive reply 
		NBytes,        // size of buffer 
		&NumBytesRead, // number of bytes read 
		NULL);         // not overlapped 
	
	if (!Success)
	{
		THROW_EXCEPTION(ConnectionException, Conn_SocketReadError)
	}
	
	// Closed for reading at EOF?
	if (NumBytesRead == 0)
	{
		mReadClosed = true;
	}
	
	return NumBytesRead;
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::Write(void *pBuffer, int NBytes)
//		Purpose: Writes data, blocking until it's all done.
//		Created: 2003/07/31
//
// --------------------------------------------------------------------------
void BOSWinNamedPipeStream::Write(const void *pBuffer, int NBytes)
{
	if (mSocketHandle == NULL || !mIsConnected) 
	{
		THROW_EXCEPTION(ServerException, BadSocketHandle)
	}
	
	// Buffer in byte sized type.
	ASSERT(sizeof(char) == 1);
	const char *pByteBuffer = (char *)pBuffer;
	
	int NumBytesWrittenTotal = 0;

	while (NumBytesWrittenTotal < NBytes)
	{
		DWORD NumBytesWrittenThisTime = 0;

		bool Success = WriteFile( 
			mSocketHandle,    // pipe handle 
			pByteBuffer + NumBytesWrittenTotal, // message 
			NBytes      - NumBytesWrittenTotal, // message length 
			&NumBytesWrittenThisTime, // bytes written this time
			NULL);            // not overlapped 

		if (!Success)
		{
			mWriteClosed = true;	// assume can't write again
			THROW_EXCEPTION(ConnectionException, 
				Conn_SocketWriteError)
		}

		NumBytesWrittenTotal += NumBytesWrittenThisTime;
	}
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::Close()
//		Purpose: Closes connection to remote socket
//		Created: 2003/07/31
//
// --------------------------------------------------------------------------
void BOSWinNamedPipeStream::Close()
{
	if (mSocketHandle == NULL && mIsConnected)
	{
		fprintf(stderr, "Inconsistent connected state\n");
		::syslog(LOG_ERR, "Inconsistent connected state");
		mIsConnected = false;
	}

	if (mSocketHandle == NULL) 
	{
		THROW_EXCEPTION(ServerException, BadSocketHandle)
	}

	if (mIsServer)
	{	
		if (!FlushFileBuffers(mSocketHandle))
		{
			::syslog(LOG_INFO, "FlushFileBuffers failed: %d", 
				GetLastError());
		}
	
		if (!DisconnectNamedPipe(mSocketHandle))
		{
			::syslog(LOG_ERR, "DisconnectNamedPipe failed: %d", 
				GetLastError());
		}

		mIsServer = false;
	}

	bool result = CloseHandle(mSocketHandle);

	mSocketHandle = NULL;
	mIsConnected = false;

	if (!result) 
	{
		::syslog(LOG_ERR, "CloseHandle failed: %d", GetLastError());
		THROW_EXCEPTION(ServerException, SocketCloseError)
	}
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::StreamDataLeft()
//		Purpose: Still capable of reading data?
//		Created: 2003/08/02
//
// --------------------------------------------------------------------------
bool BOSWinNamedPipeStream::StreamDataLeft()
{
	return !mReadClosed;
}

// --------------------------------------------------------------------------
//
// Function
//		Name:    WinNamedPipeStream::StreamClosed()
//		Purpose: Connection been closed?
//		Created: 2003/08/02
//
// --------------------------------------------------------------------------
bool BOSWinNamedPipeStream::StreamClosed()
{
	return mWriteClosed;
}

#endif // WIN32
