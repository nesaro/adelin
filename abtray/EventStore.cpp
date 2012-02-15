/***************************************
 *            EventStore.cpp
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
//      Name:    EventStore.cpp
//      Purpose: Event Store
//      Created: 5/3/08
//
// ------------------

#include "EventStore.h"
#include "EventHandler.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    EventStore::EventStore()
//      Purpose: Constructor
//      Created: 12/3/08
//
// ------------------

EventStore::EventStore(EventHandler * handler) 
{
    eh = handler;
    writer = new XMLWriter(handler);
    reader = new XMLReader(handler);
    filename = "/tmp/example.xml";
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    EventStoreStatus EventStore::get_status() const
//      Purpose: Devuelve el estado de la instancia
//      Created: 4/3/08
//
// ------------------


EventStoreStatus EventStore::get_status() const
{
    return status;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void EventStore::readFile()
//      Purpose: Lee el XML del fichero
//      Created: 11/3/08
//
// ------------------


void EventStore::readFile() 
{
    qDebug() << "EventStore::readFile():" << "Begin";
    QFile tmpfile(filename.c_str());
    tmpfile.open(QIODevice::ReadOnly);
    reader->readFile(&tmpfile);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void EventStore::writeFile()
//      Purpose: Escribe el XML del fichero
//      Created: 11/3/08
//
// ------------------


void EventStore::writeFile() 
{
    if (!QFile::exists(filename.c_str()))
    {
        qDebug() << "EventStore::writeFile():" << "File doesn't exists";
    }
    QFile tmpfile(filename.c_str());
    tmpfile.open(QIODevice::ReadWrite);
    writer->writeFile(&tmpfile);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    XMLReader::XMLReader(EventHandler *)
//      Purpose: Constructor
//      Created: 4/3/08
//
// ------------------

XMLReader::XMLReader(EventHandler * handler)
{
    eh = handler;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readTMPNameXml()
//      Purpose: lee el elemento inicial asociado a un xml (Copiado del ejemplo de QT)
//      Created: 5/3/08
//
// ------------------

void XMLReader::readTMPNameXml()
{
    Q_ASSERT(isStartElement() && name() == "TMPName"); // Verificando que se nos pasa ele elemento correcto
    while (!atEnd())
    {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement()) {
            if (name() == "event")
                readEventElement();
            else
                readUnknownElement();
        }
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool XMLReader::readFile(QIODevice * )
//      Purpose: Lee el contenido de un fichero (Copiado del ejemplo XBel de qt)
//      Created: 5/3/08
//
// ------------------

bool XMLReader::readFile(QIODevice * device)
{
    qDebug() << "XMLReader::readFile():" << "Begin";
    setDevice(device);
    while (!atEnd())
    {
        qDebug() << "XMLReader::readFile():" << "Parse Loop:" << "Begin";
        readNext();
        if (isStartElement())
        {
            if (name() == "TMPName" && attributes().value("version") == "1.0")
                readTMPNameXml();
            else
                raiseError(QObject::tr("This file isi not an TMPName version 1.0"));
        }
    }
    return !error();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readEventElement
//      Purpose: Lee un elemento del tipo evento del fichero
//      Created: 5/3/08
//
// ------------------

void XMLReader::readEventElement()
{
    Q_ASSERT(isStartElement() && name() == "event"); 
    qDebug() << "XMLReader::readEventElement():" << "Begin"; 
    //Cargar las propiedades
    Event myevent; 
    while (!atEnd())
    {
       readNext();
       if (isEndElement())
           break;
       if (isStartElement()) {
           if (name() == "date")
               readDate(&myevent);
           else if (name() == "type")
               readType(&myevent);
           else if (name() == "source")
               readSource(&myevent);
           else
               readUnknownElement();
       }
    }
    //Cargar en la queue
    qDebug() << "XMLReader::readEventElement():" << "Type:" << myevent.get_type(); 
    eh->queue.enqueue(myevent);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readUnknownElement
//      Purpose: Lee un elemento del tipo evento del fichero. Deja pasar los elementos intermedios, y hace un assert si es un elemento raiz
//      Created: 5/3/08
//
// ------------------

void XMLReader::readUnknownElement()
{
    Q_ASSERT(isStartElement());
    while (!atEnd())
    {
        readNext();
        if (isEndElement())
            break;
        if (isStartElement())
            readUnknownElement(); //Generará una excepción
    }
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readDate(Event * )
//      Purpose: 
//      Created: 5/3/08
//
// ------------------

void XMLReader::readDate(Event * event)
{
    Q_ASSERT(isStartElement() && name() == "date"); 
    event->set_date(QDateTime::fromString(readElementText()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readType(Event * )
//      Purpose: 
//      Created: 5/3/08
//
// ------------------

void XMLReader::readType(Event * event)
{
    Q_ASSERT(isStartElement() && name() == "type"); 
    event->set_type(readElementText());
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void XMLReader::readSource(Event * )
//      Purpose: 
//      Created: 5/3/08
//
// ------------------

void XMLReader::readSource(Event * event)
{
    Q_ASSERT(isStartElement() && name() == "source"); 
    event->add_source(readElementText());
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    XMLWriter::XMLWriter(EventHandler * )
//      Purpose: Constructor
//      Created: 5/3/08
//
// ------------------

XMLWriter::XMLWriter(EventHandler * myeventhandler)
{
    eh = myeventhandler;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool XMLWriter::writeFile(EventHandler * )
//      Purpose: Constructor
//      Created: 5/3/08
//
// ------------------

bool XMLWriter::writeFile(QIODevice * device)
{
    qDebug() << "XMLWriter::writeFile:" << "Begin";
    setDevice(device);
    writeStartDocument();
    writeDTD("<!DOCTYPE TMPName>");
    writeStartElement("TMPName");
    writeAttribute("version", "1.0");
    QQueue<Event>::const_iterator i;
    for (i=eh->queue.constBegin(); i!= eh->queue.constEnd(); ++i)
    {
        qDebug() << "XMLWriter::writeFile:" << "Queue iterator:" << (*i).get_type();
        writeItem(*i);
    }
    writeEndDocument();
    qDebug() << "XMLWriter::writeFile:" << "End";
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    XMLWriter::writeItem(Event)
//      Purpose: Constructor
//      Created: 5/3/08
//
// ------------------

void XMLWriter::writeItem(Event myevent)
{
    writeStartElement("event");
    QDateTime mydate = myevent.get_date();
    QString datestring = mydate.toString();
    writeTextElement("date",datestring);
    QString classstring = Event::typetostring(myevent.get_type()).c_str();
    writeTextElement("type",classstring);
    QList<Event::EventSource> sourceslist = myevent.get_sources();
    QList<Event::EventSource>::const_iterator i;
    for (i = sourceslist.constBegin(); i != sourceslist.constEnd(); ++i)
        writeTextElement("source",Event::sourcetostring(*i).c_str());
    writeEndElement();
}



