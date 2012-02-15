/***************************************
 *            FileManagementWindow.cpp
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
//      Name:    FileManagementWindow.cpp
//      Purpose: Dialogo de configuración de Location
//      Created: 12/12/07
//
// ------------------

#include "FileManagementWindow.h"

#include <QMessageBox>

// --------------------------------------------------------------------------
//
// Function
//      Name:    FileManagementWindow::FileManagementWindow(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
//      Purpose: Constructor
//      Created: 12/12/07
//
// ------------------


FileManagementWindow::FileManagementWindow(ABConfiguration * config,BBBInterfaceSocket * bbbinterface, QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
		conf = config;
		ui.setupUi(this);
        bbbi = bbbinterface;
        filemanagementwidget = new FileManagementWidget(conf,this);
        lostlocationwindow = new FMWLostLocationWindow(conf);
        ui.stackedWidget->addWidget(filemanagementwidget);
        ui.stackedWidget->setCurrentWidget(filemanagementwidget);
		connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWindow::connections()
//      Purpose: Connections
//      Created: 12/3/08
//
// ------------------


void FileManagementWindow::connections()
{
    connect(ui.pushButton, SIGNAL(clicked()), lostlocationwindow, SLOT(show()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWindow::accept()
//      Purpose: Determina las acciones a realizar cuando el usuario acepta el dialogo
//      Created: 12/12/07
//
// ------------------


void FileManagementWindow::accept()
{
    qDebug() << "FileManagementWindow::accept():" << tr("Begin");
    //FIXME La comparacion siempre es verdadera, porque la configuracion se actualiza
    //0. Comprobar si es posible leer/escribir en la configuración/El FileManagementWidget esta activado
    if (!conf->bb_config_file_writable() || !conf->bb_config_file_readable())
    {
        QMessageBox::information(this, tr("Warning"), tr("Unable to write boxbackup configuration"));
        QDialog::accept();
        return;
    }
    //Comparación innecesaria
    /*
    vector<std::string> widgetfilevector = filemanagementwidget->get_selection();
    QList<QDir> filenamelist1;
    for (vector<std::string>::iterator i = widgetfilevector.begin();
            i!=widgetfilevector.end();
            i++)
    {
        filenamelist1.append(QDir(i->c_str()));
        qDebug() << "FileManagementWidget::accept() Añadido al vector del modelo:"<< i->c_str();
    } 
    qDebug() << "FileManagementWidget::accept():Vector de directorios obtenido del modelo";
    QList<QDir> filenamelist2;
    vector<Location> locationvector = conf->bbconf->mLocations->get_locations_vector();
    for (vector<Location>::iterator i = locationvector.begin();
            i!=locationvector.end();
            i++)
    {
        filenamelist2.append(QDir((*i).get_path().c_str()));
        qDebug() << "FileManagementWidget::accept() Añadido al vector de la configuracion:"<< (*i).get_path().c_str();
    } 
    qDebug() << "FileManagementWidget::accept():Vector de directorios obtenido de la configuracion";
    //TODO: Sort lists
    if (qEqual(filenamelist1.begin(),filenamelist1.end(),filenamelist2.begin())) {QDialog::accept(); return;}
    */
    int rc = QMessageBox::question(this, tr("Question"), tr("Do you want to store current locations?"),QMessageBox::Yes|QMessageBox::No,QMessageBox::No);
    if (rc==QMessageBox::No) {QDialog::accept();return;}
    //Guardar configuracion
    conf->write_bb_config(); 
    bbbi->RequestRestartClient();
    //QMessageBox::information(static_cast<QWidget*>(this->parent()), tr("Atención"), tr("Reinicie BoxBackup para que los cambios que ha realizado tengan efecto"));
    QDialog::accept();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWindow::reject()
//      Purpose: Determina las acciones a realizar cuando el usuario cancela el dialogo
//      Created: 12/12/07
//
// ------------------


void FileManagementWindow::reject()
{
    qDebug() << "FileManagementWindow::reject():" << tr("Begin");
    //Devolver el árbol al estado original de la configuracion configuracion.
    QDialog::reject();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWindow::notify_config_change()
//      Purpose: Determina las acciones a realizar cuando hay un cambio en la configuración
//      Created: 12/12/07
//
// ------------------


void FileManagementWindow::notify_config_change()
{
    //TODO
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FileManagementWindow::showEvent(QShowEvent * event)
//      Purpose: Redefinicion del showEvent de QWidget
//      Created: 13/3/08
//
// ------------------

void FileManagementWindow::showEvent(QShowEvent * event)
{
    if (conf->bbconf->status()!=BBCInterface::CIS_OK)
    {
        ui.pushButton->setEnabled(false);
    }
    else
    {
        ui.pushButton->setEnabled(true);
    }
    QWidget::showEvent(event);
}
