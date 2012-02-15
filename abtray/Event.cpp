/***************************************
 *            Event.cpp
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
//      Name:    Event.cpp
//      Purpose: Event 
//      Created: 6/3/08
//
// ------------------

#include "Event.h"
#include <QtDebug>
#include <assert.h>

// --------------------------------------------------------------------------
//
// Method
//      Name:    Event::Event
//      Purpose: Constructor
//      Created: 4/3/08
//
// ------------------

Event::Event()
{
    type = abbsd::INVALID_EVENT;
    date = QDateTime::currentDateTime();
}

Event::Event(abbsd::EventCode mytype)
{
    date = QDateTime::currentDateTime();
    type = mytype;
}

Event::Event(abbsd::EventCode mytype, QDateTime mydate)
{
    date = mydate;
    type = mytype;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void Event::set_type()
//      Purpose: Establece el tipo de evento
//      Created: 5/3/08
//
// ------------------

void Event::set_type(QString typestring)
{
    int pos=-1;
    for (int i=0; i<(sizeof(abbsd::EventNames)/sizeof(std::string));i++)
        if (typestring.toStdString()==abbsd::EventNames[i]) {pos=i;break;}
    if (pos==-1) assert(false);
    set_type((abbsd::EventCode)pos);
}

void Event::set_type(abbsd::EventCode mytype)
{
    type = mytype;
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    abbsd::EventCode Event::get_type()
//      Purpose: Devuelve el tipo de evento
//      Created: 5/3/08
//
// ------------------

abbsd::EventCode Event::get_type()  const { return type; }
QDateTime Event::get_date()  const { return date; }

void Event::set_date(QDateTime mydate)
{
    date = mydate;
}

// --------------------------------------------------------------------------
//
// Method
//      Name:    void Event::add_source()
//      Purpose: Añade una fuente al evento
//      Created: 5/3/08
//
// ------------------

void Event::add_source(EventSource myeventsource)
{
    if (!sourcelist.contains(myeventsource))
        sourcelist.append(myeventsource);
}

QList<Event::EventSource> Event::get_sources() const
{
    return sourcelist;
}

void Event::add_source(QString mystring)
{
    if (mystring == "ES_LOG")
        add_source(ES_LOG);
    else if (mystring == "ES_NOTIFY")
        add_source(ES_NOTIFY);
    else if (mystring == "ES_SYSTEM")
        add_source(ES_SYSTEM);
    else if (mystring == "ES_INTERNAL")
        add_source(ES_INTERNAL);
    else
        assert(false); //TODO Excepcion
}


// --------------------------------------------------------------------------
//
// Method
//      Name:    void Event::add_source()
//      Purpose: Añade una fuente al evento
//      Created: 5/3/08
//
// ------------------

std::string Event::typetostring(abbsd::EventCode mytype)
{
    return abbsd::EventNames[(int)mytype];
}

std::string Event::sourcetostring(EventSource mysource)
{
    if (mysource == ES_LOG)
        return std::string("ES_LOG");
    else if (mysource == ES_NOTIFY)
        return std::string("ES_NOTIFY");
    else if (mysource == ES_SYSTEM)
        return std::string("ES_SYSTEM");
    else if (mysource == ES_INTERNAL)
        return std::string("ES_INTERNAL");
    else
        qDebug() << "Event::sourcetostring: Error value: " << mysource; 
        assert(false);
}

abbsd::EventCode Event::codetotype(int code)
{
    //TODO: Verificar que no se sale de rango
    if (code<0 || code >= abbsd::numevents)
    {
        return abbsd::INVALID_EVENT;
    }
    return (abbsd::EventCode)code;
}

