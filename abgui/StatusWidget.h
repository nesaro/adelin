/***************************************
 *            StatusWidget.h
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
//      Name:    StatusWidget.h
//      Purpose: Widget que representa el estado en la ventana principal
//      Created: 29/10/07
//
// ------------------

#ifndef STATUSWIDGET_H_
#define STATUSWIDGET_H_

#include "BBBInterfaceSocket.h"
#include "ABConfiguration.h"
#include "BBSInterface.h"

#include <QShowEvent>
#include "ui_StatusWidget.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    StatusWidget
//      Purpose: Widget que representa el estado en la ventana principal
//      Created: 29/10/07
//
// ------------------

class StatusWidget : public QWidget
{
	Q_OBJECT
	public:
	 StatusWidget(BBBInterfaceSocket * bc, ABConfiguration * conf, QWidget * parent = 0, Qt::WindowFlags f = 0);
    public slots:
     void update_progress_bar(int);
     void set_server_status(BBSInterface::BBSStatus);
     void slot_bbb_interface_state_change();
    protected:
     void showEvent(QShowEvent * event);
	private:
	 Ui::StatusWidget ui;
     BBBInterfaceSocket * bbbi;
     ABConfiguration * conf;
     void myUpdate();

    //Herencia BBBInterfaceListener
     void notify_bbb_error();
     void notify_bbb_client_state_change();
	
};

#endif /*STATUSWIDGET_H_*/
