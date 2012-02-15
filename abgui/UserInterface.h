/***************************************
 *            UserInterface.h
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
//      Name:    UserInterface.h
//      Purpose: Interfaz de usuario
//      Created: 23/10/07
//
// ------------------

#ifndef USERINTERFACE_H_
#define USERINTERFACE_H_

#include "BBDaemonInterfaces.h"
#include "BBBInterfaceSocket.h"
#include "ABConfiguration.h"

#include <QApplication>
#include "MainWindow.h"
// --------------------------------------------------------------------------
//
// Class
//      Name:    UserInterface
//      Purpose: Clase que contiene la interfaz de usuario
//      Created: 23/10/07
//
// ------------------

class UserInterface
{
	public:
	QApplication * app;
	UserInterface(QApplication *, int argc, char *argv[], ABConfiguration *, BBBInterfaceSocket *, BBSInterface *);
	int loop();
	MainWindow * mainwindow;
};

#endif /*USERINTERFACE_H_*/
