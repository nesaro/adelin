/***************************************
 *            BBBInterface.h
 *   Copyright 2007-2008 Adelin
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
//      Name:    BBBInterface.h
//      Purpose: BBackupd Interfaces
//      Created: 20/11/07
//
// ------------------

#ifndef BBBINTERFACE_H_
#define BBBINTERFACE_H_


#include <QMutex>
#include "ABConfiguration.h"
#include "BBDaemonInterfaces.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    BBBInterface
//      Purpose: Parent abstract class for every BBBInterface
//      Created: 18/04/08
//
// ------------------

class BBBInterface: public BBDaemonInterface
{
	public:

// --------------------------------------------------------------------------
//
// Enum
//      Name:    BBBDaemonStatus
//      Purpose: bbackupd daemon status
//      Created: 21/11/07
//
// ------------------


        enum BBBDaemonStatus
        {
            BBBDS_UNKNOWN = -2,
            BBBDS_INIT = -1,
            BBBDS_IDLE = 0,
            BBBDS_CONNECTED = 1,
            BBBDS_CS_ERROR = 2,
            BBBDS_STORELIMIT = 3

        };

             enum Error {
         ERR_NONE = 0,
         ERR_UNKNOWN,
         ERR_INPROGRESS,
         ERR_STARTED,
         ERR_CONNECTED,
         ERR_INVALID_STATE,
         ERR_NOSOCKETCONFIG,
         ERR_SOCKETNOTFOUND,
         ERR_SOCKETREFUSED,
         ERR_NORESPONSE,
         ERR_BADRESPONSE,
         ERR_READTIMEOUT,
         ERR_DISCONNECTED,
         ERR_FILEUNKNOWN,
         ERR_FILENOTFOUND,
         ERR_EXECFAILED,
         ERR_STARTFAILED,
         ERR_NOPIDCONFIG,
         ERR_PIDFORMAT,
         ERR_RESOURCES,
         ERR_PROCESSNOTFOUND,
         ERR_INTERNAL,
         ERR_ACCESSDENIED,
         ERR_CMDFAILED,
         ERR_INTERRUPTED,
     };


        BBBInterface(ABConfiguration *);
        ~BBBInterface();
        Error last_error() const { return mLastError;}
        BBBDaemonStatus daemon_status() const { QMutexLocker lock(mutex); return CurrentDaemonStatus;}


    protected:
        ABConfiguration * conf;
        BBBDaemonStatus CurrentDaemonStatus;
        QMutex * mutex;
        enum Error mLastError;
        enum Error prevmLastError;

};


#endif /*BBBINTERFACES_H_*/
