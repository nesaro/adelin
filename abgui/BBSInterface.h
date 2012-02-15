/***************************************
 *            BBSInterface.h
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
//      Name:    BBSInterface.h
//      Purpose: Demonio de servidor BoxBackup
//      Created: 21/11/07
//
// ------------------

#ifndef BBSINTERFACE_H_
#define BBSINTERFACE_H_

#include "SSLLib.h"
#include "Socket.h"
#include "SocketStreamTLS.h"
#include "TLSContext.h"
#include "autogen_BackupProtocolClient.h"
#include "BackupStoreDirectory.h"


#include "Location.h"
#include "BBDaemonInterfaces.h"
#include "ABConfiguration.h"
#include "BBSNode.h"

#include <QMutex>

// --------------------------------------------------------------------------
//
// Class
//      Name:    BBSInterface
//      Purpose: Implementación de BBDaemonInterface que se comunica con el servidor bbrestored. No actua como hilo, conecta y desconecta en cada acción (función)
//      Created: 25/10/07
//
// ------------------
//
// Cada función devuelve falso si no se ha conseguido realizar con éxito.

class BBSInterface: public BBDaemonInterface
{
	public:

// --------------------------------------------------------------------------
//
// Enum
//      Name:    BBSStatus
//      Purpose: Estados en los que puede encontrarse la interfaz con el servidor
//      Created: 7/11/07
//
// ------------------

        enum BBSStatus
        {
            BBSS_UNKNOWN = 0,
            BBSS_AVAILABLE,
            BBSS_UNAVAILABLE,
        };

     BBSInterface(ABConfiguration* );
	 ~BBSInterface();
     BBSStatus check_status(); 
     bool restore_file(int64_t,int64_t,std::string);
     bool delete_file(int64_t,std::string);
     bool delete_directory(int64_t);
     bool list_directory(int64_t,int16_t,BackupStoreDirectory&);
     BackupProtocolClientAccountUsage * GetAccountUsage() ;
     int64_t root_directory_id() const {return BackupProtocolClientListDirectory::RootDirectory;}
     int64_t get_location_id(Location);
     BBSNode get_bbsnode(int64_t, int64_t);
    private:
	 bool connect(bool writable=false);
     bool connect2(bool );
	 void disconnect();
     bool InitTlsContext(TLSContext& target);
     SocketStreamTLS * mpSocket;
     BackupProtocolClient* mpConnection;
     ABConfiguration * conf;
     QMutex * mutex, * connected_protect, * writable_protect;
     bool writable_connection;
};

#endif /*BBSINTERFACE_H_*/


