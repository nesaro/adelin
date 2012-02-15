/***************************************
 *            ABBSDClientListener.h
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdserver.
 *   abbsdserver is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdserver is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdserver.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    ABBSDClientListener.h
//      Purpose: Manejador del fichero de logs de BB
//      Created: 3/3/08
//
// ------------------


#ifndef ABBSDCLIENTLISTENER_H_
#define ABBSDCLIENTLISTENER_H_
#include <QThread>
#include <QTcpServer>
#include "Event.h"
#include "EventHandler.h"

class SocketHandler: public QThread
{
    Q_OBJECT
    public:
        SocketHandler(int socketDesctriptor);
        void run();
    signals:
        void new_event(Event);
    private:
        int socketDescriptor;
};

class ABBSDClientListener: public QTcpServer
{
    Q_OBJECT
    public:
        ABBSDClientListener(EventHandler *);
    protected:
        void incomingConnection(int socketDesctriptor);
    private:
        EventHandler * eh;
};

#endif /*ABBSDCLIENTLISTENER_H_*/
