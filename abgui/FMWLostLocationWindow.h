/***************************************
 *            FMWLostLocationWindow.h
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
//      Name:    FMWLostLocationWindow.h
//      Purpose: Dialogo de las location que ya no existen en el disco duro local
//      Created: 12/3/08
//
// ------------------
#ifndef FMWLOSTLOCATIONWINDOW_H_
#define FMWLOSTLOCATIONWINDOW_H_

#include "ABConfiguration.h"
#include "Location.h"

#include <QtGui>
#include "ui_FMWLostLocationWindow.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FMWLostLocationWindow
//      Purpose: Dialogo de las location que ya no existen en el disco duro local
//      Created: 12/3/08
//
// ------------------

class FMWLostLocationWindow : public QDialog
{
	Q_OBJECT
	public:
	FMWLostLocationWindow(ABConfiguration * config, QWidget * parent = 0, Qt::WindowFlags f = 0);
    public slots:
        void deleteLocation();
        void restoreLocation();
        void restoreLocation(std::string);
	private:
	 Ui::FMWLostLocationWindow ui;
     void updateEntries();
	 void connections();
	 ABConfiguration * conf;
     LocationContainer * locations;
	
};


#endif /*FMWLOSTLOCATIONWINDOW_H_*/
