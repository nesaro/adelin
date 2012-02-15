/***************************************
 *            BBBInterfaceSocket.cpp
 *   Copyright 2007-2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of Adelin Backup.
 *   Adelin Backup is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   Adelin Backup is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Adelin Backup.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    BBBInterfaceSocket.cpp
//      Purpose: bbackupd binary interface. Socket version
//      Created: 20/11/07
//
// ------------------

#include <iostream> 
//#include "IOStream.h" //second include was used to succesfully load socketstream. It was required to include it after <iostream>, but now it is not required

#include "BBBInterfaceSocket.h"

#define BOX_NAMED_PIPE_NAME L"\\\\.\\pipe\\boxbackup" //TODO: Get it from Config

#include <QMutexLocker>
#include <QtDebug>

#include <assert.h>

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBBInterfaceSocket::BBBInterfaceSocket()  
//      Purpose: Constructor
//      Created: 16/11/07
//
// ------------------


BBBInterfaceSocket::BBBInterfaceSocket(ABConfiguration * configuration):
    BBBInterface(configuration)
{	
    CurrentInterfaceStatus = BBBIS_CONNECTING;
    prevmLastError = mLastError = ERR_NONE;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBBInterfaceSocket::~BBBInterfaceSocket()  
//      Purpose: Destructor
//      Created: 31/10/08
//
// ------------------


BBBInterfaceSocket::~BBBInterfaceSocket()
{	
    if(mapReader.get()!=NULL && mapSocket.get()!=NULL)
    {
	    _sendCommand("quit");
    }
    if(mapReader.get()!=NULL) 
    {
	    mapReader.reset();
    }
    if (mapSocket.get()!=NULL)
    {
        try
        {
            mapSocket->Close();
        }
        catch (BoxException& be)  {} //Bad socket handle. Do Nothing
	    mapSocket.reset();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBBInterfaceSocket::Error BBBInterfaceSocket::connect()  
//      Purpose: Connects to bbackupd socket 
//      Created: 31/10/07
//
// ------------------
//

BBBInterfaceSocket::Error BBBInterfaceSocket::connect() //It doesn't generate exceptions, only error codes
{
    std::string CommandSocket;
    // Is socket defined at configuration
    if (!conf->bbconf->pathprops["CommandSocket"]->GetInto(CommandSocket)) {return ERR_NOSOCKETCONFIG;}
    qDebug() << "BBBInterfaceSocket::connect(): CommandSocket value:" << CommandSocket.c_str();
#ifdef WIN32
    if(mapReader.get()!=NULL && mapSocket.get()!=NULL)
    {
	    _sendCommand("quit");
    }
#endif
    qDebug() << "BBBInterfaceSocket::connect(): freeing MAPREADER";
    {
        QMutexLocker lock(mutex);
        //if(mapReader.get()!=NULL) mapReader->DetachFile(); //Segfault while trying to connect second time
        mapReader.reset();
    }
    qDebug() << "BBBInterfaceSocket::connect(): freeing MAPSOCKET";
    {
        QMutexLocker lock(mutex);
        if (mapSocket.get()!=NULL)
        {
            try
            {
                mapSocket->Close();
            }
            catch (BoxException& be)  {} //Bad socket handle. Do Nothing
            mapSocket.reset();
        }
#ifndef WIN32
        SocketStream * tempsocket;
        tempsocket = new SocketStream();
#else
	BOSWinNamedPipeStream * tempsocket;
        tempsocket = new BOSWinNamedPipeStream();
#endif
        qDebug() << "BBBInterfaceSocket::connect(): new socket: " << tempsocket;
    mapSocket.reset(tempsocket);
    }
    qDebug() << "BBBInterfaceSocket::connect(): Opening new MAPSOCKET";
    //Trying to open socket
    try
    {
        QMutexLocker lock(mutex);
#ifndef WIN32
        mapSocket->Open(Socket::TypeUNIX, CommandSocket.c_str());
#else
        mapSocket->Connect(BOX_NAMED_PIPE_NAME);
#endif
    }
    catch(BoxException& e)
    {
        qDebug() << "BBBInterfaceSocket::connect: Exception while opening socket:" << "BoxException. type="<<e.what() ;
        return ERR_ACCESSDENIED;
    }
    catch(...)
    {
        qDebug() << "BBBInterfaceSocket::connect: Unknown Exception while opening socket:" << "TODO: Show exception info" ;
        return ERR_SOCKETREFUSED;
    }
    qDebug() << "BBBInterfaceSocket::connecti: checking if client status is ok";
    //This block is locked by mutex
    //Check if client status is expected
    {
        QMutexLocker lock(mutex);
        if (CurrentInterfaceStatus != BBBIS_CONNECTING) {
            qDebug() << "BBBInterfaceSocket::connect: Not expected client status";
            return ERR_INTERRUPTED;
        }
    }
    // Wait for the configuration summary

    qDebug() << "BBBInterfaceSocket::connect: creating new MAPREADER";
    {
        QMutexLocker lock(mutex);
        mapReader.reset(new IOStreamGetLine(*mapSocket));
    }
    std::string ConfigSummary;


    qDebug() << "BBBInterfaceSocket::connect: get new line from MAPREADER";
    try {
        if (!mapReader->GetLine(ConfigSummary,0)) {
            qDebug() << "Failed to read status from client";
            return ERR_NORESPONSE;
        }
	else
	{
            qDebug() << "BBBInterfaceSocket::connect: client status: <<" << ConfigSummary.c_str();
	}

    } 
    catch (BoxException& be)  {
	qDebug() << "BBBInterfaceSocket::connect: Exception while reading status from client" << be.what();
        return ERR_NORESPONSE;
    }
    catch (...) {
	qDebug() << "BBBInterfaceSocket::connect: Exception while reading status from client";
        return ERR_NORESPONSE;
    }

    qDebug() << "BBBInterfaceSocket::connect: Status check ";
    {
        QMutexLocker lock(mutex);
        if (CurrentInterfaceStatus != BBBIS_CONNECTING) {
            qDebug() << "GetLine interrupted";
            return ERR_INTERRUPTED;
        }
    }

    if (mapReader->IsEOF()) return ERR_DISCONNECTED;

    return ERR_NONE;

}



// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::run2()
//      Purpose: thread main function
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::run2()
{	
    qDebug() << "BBBInterfaceSocket::run2(): Begin";
    BBBInterfaceStatus prevStatus;
    while (CurrentInterfaceStatus != BBBIS_SHUTDOWN)
    {
        prevStatus = CurrentInterfaceStatus;
        prevmLastError = mLastError;
        try{
            switch (prevStatus)
            {
                case BBBIS_CONNECTING:
                    qDebug("BBBInterfaceSocket::run2: Trying to connect");
                    OnConnecting(); break;
                case BBBIS_CONNECTED:
                    OnConnectedIdle(); 
                    break;
                case BBBIS_RESTARTING:
                    CurrentInterfaceStatus = BBBIS_CONNECTING;
                    break;
                case BBBIS_REQ_START:
                    OnStartClient(); break;
                case BBBIS_REQ_RESTART:
                    OnRestartClient(); break;
                case BBBIS_SHUTDOWN: 
                    break;
                default:
                    qDebug("Unknown client worker state %d", prevStatus); //FIXME should notify with qfatal
                    break;
            }
        }
	catch (BoxException& be)  {
            qDebug() << "BBBInterfaceSocket::run2(): Exception inside bbackupd client control thread:" << be.what();
	}
        catch (...) //FIXME avoid generic exception handling
        {
            qDebug() << "BBBInterfaceSocket::run2(): Excepción inside bbackupd client control thread";
        }
        qDebug() << "BBBInterfaceSocket::run2(): New client control thread iteration, current status = "<< prevStatus;
        if (prevStatus != CurrentInterfaceStatus || mLastError!=prevmLastError)
        {
            //QMutexLocker lock(mutex); 
            emit bbb_interface_state_change();
            //listener->notify_bbb_interface_state_change();
            qDebug() << "BBBInterfaceSocket::run2(): BBBInterfaceSocket status change (or error):" << prevStatus << "to" << CurrentInterfaceStatus << ". Error " << prevmLastError << "to" << mLastError ;
        }
        if (CurrentInterfaceStatus == BBBIS_CONNECTING || CurrentInterfaceStatus == BBBIS_STARTWAIT)
        {
            sleep(10);
        }
        else
        {
            sleep(3);
        }
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::run()
//      Purpose: Safe thread execution
//      Created: 22/11/07
//
// ------------------


void BBBInterfaceSocket::run()
{	
    try{
        run2();
    }
    catch(...)
    {
        qDebug() << "BBBInterfaceSocket::run: Handling exception";
        sleep(1);
        exit();
        return;
    }
}


// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnConnecting()
//      Purpose: What to do when connecting (BBBIS_CONNECTING)
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::OnConnecting()
{	
    {
        QMutexLocker lock(mutex); 
        assert(CurrentInterfaceStatus == BBBIS_CONNECTING);
    }
    BBBInterfaceSocket::Error result = connect();
    qDebug() << "BBBInterfaceSocket::OnConnecting: connect call output: " << result;
    {
        QMutexLocker lock(mutex);
        if(CurrentInterfaceStatus != BBBIS_CONNECTING)
        {
            qDebug() << "BBBInterfaceSocket::OnConnecting: Client connect interrupted";
            assert(false); //FIXME: Exception
        }
        if (result == ERR_NONE)
            CurrentInterfaceStatus = BBBIS_CONNECTED;
        if (result != ERR_INTERRUPTED)
        {
            mLastError = result;
        }
    }

}


// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnConnectedIdle()
//      Purpose: What to do when client is ok (BBBIS_CONNECTED,BBBIS_RESTARTING?)
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::OnConnectedIdle()
{	
    qDebug("BBBInterfaceSocket::OnConnectedIdle: Begin"); 
    {
        QMutexLocker lock(mutex); 
        assert(CurrentInterfaceStatus == BBBIS_CONNECTED ||
                CurrentInterfaceStatus == BBBIS_RESTARTING);
    }
    //Error result = DoReadLine(); //FIXME: Windows requires a timeout
    Error result = ERR_NONE;
    qDebug() << "BBBInterfaceSocket::OnConnectedIdle: DoReadLine call output: " << result;
    {
        QMutexLocker lock(mutex); 
        if (CurrentInterfaceStatus != BBBIS_CONNECTED &&
                CurrentInterfaceStatus != BBBIS_RESTARTING)
        {
            mLastError = ERR_INTERRUPTED;
            return;
        }
        if (result == ERR_DISCONNECTED) {
            qDebug("BBBInterfaceSocket::OnConnectedIdle: ERR_DISCONNECTED"); 
            CurrentDaemonStatus = BBBDS_UNKNOWN;
            if (CurrentInterfaceStatus == BBBIS_CONNECTED)
                CurrentInterfaceStatus = BBBIS_CONNECTING;
            else if (CurrentInterfaceStatus == BBBIS_RESTARTING)
            {
                qDebug("BBBInterfaceSocket::OnConnectedIdle: ERR_DISCONNECTED and BBBIS_RESTARTING"); 
                CurrentInterfaceStatus = BBBIS_REQ_START;
                result = ERR_NONE;
            }
            else
                assert(false);
        }
        if (result == ERR_READTIMEOUT)
            mLastError = ERR_NONE;
        else
            mLastError = result;
    }
}


// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnRestartClient()
//      Purpose: What to do on restart (BBBIS_REQ_RESTART)
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::OnRestartClient()
{	
    {
        QMutexLocker lock(mutex);
        assert(CurrentInterfaceStatus == BBBIS_REQ_RESTART);
        CurrentInterfaceStatus = BBBIS_RESTARTING;
    }
    bool result = _sendCommand("reload");
    qDebug() << "BBBInterfaceSocket::OnRestartClient(): client response: " << result;
    {
        QMutexLocker lock(mutex);
        mLastError = result ? ERR_NONE : ERR_UNKNOWN;
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnSyncClient()
//      Purpose: What to do when syncronizing (BBBIS_SYNC)
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::OnSyncClient()
{	
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnReloadClient()
//      Purpose: What to do while reloading (BBBIS_RELOAD)
//      Created: 16/11/07
//
// ------------------


void BBBInterfaceSocket::OnReloadClient()
{	
}


// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::DoReadLine()
//      Purpose: Reads 1 line from socket
//      Created: 16/11/07
//
// ------------------

BBBInterfaceSocket::Error BBBInterfaceSocket::DoReadLine()
{	
    qDebug() << "BBBInterfaceSocket::DoReadLine(): Begin";
    std::string Line;
    bool result = false;    

    try {
	qDebug() << "BBBInterfaceSocket::DoReadLine(): Trying to read 1 line";
    qDebug() << "BBBInterfaceSocket::DoReadLine():" << "Data left in stream?" << mapSocket->StreamDataLeft();
#ifdef WIN32
    result = mapReader->GetLine(Line, 0); //FIXME: Verificar como emular timeout en windows
#else
    result = mapReader->GetLine(Line, 0, 1000); 
#endif
	qDebug() << "BBBInterfaceSocket::DoReadLine(): Getline returned line:" << Line.c_str();
    }
    catch (BoxException& be)  
    {
	qDebug() << "BBBInterfaceSocket::DoReadLine(): Exception while reading line:" << be.what() ;
        return ERR_DISCONNECTED;
    }

    if (!result) 
    {
        return ERR_READTIMEOUT;
    }

    if (mapReader->IsEOF()) return ERR_DISCONNECTED;

    if (Line == "ok")
        return ERR_NONE;

    if (Line == "error")
        return ERR_CMDFAILED;

    std::string line2 = Line.c_str();
    std::string rest;
    std::string::size_type pos = line2.find("state ");
    if (pos == 0 )
    {
        long value;
        //if (!rest.ToLong(&value))
        if (!(value = atoi(rest.c_str()))) //FIXME: Is 0 valid?
            return ERR_BADRESPONSE;
        if (value >= BBBDS_INIT && value <= BBBDS_STORELIMIT)
        {
            CurrentDaemonStatus = (BBBDaemonStatus)value;
        }
        else
        {
            CurrentDaemonStatus = BBBDS_UNKNOWN;
            return ERR_BADRESPONSE;
        }
    }

    qDebug() << "read from daemon: " << Line.c_str();
    return ERR_NONE;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:  enum Error BBBInterfaceSocket::DoStartClient()
//      Purpose: launch daemon execution
//      Created: 19/11/07
//
// ------------------

BBBInterfaceSocket::Error BBBInterfaceSocket::DoStartClient()
{	
    qDebug() << "BBBInterfaceSocket::DoStartClient(): Begin";
    std::string ClientPath=conf->get_bb_binary_filename();
    qDebug() << "BBBInterfaceSocket::DoStartClient(): Binary path:" << conf->get_bb_binary_filename().c_str() ;
    if (ClientPath==std::string())
    {
        return ERR_FILEUNKNOWN;
    }

    if (ClientPath.length() == 0)
    {
        return ERR_FILEUNKNOWN;
    }

    //getting configuration and client path
    std::auto_ptr<char> ClientPathStr = std::auto_ptr<char>(strdup(ClientPath.c_str()));
    std::auto_ptr<char> ConfigFileStr = std::auto_ptr<char>(strdup(conf->get_bb_config_filename().c_str()));

    {
        QMutexLocker lock(mutex);
        qDebug() << "Start client: creating new process object";
        // the old object will delete itself 
        // when the process terminates
        //mapBackupClientProcess.release();
        //delete mapBackupClientProcess; //FIXME: Poner
        mapBackupClientProcess.reset(new QProcess());
        //mapBackupClientProcess->Redirect();
    }

    QStringList arguments;
    arguments << ConfigFileStr.get();
    mapBackupClientProcess->start(ClientPathStr.get(),arguments);
    //FIXME Avoid wait if possible
    if (!mapBackupClientProcess->waitForStarted()) return ERR_EXECFAILED;

    //// the ClientProcess class will call our OnClientStarted method
    //// when the BoxBackup master process terminates (and hopefully
    //// it has forked into the background!)
    return ERR_NONE;

}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBBInterfaceSocket::OnStartClient()
//      Purpose: What to do on client start request
//      Created: 20/11/07
//
// ------------------

void BBBInterfaceSocket::OnStartClient()
{	
    qDebug() << "BBBInterfaceSocket::OnStartClient(): Begin";
    {
        QMutexLocker lock(mutex); 
        //Status check
        assert(CurrentInterfaceStatus == BBBIS_REQ_START);
        //status changed to STARTING 
        CurrentInterfaceStatus = BBBIS_STARTING;
    }
    BBBInterfaceSocket::Error result = DoStartClient();
    {
        QMutexLocker lock(mutex); 
        assert(CurrentInterfaceStatus == BBBIS_STARTING);
        // no notification until forked process ends? don't wait for it!
        // mCurrentState = (result == ERR_NONE) ? BST_STARTWAIT : BST_CONNECTING;
        CurrentInterfaceStatus = BBBIS_CONNECTING;
        mLastError = result;
    }


}

// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBBInterfaceSocket::RequestStartClient()
//      Purpose: a start is requested
//      Created: 20/11/07
//
// ------------------

bool BBBInterfaceSocket::RequestStartClient()
{	
    return client_status_changer(BBBIS_CONNECTING, BBBIS_REQ_START, "start client");
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBBInterfaceSocket::RestartClient()
//      Purpose: a restart is requested
//      Created: 3/1/08
//
// ------------------

bool BBBInterfaceSocket::RequestRestartClient()
{	
    if (!conf->check_bb_binary_filename()) return false; //Can't restart if bbackupd doesn't exists
    return client_status_changer(BBBIS_CONNECTED, BBBIS_REQ_RESTART, "restart client");
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBBInterfaceSocket::client_status_changer(BBBInterfaceStatus, BBBInterfaceStatus, std::string)
//      Purpose: requested client status change
//      Created: 20/11/07
//
// ------------------

bool BBBInterfaceSocket::client_status_changer(BBBInterfaceStatus oldState, BBBInterfaceStatus newState, std::string description)
{
    QMutexLocker lock(mutex);

    if (CurrentInterfaceStatus != oldState) {
        qDebug() << "BBBInterface::client_status_changer: Wrong state for" << description.c_str() << ": should be " << (int)oldState << " but currently" << (int)CurrentInterfaceStatus;
        mLastError = ERR_INVALID_STATE;
        return false;
    }

    CurrentInterfaceStatus = newState;
    return true;
}

