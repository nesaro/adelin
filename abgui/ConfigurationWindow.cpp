/***************************************
 *            ConfigurationWindow.cpp
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
//      Name:    ConfigurationWindow.cpp
//      Purpose: Dialogo de configuración
//      Created: 24/10/07
//
// ------------------

#include "ConfigurationWindow.h"
#include <string>

// --------------------------------------------------------------------------
//
// Function
//      Name:    ConfigurationWindow::ConfigurationWindow
//      Purpose: Constructor
//      Created: 24/10/07
//
// ------------------

ConfigurationWindow::ConfigurationWindow(ABConfiguration * config, BBBInterfaceSocket * bbbinterface, QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
		conf = config;
        bbbi = bbbinterface;
		ui.setupUi(this);
        myUpdate();
		connections();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::notify_config_change
//      Purpose: Acciones a realizar cuando cambia la configuración
//      Created: 25/10/07
//
// ------------------

void ConfigurationWindow::notify_config_change()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::connections
//      Purpose: Realiza las conexiones SLOT SIGNAL
//      Created: 26/10/07
//
// ------------------

void ConfigurationWindow::connections()
{
	QObject::connect(this, SIGNAL(accepted()), this, SLOT(store_all_props()));
	QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(load_config_path()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::store_all_props
//      Purpose: Guarda los valores en la configuración (en su respectivas Property), y luego llama a guardar en fichero
//      Created: 26/10/07
//
// ------------------

void ConfigurationWindow::store_all_props()
{
	std::string tmp;
    bool writable, ok=true; //TODO: Excepción en caso de error
    // Variables de ABConfiguration
    conf->set_bb_keys_dirname(ui.lineEdit_20->text().toStdString());
    conf->set_bb_binary_filename(ui.lineEdit_10->text().toStdString());
    conf->set_bb_data_dirname(ui.lineEdit_21->text().toStdString());
    conf->set_openssl_filename(ui.lineEdit_22->text().toStdString());

    // Variables de BBConfig
    writable = conf->write_bb_config(); //Verifica si es posible escribir //TODO Preferible Comprobación sin escribir
    if (!writable)
    {
        QMessageBox::information(static_cast<QWidget*>(this->parent()), tr("Information"), tr("Unable to write bbackupd.conf"));
        load_all_props();
        return;
    }
	tmp="AccountNumber";
	conf->bbconf->intprops[tmp]->set(ui.lineEdit->text().toInt(&ok,16));
	tmp="StoreHostname";
	conf->bbconf->stringprops[tmp]->set(ui.lineEdit_11->text().toStdString());
    tmp="FileTrackingSizeThreshold";
	conf->bbconf->intprops[tmp]->set(ui.lineEdit_5->text().toInt(&ok));
    tmp="UpdateStoreInterval";
	conf->bbconf->intprops[tmp]->set(ui.lineEdit_2->text().toInt(&ok));
    conf->write_bb_config(); 
    bbbi->RequestRestartClient();
    //QMessageBox::information(static_cast<QWidget*>(this->parent()), tr("Atención"), tr("Reinicie BoxBackup para que los cambios que ha realizado tengan efecto"));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::load_all_props
//      Purpose: Carga los valores de los elementos del objeto Configuration
//      Created: 30/11/07
//
// ------------------

void ConfigurationWindow::load_all_props()
{
    QString tmp;
    tmp.setNum(conf->bbconf->intprops["AccountNumber"]->get(),16);
    ui.lineEdit->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["UpdateStoreInterval"]->get());
    ui.lineEdit_2->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["MinimumFileAge"]->get());
    ui.lineEdit_3->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["MaxUploadWait"]->get());
    ui.lineEdit_4->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["FileTrackingSizeThreshold"]->get());
    ui.lineEdit_5->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["DiffingUploadSizeThreshold"]->get());
    ui.lineEdit_6->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["MaximumDiffingTime"]->get());
    ui.lineEdit_7->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["MaxFileTimeInFuture"]->get());
    ui.lineEdit_8->setText(tmp);
    tmp.setNum(conf->bbconf->intprops["KeepAliveTime"]->get());
    ui.lineEdit_9->setText(tmp);
    tmp = QString(conf->bbconf->stringprops["StoreHostname"]->get().c_str());
    ui.lineEdit_11->setText(tmp);
    ui.label_14->setText(conf->bbconf->pathprops["PrivateKeyFile"]->get().c_str());
    ui.label_15->setText(conf->bbconf->pathprops["KeysFile"]->get().c_str());
    ui.label_16->setText(conf->bbconf->pathprops["TrustedCAsFile"]->get().c_str());
    ui.label_17->setText(conf->bbconf->pathprops["CertificateFile"]->get().c_str());
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::enable_widgets()
//      Purpose: Enables all widgets
//      Created: 2/1/08
//
// ------------------

void ConfigurationWindow::enable_widgets()
{
    ui.groupBox->setEnabled(true);
    ui.groupBox_2->setEnabled(true);
    ui.lineEdit_2->setEnabled(true);
    ui.lineEdit_3->setEnabled(true);
    ui.lineEdit_4->setEnabled(true);
    ui.lineEdit_5->setEnabled(true);
    ui.lineEdit_6->setEnabled(true);
    ui.lineEdit_7->setEnabled(true);
    ui.lineEdit_8->setEnabled(true);
    ui.lineEdit_9->setEnabled(true);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::disable_widgets()
//      Purpose: Disables all but ConfigurationLocation Label
//      Created: 2/1/08
//
// ------------------

void ConfigurationWindow::disable_widgets()
{
    ui.groupBox->setEnabled(false);
    ui.groupBox_2->setEnabled(false);
    ui.lineEdit_2->setEnabled(false);
    ui.lineEdit_3->setEnabled(false);
    ui.lineEdit_4->setEnabled(false);
    ui.lineEdit_5->setEnabled(false);
    ui.lineEdit_6->setEnabled(false);
    ui.lineEdit_7->setEnabled(false);
    ui.lineEdit_8->setEnabled(false);
    ui.lineEdit_9->setEnabled(false);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::myUpdate()
//      Purpose: Performs an update
//      Created: 2/1/08
//
// ------------------

void ConfigurationWindow::myUpdate()
{
    ui.lineEdit_19->setText(conf->get_bb_config_filename().c_str());
    ui.lineEdit_20->setText(conf->get_bb_keys_dirname().c_str());
    ui.lineEdit_21->setText(conf->get_bb_data_dirname().c_str());
    ui.lineEdit_22->setText(conf->get_openssl_filename().c_str());
    ui.lineEdit_10->setText(conf->get_bb_binary_filename().c_str());
    if (conf->bbconf->status()!=BBCInterface::CIS_OK)
    {
        disable_widgets();
        return;
    }
    enable_widgets();
    load_all_props();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ConfigurationWindow::load_config_path()
//      Purpose: Performs an attemp to load user path. Slot button
//      Created: 2/1/08
//
// ------------------

void ConfigurationWindow::load_config_path()
{
    qDebug() << "ConfigurationWindow::load_config_path: Begin";
    //TODO
    //Compare path with textlabel
    if (ui.lineEdit_19->text().toStdString()==conf->get_bb_config_filename())
    {return;}
    //if diff:
    //Save path from textlabel
    conf->set_bb_config_filename(ui.lineEdit_19->text().toStdString());
    //Tell BBC to load new path
    conf->load_bb_config();
    //If status == ok, enable_widgets and load_all_props
    if (conf->bbconf->status()==BBCInterface::CIS_OK)
    {
        enable_widgets();
        load_all_props();
    }
    //else: disable_widgets
    else
    {
        disable_widgets();
    }
}

