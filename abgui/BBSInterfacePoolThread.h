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
//      Name:    BBSInterfacePoolThread.h
//      Purpose: Hilo encargado de preguntar periodicamente a BBSI el estado de la conexión
//      Created: 31/1/08
//
// ------------------

#ifndef BBSINTERFACEPOOLTHREAD_H_
#define BBSINTERFACEPOOLTHREAD_H_

#include "BBSInterface.h"

#include <QThread>


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBSInterfacePoolThread
//      Purpose: Consulta periódicamente el estado de BBS
//      Created: 31/1/08
//
// ------------------

class BBSInterfacePoolThread: public QThread
{
    Q_OBJECT
	public:
     BBSInterfacePoolThread(BBSInterface *);
	 ~BBSInterfacePoolThread();
     void run();
    signals:
     void server_status_changed(BBSInterface::BBSStatus);
     void server_usage_changed(int);
     void server_connected(bool);
    private:
     BBSInterface * bbsi;
     int get_account_usage();
};

#endif /*BBSINTERFACEPOOLTHREAD_H_*/


