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
//		Name:    WinNamedPipeStream.h
//		Purpose: I/O stream interface for Win32 named pipes
//		Created: 2005/12/07
//
// --------------------------------------------------------------------------

#if ! defined WINNAMEDPIPESTREAM__H && defined WIN32
#define WINNAMEDPIPESTREAM__H

#include "BOSIOStream.h"

// --------------------------------------------------------------------------
//
// Class
//		Name:    WinNamedPipeStream
//		Purpose: I/O stream interface for Win32 named pipes
//		Created: 2003/07/31
//
// --------------------------------------------------------------------------
class BOSWinNamedPipeStream : public BOSIOStream
{
public:
	BOSWinNamedPipeStream();
	~BOSWinNamedPipeStream();

	// server side - create the named pipe and listen for connections
	void Accept(const wchar_t* Name);

	// client side - connect to a waiting server
	void Connect(const wchar_t* Name);

	// both sides
	virtual int Read(void *pBuffer, int NBytes, 
		int Timeout = BOSIOStream::TimeOutInfinite);
	virtual void Write(const void *pBuffer, int NBytes);
	virtual void Close();
	virtual bool StreamDataLeft();
	virtual bool StreamClosed();
	bool IsConnected() { return mIsConnected; }

protected:
	HANDLE GetSocketHandle();
	void MarkAsReadClosed()  {mReadClosed  = true;}
	void MarkAsWriteClosed() {mWriteClosed = true;}

private:
	BOSWinNamedPipeStream(const BOSWinNamedPipeStream &rToCopy) 
		{ /* do not call */ }

	HANDLE mSocketHandle;
	bool mReadClosed;
	bool mWriteClosed;
	bool mIsServer;
	bool mIsConnected;
};

#endif // WINNAMEDPIPESTREAM__H
