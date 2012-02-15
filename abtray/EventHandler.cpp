/***************************************
 *            EventHandler.cpp
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
//      Name:    EventHandler.cpp
//      Purpose: Event Handler
//      Created: 3/3/08
//
// ------------------

#include "EventHandler.h"


// --------------------------------------------------------------------------
//
// Method
//      Name:    EventHandler::EventHandler
//      Purpose: Constructor
//      Created: 4/3/08
//
// ------------------

EventHandler::EventHandler()
{
    es = new EventStore(this);
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    EventHandler::~EventHandler
//      Purpose: Constructor
//      Created: 12/3/08
//
// ------------------

EventHandler::~EventHandler()
{
    qDebug() << "EventHandler::~EventHandler:" << tr("Begin");
    es->writeFile();
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    void EventHandler::handle_new_event
//      Purpose: Registra un nuevo evento y realiza las acciones pertinentes
//      Created: 5/3/08
//
// ------------------

void EventHandler::handle_new_event(Event event)
{
    QString texttosend;
    //Guardar en la cola
    queue.enqueue(event);
    //Decidir si enviarle algo al trayicon
    switch(event.get_type())
    {
        case abbsd::storefull: texttosend += tr("Store full"); break;
        case abbsd::readerror: texttosend += tr("Read Error"); break;
        case abbsd::backupstart: texttosend += tr("Backup Start"); break;
        case abbsd::backupfinish: texttosend += tr("Backup Finish"); break;
        case abbsd::daemonstatuschanged: texttosend += tr("daemon status has changed"); break;
        case abbsd::daemonbadstatus: texttosend += tr("Bad daemon status"); break;
        default: texttosend += tr("Unknown Event"); break;
    }
    //emitir señal
    emit user_msg(texttosend);
    emit registered_event(event);
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    Event EventHandler::last_entry()
//      Purpose: devuelve el último evento
//      Created: 5/3/08
//
// ------------------

Event EventHandler::last_entry()
{
    return Event();
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    void EventHandler::saveXML()
//      Purpose: Guarda el fichero XML
//      Created: 12/3/08
//
// ------------------

void EventHandler::saveXML()
{
    es->writeFile();
}


// --------------------------------------------------------------------------
//
// Method
//      Name:    void EventHandler::loadXML()
//      Purpose: Carga el fichero XML
//      Created: 12/3/08
//
// ------------------

void EventHandler::loadXML()
{
    es->readFile();
    emit layout_changed();
}


