/***************************************
 *            SplashWindow.h
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
//      Name:    SplashWindow.h
//      Purpose: Dialogo de Bienvenida
//      Created: 24/10/07
//
// ------------------
#ifndef SPLASHWINDOW_H_
#define SPLASHWINDOW_H_
#include <QtGui>
#include "ConfigurationWizard.h"
#include "KeyWizard.h"
#include "ui_SplashWindow.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    SplashWindow
//      Purpose: Dialogo de Bienvenida
//      Created: 24/10/07
//
// ------------------

class SplashWindow : public QDialog
{
	Q_OBJECT
	public:
	 SplashWindow(ABConfiguration * config, QWidget * parent = 0, Qt::WindowFlags f = 0);
     void showEvent(QShowEvent * event);
     bool bconfig;
     bool bkeys;

    signals:
     void SplashDone();

	private slots:
	 void done();
     void myUpdate();
     void finishing();

	private:
	 Ui::SplashWindow ui;
	 void connections();
     void check_buttons();
	 KeyWizard * keywizard;
	 ConfigurationWizard * configurationwizard;
     ABConfiguration * conf;
	
	
};

#endif /*SPLASHWINDOW_H_*/
