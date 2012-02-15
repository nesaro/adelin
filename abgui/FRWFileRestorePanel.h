/***************************************
 *            FRWFileRestorePanel.h
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
//      Name:    FRWFileRestorePanel.h
//      Purpose: Widget que muestra las acciones que se puede realizar con un fichero en el FileRestorePanel
//      Created: 30/11/07
//
// ------------------

#ifndef FRWFILERESTOREPANEL_H_
#define FRWFILERESTOREPANEL_H_


#include "ABConfiguration.h"
#include "BBSInterface.h"
#include "BBSNode.h"

#include "ui_FRWFileRestorePanel.h"

#include "assert.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FRWFileRestorePanel
//      Purpose: Widget que muestra las acciones que se puede realizar con un fichero en el FileRestorePanel
//      Created: 30/11/07
//
// ------------------

class FRWFileRestorePanel : public QWidget
{
	Q_OBJECT
	public:
	 FRWFileRestorePanel(ABConfiguration *, BBSInterface *, QWidget * parent = 0, Qt::WindowFlags f = 0);
     void show_node(BBSNode node);
     void show_file(int64_t id);
     void show_dir(int64_t id);
     void show_location(int64_t id);
     void set_last_selected_item(BBSNode * node){assert(node!=NULL); last_selected_item = node;}

	private slots:	
     void restore_element();
     void delete_element();

	private:
	 Ui::FRWFileRestorePanel ui;
	 void connections();
     ABConfiguration * conf;
     BBSInterface * bbsinterface;
     BBSNode * last_selected_item;

};

#endif /*FRWFILERESTOREPANEL_H_*/

