/***************************************
 *            Event.h
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
//      Name:    Event.h
//      Purpose: Eventos
//      Created: 6/3/08
//
// ------------------


#ifndef EVENT_H_
#define EVENT_H_
#include <QDateTime>
#include <QList>
#include <QObject>
#include "abbsd.h"


// --------------------------------------------------------------------------
//
// Class
//      Name:    Event
//      Purpose: Representa un evento
//      Created: 4/3/08
//
// ------------------

class Event
{
    public:
        enum EventSource{
            ES_LOG=0,
            ES_NOTIFY,   
            ES_SYSTEM,
            ES_INTERNAL
        };

        Event();
        Event(abbsd::EventCode);
        Event(abbsd::EventCode,QDateTime);
        abbsd::EventCode get_type() const;
        QDateTime get_date() const;
        QList<EventSource> get_sources() const;
        static std::string typetostring(abbsd::EventCode);
        static std::string sourcetostring(EventSource);
        static abbsd::EventCode codetotype(int code);
        void set_type(abbsd::EventCode);
        void set_type(QString);
        void set_date(QDateTime);
        void add_source(EventSource);
        void add_source(QString);
    private:
        abbsd::EventCode type;
        QDateTime date;
        QList<EventSource> sourcelist;

};


#endif /*EVENT_H_*/
