/***************************************
 *            FileManagementWindow.h
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
//      Name:    FileManagementWindow.h
//      Purpose: Dialogo de configuración de Locations
//      Created: 12/12/07
//
// ------------------
#ifndef FILEMANAGEMENTWINDOW_H_
#define FILEMANAGEMENTWINDOW_H_

#include "ABConfiguration.h"
#include "FileManagementWidget.h"
#include "BBBInterfaceSocket.h"

#include <QtGui>
#include "ui_FileManagementWindow.h"
#include "FMWLostLocationWindow.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FileManagementWindow 
//      Purpose: Dialogo de configuración de Locations
//      Created: 24/10/07
//
// ------------------

class FileManagementWindow : public QDialog, public BBCInterfaceListener
{
	Q_OBJECT
	public:
	FileManagementWindow(ABConfiguration * config, BBBInterfaceSocket * bbbinterface, QWidget * parent = 0, Qt::WindowFlags f = 0);
	void notify_config_change(); //required - BBCInterfaceListener

	private slots:
	 void accept();
	 void reject();

    protected:
     void showEvent(QShowEvent *);

	private:
	 Ui::FileManagementWindow ui;
     void connections();
	 ABConfiguration * conf;
     BBBInterfaceSocket * bbbi;
     FileManagementWidget * filemanagementwidget;
     FMWLostLocationWindow * lostlocationwindow;
	
};


#endif /*FILEMANAGEMENTWINDOW_H_*/
