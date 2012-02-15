/***************************************
 *            InstanceSelectorWindow.cpp
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdclient.
 *   abbsdclient is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdclient is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdclient.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    InstanceSelectorWindow.cpp
//      Purpose: Ventana que muestra los logs
//      Created: 3/3/08
//
// ------------------


#include "InstanceSelectorWindow.h"
#include <QtDebug>

InstanceSelectorWindow::InstanceSelectorWindow(BBInterfaceStatus * mybbis)
{
    qDebug() << "InstanceSelectorWindow::InstanceSelectorWindow: Begin";
    ui.setupUi(this);
    bbis = mybbis;
    connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void InstanceSelectorWindow::connections
//      Purpose: Realiza las conexiones SLOT SIGNAL
//      Created: 6/3/08
//
// ------------------

void InstanceSelectorWindow::connections()
{
	QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(hide()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(updateInstanceList()));
//	QObject::connect(eh, SIGNAL(layout_changed()), this, SLOT(refresh()));
}


bool InstanceSelectorWindow::appendInstancetoList(BBInstance instance)
{
    qDebug() << "InstanceSelectorWindow::appendInstancetoList: Begin";
    //Aumentar en 1 el numero de filas
    ui.tableWidget->insertRow(ui.tableWidget->rowCount());
    //Introducir el nombre de usuario
    ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,0,new QTableWidgetItem(instance.getUserName())); //TODO Obtener de instance
    //Introducir la ruta de configuracion
    ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,1,new QTableWidgetItem(instance.getConfigPath().absoluteFilePath())); //TODO Obtener de instance
    //Introducir PID
    QString instancepid;
//    instancepid.setNum(ProcessControl::getInstancePID(instance)); //TODO: Descomentar si se implementa
    ui.tableWidget->setItem(ui.tableWidget->rowCount()-1,2,new QTableWidgetItem(instancepid)); //TODO Obtener de instance
    return true;
}

void InstanceSelectorWindow::updateInstanceList()
{
    qDebug() << "InstanceSelectorWindow::updateInstanceList: Begin";
    //Limpiar la tabla
    ui.tableWidget->clearContents();
    for (int i=ui.tableWidget->rowCount(); i>0 ; i--)
        ui.tableWidget->removeRow(i-1);
    //Obtener lista de instancias en ejecucion
    std::list<BBInstance> listainstancias = bbis->allInstances();
    //llamar a appendInstancetoList para cada instancia
    for (std::list<BBInstance>::const_iterator i = listainstancias.begin();
            i!=listainstancias.end(); i++)
    {
        qDebug() << "InstanceSelectorWindow::updateInstanceList: Adding instance";
        appendInstancetoList(*i);
    }
    //TODO Marcar de alguna manera la instancia actual(válida)
}
