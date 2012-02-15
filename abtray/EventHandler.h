/***************************************
 *            EventHandler.h
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
//      Name:    EventHandler.h
//      Purpose: Manejador del fichero de logs de BB
//      Created: 3/3/08
//
// ------------------


#ifndef EVENTHANDLER_H_
#define EVENTHANDLER_H_
#include <QObject>
#include <QQueue>
#include <QMutex>
#include "Event.h"
#include "EventStore.h"



// --------------------------------------------------------------------------
//
// Class
//      Name:    EventHandler
//      Purpose: Clase manejadora de eventos
//      Created: 4/3/08
//
// ------------------

class EventHandler: public QObject
{
    Q_OBJECT
    public:
        EventHandler();
        ~EventHandler();
        Event last_entry();
        QQueue<Event> queue;
    signals:
        void user_msg(QString);
        void registered_event(Event);
        void layout_changed();
    public slots:
        void handle_new_event(Event);
        void saveXML();
        void loadXML();
    private:
        QMutex mutex;
        EventStore * es;
};

#endif /*EVENTHANDLER_H_*/
