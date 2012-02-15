/***************************************
 *            UserInterface.cpp
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
//      Name:    UserInterface.cpp
//      Purpose: Interfaz de usuario
//      Created: 23/10/07
//
// ------------------
 
#include "UserInterface.h"
#include <QApplication>

// --------------------------------------------------------------------------
//
// Function
//      Name:    UserInterface::UserInterface(QApplication*, int, char *[], ABConfiguration *, BBBInterfaceSocket *, BBSInterface *)
//      Purpose: Constructor
//      Created: 23/10/07
//
// ------------------

UserInterface::UserInterface(QApplication * application, int argc, char *argv[], ABConfiguration * conf,BBBInterfaceSocket * bbbi, BBSInterface * bbbs)
{
    app = application;
	mainwindow = new MainWindow(conf, bbbi, bbbs) ;
    //bbbi->set_listener(mainwindow->statuswidget);
    app->connect(bbbi, SIGNAL(bbb_interface_state_change()), mainwindow->statuswidget, SLOT(slot_bbb_interface_state_change()));
	
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int UserInterface::loop()
//      Purpose: Lanza el bucle de la interfaz de usuario
//      Created: 23/10/07
//
// ------------------

int UserInterface::loop()
{
    	
    mainwindow->show(); 
	return app->exec();
}
