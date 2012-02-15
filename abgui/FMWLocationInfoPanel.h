/***************************************
 *            FMWLocationInfoPanel.h
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
//      Name:    FMWLocationInfoPanel.h
//      Purpose: Widget que muestra la información de una Location dada
//      Created: 30/11/07
//
// ------------------

#ifndef FMWLOCATIONINFOPANEL_H_
#define FMWLOCATIONINFOPANEL_H_


#include "Location.h"
#include "ui_FMWLocationInfoPanel.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FMWLocationInfoPanel
//      Purpose: Widget que muestra la información de una Location dada
//      Created: 30/11/07
//
// ------------------

class FMWLocationInfoPanel : public QWidget
{
	Q_OBJECT
	public:
	 FMWLocationInfoPanel(LocationContainer *, QWidget * parent = 0, Qt::WindowFlags f = 0);
     void show_location(Location);
     //void show_location(std::string location_name);
     std::string show_current_location_name() const { return ui.lineEdit->text().toStdString();}
     void set_current_location_name(std::string name) { ui.lineEdit->setText(name.c_str());}
     void disable_all(){setEnabled(false);ui.lineEdit->setText("");}

    private slots:
     void update_location_name();

	private:
	 Ui::FMWLocationInfoPanel ui;
     void connections();

};

#endif /*FMWLOCATIONINFOPANEL_H_*/

