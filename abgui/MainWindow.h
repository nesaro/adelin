/***************************************
 *            MainWindow.h
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
//      Name:    MainWindow.h
//      Purpose: Ventana Principal
//      Created: 23/10/07
//
// ------------------

#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include "BBDaemonInterfaces.h"
#include "BBBInterfaceSocket.h"
#include "ABConfiguration.h"
#include "BBSInterfacePoolThread.h"

#include <QtGui>
#include <QMainWindow>
#include "ui_MainWindow.h"
#include "StatusWidget.h"
#include "FileManagementWindow.h"
#include "FileRestoreWidget.h"
#include "ConfigurationWindow.h"
// --------------------------------------------------------------------------
//
// Class
//      Name:    MainWindow
//      Purpose: Ventana Principal
//      Created: 23/10/07
//
// ------------------

class MainWindow : public QMainWindow
{
	Q_OBJECT

	public:
	 MainWindow(ABConfiguration *,BBBInterfaceSocket *,BBSInterface *);
	 ConfigurationWindow * configwindow;
	 StatusWidget * statuswidget;

    protected:
     virtual void showEvent(QShowEvent * event);

	private slots:
	 void show_about_dialog();
	 void show_warnings();

	private:
	 Ui::MainWindow ui;
     void initPubli();
	 void connections();
     bool non_writable_conf_show;
     bool non_readable_conf_show;
     BBSInterfacePoolThread * bbsipoolthread;
	 FileManagementWindow * filemanagementwindow;
	 FileRestoreWidget * filerestorewidget;
     QToolBar *mainToolBar;
     QAction * configurePaths;
     QAction * configureProgram;
     QAction * aboutProgram;
}; 

#endif /*MAINWINDOW_H_*/
