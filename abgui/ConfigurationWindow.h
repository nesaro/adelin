/***************************************
 *            ConfigurationWindow.h
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
//      Name:    ConfigurationWindow.h
//      Purpose: Dialogo de configuración
//      Created: 24/10/07
//
// ------------------
#ifndef CONFIGURATIONWINDOW_H_
#define CONFIGURATIONWINDOW_H_
#include "Listeners.h"

#include "ABConfiguration.h"
#include "BBBInterfaceSocket.h"

#include <QtGui>
#include "ui_ConfigurationWindow.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    ConfigurationWindow
//      Purpose: Dialogo de configuración
//      Created: 24/10/07
//
// ------------------

class ConfigurationWindow : public QDialog, public BBCInterfaceListener
{
	Q_OBJECT
	public:
	ConfigurationWindow(ABConfiguration * config, BBBInterfaceSocket * bbbinterface, QWidget * parent = 0, Qt::WindowFlags f = 0);
	void notify_config_change(); //required - BBCInterfaceListener

	private slots:
	 void store_all_props();
     void load_config_path();

	private:
	 Ui::ConfigurationWindow ui;
	 void connections();
     void myUpdate();
     void load_all_props();
     void disable_widgets();
     void enable_widgets();
	 ABConfiguration * conf;
     BBBInterfaceSocket * bbbi;
	
};


#endif /*CONFIGURATIONWINDOW_H_*/
