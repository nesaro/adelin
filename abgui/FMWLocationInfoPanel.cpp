/***************************************
 *            FMWLocationInfoPanel.cpp
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
//      Name:    FMWLocationInfoPanel.cpp
//      Purpose: Widget que muestra la información de una Location dada
//      Created: 30/11/07
//
// ------------------

#include "FMWLocationInfoPanel.h"
#include "FileManagementWidget.h"

#include <QtDebug>

// --------------------------------------------------------------------------
//
// Function
//      Name:    FMWLocationInfoPanel::FMWLocationInfoPanel
//      Purpose: Constructor
//      Created: 30/11/07
//
// ------------------
FMWLocationInfoPanel::FMWLocationInfoPanel(LocationContainer * locationscontainer, QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f) 
{
		ui.setupUi(this);
        setEnabled(false);
        connections();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLocationInfoPanel::show_location(Location)
//      Purpose: muestra la location Location
//      Created: 5/12/07
//
// ------------------
void FMWLocationInfoPanel::show_location(Location loc)
{
    qDebug() << "FMWLocationInfoPanel:show_location: Entrando en la función";
    setEnabled(true);
    ui.lineEdit->setText(loc.get_name().c_str());
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLocationInfoPanel::connections
//      Purpose: Conecta SLOTS con SIGNALS
//      Created: 25/01/08
//
// ------------------

void FMWLocationInfoPanel::connections()
{
	QObject::connect(ui.lineEdit, SIGNAL(editingFinished()), this, SLOT(update_location_name()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FMWLocationInfoPanel::update_location_name
//      Purpose: Updates Location name
//      Created: 25/01/08
//
// ------------------

void FMWLocationInfoPanel::update_location_name()
{
    qDebug() << "FMWLocationInfoPanel:update_location_name: Entrando en la función";
    static_cast<FileManagementWidget*>(this->parent()->parent())->update_location_name(ui.lineEdit->text().toStdString());
}

