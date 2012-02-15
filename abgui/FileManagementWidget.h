/***************************************
 *            FileManagementWidget.h
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
//      Name:    FileManagementWidget.h
//      Purpose: Widget que permite gestionar sobre que ficheros se realizará backup
//      Created: 31/10/07
//
// ------------------

#ifndef FILEMANAGEMENTWIDGET_H_
#define FILEMANAGEMENTWIDGET_H_

#include "ABConfiguration.h"
#include "FMWLocationInfoPanel.h"

#include <QTreeView>
#include "ui_FileManagementWidget.h"
#include "CheckBoxDirModel.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FileManagementWidget
//      Purpose: Widget que permite gestionar sobre que ficheros se realizará backup
//      Created: 31/10/07
//
// ------------------

class FileManagementWidget : public QWidget, public BBCInterfaceListener
{
	Q_OBJECT
	public:
	 FileManagementWidget(ABConfiguration *, QWidget * parent = 0, Qt::WindowFlags f = 0);
	 ~FileManagementWidget();
     void showEvent( QShowEvent *);
     void notify_config_change();
     std::vector <std::string> get_selection()const {return model->get_dir_selection();} 

    public slots:
     void update_location_name(std::string name);

	private slots:	
     void resizeFilenameColumn(){ui.treeView->resizeColumnToContents(0);}
     void current_item_selected(QModelIndex);
     void current_selection_changed();

	private:
     void update_tree();
	 void connections();
	 Ui::FileManagementWidget ui;
	 ABConfiguration * conf;
     CheckBoxDirModel * model;
     FMWLocationInfoPanel * locationpanel;
	
};

#endif /*FILEMANAGEMENTWIDGET_H_*/


