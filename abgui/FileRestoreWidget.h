/***************************************
 *            FileRestoreWidget.h
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
//      Name:    FileRestoreWidget.h
//      Purpose: Widget que permite recuperar ficheros
//      Created: 31/10/07
//
// ------------------

#ifndef FILERESTOREWIDGET_H_
#define FILERESTOREWIDGET_H_

#include "ABConfiguration.h"
#include "BBSInterface.h"

#include "ui_FileRestoreWidget.h"
#include "FRWFileRestorePanel.h"
#include "FRWLocationModel.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FileRestoreWidget
//      Purpose: Widget que permite recuperar ficheros
//      Created: 31/10/07
//
// ------------------

class FileRestoreWidget : public QWidget
{
	Q_OBJECT
	public:
	 FileRestoreWidget(ABConfiguration *, BBSInterface *, QWidget * parent = 0, Qt::WindowFlags f = 0);

    public slots:
     void activate(bool on){
         if(on) {
             filerestorepanel->setEnabled(true);
             model->setupModelData();
             ui.treeView->showColumn(1);
         } 
         else {filerestorepanel->setEnabled(false);}}

	private slots:	
     void current_item_changed(QModelIndex);

	private:
	 Ui::FileRestoreWidget ui;
	 void connections();
     BBSInterface * bbsinterface;
     FRWFileRestorePanel * filerestorepanel;
     FRWLocationModel * model;
     ABConfiguration * conf;
	
};


#endif /*FILERESTOREWIDGET_H_*/
