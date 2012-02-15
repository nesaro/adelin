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
//      Name:    EventStore.h
//      Purpose: Almacenamiento de eventos.
//      Created: 5/3/08
//
// ------------------


#ifndef EVENTSTORE_H_
#define EVENTSTORE_H_
#include <QtGui>
#include <QtXml>
#include "Event.h"


class EventHandler;

enum EventStoreStatus{
    ESS_OK=0,
    ESS_NO,   
    ESS_RO,
    ESS_UNKNOWN,
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    XMLReader
//      Purpose: Clase encargada de leer el fichero XML
//      Created: 5/3/08
//
// ------------------

class XMLReader: public QXmlStreamReader
{
    public:
        XMLReader(EventHandler *);
        void readTMPNameXml();
        bool readFile(QIODevice *);
    private:
        EventHandler *eh;
        void readEventElement();
        void readUnknownElement();
        void readDate(Event *);
        void readType(Event *);
        void readSource(Event *);

};

// --------------------------------------------------------------------------
//
// Class
//      Name:    XMLWriter
//      Purpose: Clase encargada de escribir en el fichero XML
//      Created: 5/3/08
//
// ------------------

class XMLWriter: public QXmlStreamWriter
{
    public:
        XMLWriter(EventHandler *);
        bool writeFile(QIODevice *);
    private:
        EventHandler *eh;
        void writeItem(Event);

};

// --------------------------------------------------------------------------
//
// Class
//      Name:    EventStore
//      Purpose: Clases que asiste a EventHandler para guardar de forma estática los datos
//      Created: 4/3/08
//
// ------------------

class EventStore
{
    public:
        EventStore(EventHandler *);
        EventStoreStatus get_status() const;
        void readFile();
        void writeFile();
    private:
        EventStoreStatus status;
        XMLWriter * writer;
        XMLReader * reader;
        std::string filename;
        EventHandler * eh;
};


#endif /*EVENTSTORE_H_*/
