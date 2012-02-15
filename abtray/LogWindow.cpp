/***************************************
 *            LogWindow.cpp
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdclient.
 *   abbsdclient is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdclient is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdclient.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    LogWindow.cpp
//      Purpose: Ventana que muestra los logs
//      Created: 3/3/08
//
// ------------------


#include "LogWindow.h"

LogWindow::LogWindow(EventHandler * eventhandler)
{
    ui.setupUi(this);
    eh = eventhandler;
    connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void LogWindow::connections
//      Purpose: Realiza las conexiones SLOT SIGNAL
//      Created: 6/3/08
//
// ------------------

void LogWindow::connections()
{
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(hide()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), eh, SLOT(saveXML()));
	QObject::connect(eh, SIGNAL(layout_changed()), this, SLOT(refresh()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void LogWindow::handle_registered_signal(Event event)
//      Purpose: Cuando EventHandler Ha registrado un evento, logwindow debe mostrarlo
//      Created: 11/3/08
//
// ------------------

void LogWindow::handle_registered_event(Event event)
{
   ui.tableWidget->setRowCount(ui.tableWidget->rowCount()+1); 
   QTableWidgetItem *newItem = new QTableWidgetItem(Event::typetostring(event.get_type()).c_str());
   ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,0,newItem);
   QTableWidgetItem *newItem2 = new QTableWidgetItem(event.get_date().toString());
   ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,newItem2);
   QList<Event::EventSource> eventsources = event.get_sources();
   QString sourceslist;
   QList<Event::EventSource>::const_iterator i;
   for (i = eventsources.constBegin(); i != eventsources.constEnd(); ++i)
       sourceslist+=Event::sourcetostring(*i).c_str();
   QTableWidgetItem *newItem3 = new QTableWidgetItem(sourceslist);
   ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,2,newItem3);
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    void LogWindow::refresh()
//      Purpose: Recarga el contenido de la ventana
//      Created: 12/3/08
//
// ------------------

void LogWindow::refresh()
{
    ui.tableWidget->clear();
    ui.tableWidget->setRowCount(eh->queue.size()); 
    QQueue<Event>::const_iterator i;
    int j=0;
    for (i=eh->queue.constBegin(); i!= eh->queue.constEnd(); ++i)
    {
        Event event;
        event = (*i); //Copia
        QTableWidgetItem *newItem = new QTableWidgetItem(Event::typetostring(event.get_type()).c_str());
        ui.tableWidget->setItem(j,0,newItem);
        QTableWidgetItem *newItem2 = new QTableWidgetItem(event.get_date().toString());
        ui.tableWidget->setItem(j,1,newItem2);
        QList<Event::EventSource> eventsources = event.get_sources();
        QString sourceslist;
        QList<Event::EventSource>::const_iterator k;
        for (k = eventsources.constBegin(); k != eventsources.constEnd(); ++k)
            sourceslist+=Event::sourcetostring(*k).c_str();
        QTableWidgetItem *newItem3 = new QTableWidgetItem(sourceslist);
        ui.tableWidget->setItem(j,2,newItem3);
        j++;
    }
}



