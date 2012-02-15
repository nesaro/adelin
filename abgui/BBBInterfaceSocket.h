/***************************************
 *            BBBInterfaceSocket.h
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
//      Name:    BBBInterfaceSocket.h
//      Purpose: Interfaces con BBackupd
//      Created: 20/11/07
//
// ------------------

#ifndef BBBINTERFACESOCKET_H_
#define BBBINTERFACESOCKET_H_



#include <iostream> //Unstable

#include <QThread>
#include <QProcess>



#include "BBBInterface.h"

#include "Socket.h"
#include "IOStreamGetLine.h"
#include "BoxException.h" 

#ifndef WIN32
#include "SocketStream.h"
#else
#include "BOSWinNamedPipeStream.h"
#endif


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBBInterfaceSocket
//      Purpose: Implementación de BBDaemonInterface que se comunica con el cliente bbackupd
//      Created: 25/10/07
//
// ------------------
//
// Excepciones en BBBInterfaceSocket
// Al ser un hilo, todas las excepciones que genere deben ser capturadas en el bucle principal del hilo,

class BBBInterfaceSocket: public QThread, public BBBInterface
{
    Q_OBJECT
	public:
// --------------------------------------------------------------------------
//
// Enum
//      Name:    BBBInterfaceStatus
//      Purpose: Estados en los que puede encontrarse el hilo de control del cliente bbackupd
//      Created: 7/11/07
//
// ------------------

        enum BBBInterfaceStatus
        {
            BBBIS_UNKNOWN = 0,
            BBBIS_CONNECTING,
            BBBIS_CONNECTED,
            BBBIS_REQ_START, //Recibida señal de arranque
            BBBIS_STARTING, //Arrancando
            BBBIS_STARTWAIT,
            BBBIS_REQ_STOP,
            BBBIS_STOPPING,
            BBBIS_REQ_RESTART, //Solicitud de reinicio
            BBBIS_RESTARTING, //En proceso de reinicio
            BBBIS_REQ_SYNC,
            BBBIS_SYNCING,
            BBBIS_REQ_RELOAD,
            BBBIS_RELOADING,
            BBBIS_SHUTDOWN,
            BBBIS_PERM_PROBLEM, //Existe un problema de permisos
        };
	 ~BBBInterfaceSocket();
     BBBInterfaceSocket(ABConfiguration *);
     bool RequestStartClient();
     bool RequestRestartClient();
     void run();
     void shutdown(){QMutexLocker lock(mutex); CurrentInterfaceStatus=BBBIS_SHUTDOWN;}
     BBBInterfaceStatus interface_status() const { QMutexLocker lock(mutex); return CurrentInterfaceStatus; }

    signals:
     void bbb_interface_state_change();
     void bbb_client_state_change();
     void bbb_error_change();


    private:
     bool client_status_changer(BBBInterfaceStatus, BBBInterfaceStatus, std::string);
     BBBInterfaceStatus CurrentInterfaceStatus;
	 enum Error connect();
     void run2();
     //Do* == Action
     enum Error DoStartClient();
     enum Error DoReadLine();
     //On* == Slot
     void OnConnectedIdle();
     void OnConnecting();
     void OnReloadClient();
     void OnRestartClient();
     void OnStartClient();
     void OnSyncClient();
     bool _sendCommand(const char * cmd) { 
         assert(mapSocket.get()!=NULL);
         std::string str = cmd;
         str += "\n";
         mapSocket->Write(str.c_str(), str.length());
         //wxLogDebug("wrote to daemon: '%s'", cmd);
         return true;
     }
     std::auto_ptr<QProcess> mapBackupClientProcess;//TODO Estudiar si hace falta que sea un puntero
#ifndef WIN32
     std::auto_ptr<SocketStream> mapSocket;  
#else
     std::auto_ptr<BOSWinNamedPipeStream> mapSocket;  
#endif
     std::auto_ptr<IOStreamGetLine> mapReader; 

};


#endif /*BBBINTERFACESOCKET_H_*/
