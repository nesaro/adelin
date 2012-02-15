/***************************************
 *            FMWLostLocationWindow.cpp
 *   Copyright 2008 Adelin ??
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
//      Name:    FMWLostLocationWindow.cpp
//      Purpose: Dialogo de las location que ya no existen en el disco duro local
//      Created: 12/3/08
//
// ------------------

#include "FMWLostLocationWindow.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    FMWLostLocationWindow::FMWLostLocationWindow()
//      Purpose: Constructor
//      Created: 12/3/08
//
// ------------------
FMWLostLocationWindow::FMWLostLocationWindow(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f)
{
    conf = config;
    locations = config->bbconf->mLocations;
    ui.setupUi(this);
    ui.pushButton_2->setEnabled(false);
    connections();
    updateEntries();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLostLocationWindow::connections()
//      Purpose: Slots and Signals connections
//      Created: 12/3/08
//
// ------------------

void FMWLostLocationWindow::connections()
{
    connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(deleteLocation()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLostLocationWindow::deleteLocation()
//      Purpose: 
//      Created: 12/3/08
//
// ------------------

void FMWLostLocationWindow::deleteLocation()
{
    int row = ui.tableWidget->currentRow();
    QString locationpath = ui.tableWidget->item(row,1)->text();
    qDebug() << "FMWLostLocationWindow::deleteLocation:" << "Texto:" << locationpath;
    if(!locations->delete_location_by_path(locationpath.toStdString()))
        assert(false);
    updateEntries();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLostLocationWindow::restoreLocation()
//      Purpose: 
//      Created: 12/3/08
//
// ------------------

void FMWLostLocationWindow::restoreLocation()
{
    assert(false); //TODO
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLostLocationWindow::restoreLocation()
//      Purpose: 
//      Created: 12/3/08
//
// ------------------

void FMWLostLocationWindow::restoreLocation(std::string path)
{
}

void FMWLostLocationWindow::updateEntries()
{
    ui.tableWidget->clearContents();
    ui.tableWidget->setRowCount(0);
    std::vector<Location> locationvector;
    std::vector<Location>::const_iterator i;
    locationvector = locations->get_locations_vector();
    for (i=locationvector.begin();i!=locationvector.end();++i)
    {
        QString path = (*i).get_path().c_str();
        if (!QFile::exists(path))
        {
            ui.tableWidget->setRowCount(ui.tableWidget->rowCount()+1);
            QTableWidgetItem *newItem = new QTableWidgetItem((*i).get_name().c_str());
            ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,0,newItem);
            QTableWidgetItem *newItem2 = new QTableWidgetItem(path);
            ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,newItem2);
            
        }

    }
    
    //Para cada entrada de LocationContainer
    ////Obtener la ruta
    ////Si no existe la ruta en el sistema de ficheros
    //////Añadir par nombre - ruta
}
