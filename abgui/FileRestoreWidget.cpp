/***************************************
 *            FileRestoreWidget.cpp
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
//      Name:    FileRestoreWidget.cpp
//      Purpose: Widget que permite recuperar ficheros
//      Created: 31/10/07
//
// ------------------

#include "FileRestoreWidget.h"

#include <QItemSelectionModel>
#include <QModelIndex>
#include <QtDebug>

#include "assert.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    FileRestoreWidget::FileRestoreWidget(QWidget * parent, Qt::WindowFlags f)
//      Purpose: Constructor
//      Created: 31/10/07
//
// ------------------

FileRestoreWidget::FileRestoreWidget(ABConfiguration * config, BBSInterface * bbsi, QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f)
{
        conf = config;
        bbsinterface = bbsi;
		ui.setupUi(this);
        //FileRestorePanel
        filerestorepanel = new FRWFileRestorePanel(conf,bbsi, this);
        ui.stackedWidget->addWidget(filerestorepanel);
        ui.stackedWidget->setCurrentWidget(filerestorepanel);
        model = new FRWLocationModel(bbsinterface,this);
        ui.treeView->setModel(model);
        ui.treeView-> setSelectionMode (QAbstractItemView::SingleSelection);
        connections();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileRestoreWidget::connections
//      Purpose: Conecta SLOTS con SIGNALS
//      Created: 14/12/07
//
// ------------------

void FileRestoreWidget::connections()
{
	QObject::connect(ui.treeView, SIGNAL(pressed(QModelIndex)), this, SLOT(current_item_changed(QModelIndex)));
	QObject::connect(ui.treeView, SIGNAL(expanded(QModelIndex)), model, SLOT(populate_at_index(QModelIndex)));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileRestoreWidget::current_item_changed
//      Purpose: Determina que hacer cuando se ha cambiado el item seleccionado
//      Created: 14/12/07
//
// ------------------

void FileRestoreWidget::current_item_changed(QModelIndex index)
{
    if (index==QModelIndex()) return; //Valor invalido
    BBSNode node = model->get_node(index);
    filerestorepanel->show_node(node); 
    BBSNode * tmpnode = new BBSNode(node);
    filerestorepanel->set_last_selected_item(tmpnode);
}

