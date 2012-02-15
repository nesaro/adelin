/***************************************
 *            FileManagementWidget.cpp
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

#define GLOBAL_MAX_LOCATIONS 32 //Debe desaparecer o moverse a BOS.h

// --------------------------------------------------------------------------
//
// File
//      Name:    FileManagementWidget.cpp
//      Purpose: Widget que permite gestionar sobre que ficheros se realizará backup
//      Created: 31/10/07
//
// ------------------

#include "FMWLocationInfoPanel.h"
#include "FileManagementWidget.h"

#include <QtDebug>

#include <vector>
#include <string>
// --------------------------------------------------------------------------
//
// Function
//      Name:    FileManagementWidget::FileManagementWidget(ABConfiguration *, QWidget *, Qt::WindowFlags)
//      Purpose: Constructor
//      Created: 31/10/07
//
// ------------------

FileManagementWidget::FileManagementWidget(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f)
{
    qDebug() << "FileManagementWidget::FileManagementWidget:" << tr("Begin");
		ui.setupUi(this);
        model = new CheckBoxDirModel;
        ui.treeView->setModel(model);
        ui.treeView->setColumnWidth(0,150);
        ui.treeView->setColumnWidth(1,50);
        ui.treeView->setColumnWidth(2,50);
        conf = config;
        //LocationInfoPanel
        locationpanel = new FMWLocationInfoPanel(conf->bbconf->mLocations, this);
        ui.stackedWidget->addWidget(locationpanel);
        ui.stackedWidget->setCurrentWidget(locationpanel);
        connections();
        //FIXME: update_tree de prueba, borrar
        update_tree();
}

FileManagementWidget::~FileManagementWidget()
{
    delete locationpanel;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::update_tree
//      Purpose: Actualiza el árbol con las rutas seleccionadas para backup
//      Created: 28/11/07
//
// ------------------

void FileManagementWidget::update_tree()
{
   //Obtener la lista de Locations de la configuración
   //Para cada elemento de la lista, marcar casilla en el árbol 
    std::vector<Location> locations = conf->bbconf->mLocations->get_locations_vector();
    std::vector<std::string> paths(GLOBAL_MAX_LOCATIONS); //FIXME.. Por que limitar?
    std::vector<Location>::const_iterator theiterator;
    theiterator = locations.begin();
    int i=0;
    while(theiterator != locations.end())
    {
        const Location * tmp = &(*theiterator);
        paths[i] = tmp->get_path();
        theiterator++;
        i++;
    }
    model->setSelection(paths);

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::notify_config_change
//      Purpose: Determina que hacer cuando hay un cambio en la configuración
//      Created: 28/11/07
//
// ------------------

void FileManagementWidget::notify_config_change()
{
    update_tree();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::connections
//      Purpose: Conecta SLOTS con SIGNALS
//      Created: 5/12/07
//
// ------------------

void FileManagementWidget::connections()
{
	QObject::connect(ui.treeView, SIGNAL(pressed(QModelIndex)), this, SLOT(current_item_selected(QModelIndex)));
	QObject::connect(ui.treeView, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeFilenameColumn()));
	QObject::connect(model, SIGNAL(selectionChanged()), this, SLOT(current_selection_changed()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::current_item_selected
//      Purpose: Determina que hacer cuando se hacambiado el item seleccionado
//      Created: 5/12/07
//
// ------------------

void FileManagementWidget::current_item_selected(QModelIndex index)
{
    qDebug() << "FileManagementWidget::current_item_selected: Comienzo de función";
    std::string dir1 = model->dir_from_modelindex(index).path().toStdString();
    qDebug() << "FileManagementWidget::current_item_selected: Ruta del elemento actual:"<< dir1.c_str();
    Location mylocation = conf->bbconf->mLocations->find_by_path(dir1);
    if(mylocation!=Location())
    {
        locationpanel->show_location(mylocation);
        return; 
    }
    qDebug() << "FileManagementWidget::current_item_selected: Error: Location not found";
    locationpanel->disable_all();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::current_selection_changed
//      Purpose: Determina que hacer cuando ha habido con
//      Created: 8/1/08
//
// ------------------

void FileManagementWidget::current_selection_changed()
{
    qDebug() << "FileManagementWidget::current_selection_changed: Comienzo de función";
    //1. Obtener el item actual
    QModelIndex currentindex = ui.treeView->currentIndex();
    if (currentindex == QModelIndex()) return;
    if (!model->isDir(currentindex)) return;
    //2. Verificar su estado
    //3. Si esta chequeado, crear la location y llamar a current_item_selected
    if (model->is_checked(currentindex))
    {
        qDebug() << "FileManagementWidget::current_selection_changed: Item Seleccionado";
        //El elemento debe no existir en la lista
        //assert((location_name_list.find(model->dir_from_modelindex(currentindex).path().toStdString())) == location_name_list.end());
        std::string locationname;
        if (locationpanel->show_current_location_name() == std::string())
        {
            locationname = "NombreTemporal FIXME";
            locationpanel->set_current_location_name(locationname);
        }
        else
        {
            locationname = locationpanel->show_current_location_name();
        }
        if (!conf->bbconf->mLocations->append_location(locationname,model->dir_from_modelindex(currentindex).path().toStdString()))
        {
            qDebug() << "FileManagementWidget::current_selection_changed: Error appending Location";
        }
        else
        {
            locationpanel->setEnabled(true);
        }
    }
    //4. Si no lo está, borrar la entrada de location_name_list
    else
    {
        qDebug() << "FileManagementWidget::current_selection_changed: Item desseleccionado";
        conf->bbconf->mLocations->delete_location_by_path(model->dir_from_modelindex(currentindex).path().toStdString());
        locationpanel->disable_all();
    }


}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::showEvent(QShowEvent * event)
//      Purpose: Redefinicion del showEvent de QWidget
//      Created: 5/12/07
//
// ------------------

void FileManagementWidget::showEvent(QShowEvent * event)
{
    if (conf->bbconf->status()!=BBCInterface::CIS_OK)
    {
        ui.treeView->setEnabled(false);
    }
    else
    {
        ui.treeView->setEnabled(true);
    }
    QWidget::showEvent(event);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWidget::update_location_name(std::string path, std::string name)
//      Purpose: Update Location name
//      Created: 25/1/08
//
// ------------------

void FileManagementWidget::update_location_name(std::string name)
{
    qDebug() << "FileManagementWidget::update_location_name: Comienzo de función";
    QModelIndex currentindex = ui.treeView->currentIndex();
    if (currentindex == QModelIndex()) return;
    if (!model->isDir(currentindex)) return;
    std::string path = model->dir_from_modelindex(currentindex).path().toStdString();
    Location tmplocation = conf->bbconf->mLocations->find_by_path(path);
    assert(tmplocation!=Location());
    assert(conf->bbconf->mLocations->delete_location_by_path(path));
    tmplocation.set_name(name);
    conf->bbconf->mLocations->append_location(tmplocation);
}
