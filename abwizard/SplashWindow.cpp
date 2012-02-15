/***************************************
 *            SplashWindow.cpp
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
//      Name:    SplashWindow.cpp
//      Purpose: Dialogo de Bienvenida
//      Created: 24/10/07
//
// ------------------

#include "SplashWindow.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    SplashWindow::SplashWindow
//      Purpose: Constructor
//      Created: 24/10/07
//
// ------------------

SplashWindow::SplashWindow(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QDialog(parent,f)
{
    bconfig=bkeys=true;
    conf = config;
    ui.setupUi(this);
    configurationwizard = new ConfigurationWizard(config, this);
    keywizard = new KeyWizard(config, this);
    ui.label_4->setPixmap(QPixmap(":files/adelinbackupintrologo.png"));
    connections();
    check_buttons();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void SplashWindow::connections()
//      Purpose: Conecta SLOTS con SIGNALS
//      Created: 24/10/07
//
// ------------------

void SplashWindow::connections()
{
    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(done()));
    QObject::connect(ui.pushButton_3, SIGNAL(clicked()), configurationwizard, SLOT(show()));
    QObject::connect(ui.pushButton_4, SIGNAL(clicked()), keywizard, SLOT(show()));
    QObject::connect(configurationwizard, SIGNAL(accepted()), this, SLOT(myUpdate())); 
    QObject::connect(keywizard, SIGNAL(accepted()), this, SLOT(finishing())); 
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void SplashWindow::done()
//      Purpose: Emite la señal que indica que el dialogo splash ha terminado bien
//      Created: 24/10/07
//
// ------------------

void SplashWindow::done()
{
    emit SplashDone();
    accept();	
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void SplashWindow::check_buttons()
//      Purpose: Determina en que estado deben estar los tres botones
//      Created: 21/2/08
//
// ------------------

void SplashWindow::check_buttons()
{
    if (conf->bbconf->status()==BBCInterface::CIS_OK && 
            conf->bbconf->intprops["AccountNumber"]->get() != -1)// || conf->bbconf->status()==CIS_NWRITE )
    { bconfig=false; }

    if (conf->bbconf->pathprops.find("TrustedCAsFile") != conf->bbconf->pathprops.end())
    {
        bool ok;
        QString numerousuario;
        numerousuario.setNum(conf->bbconf->intprops["AccountNumber"]->get());
        QFileInfo key1(conf->bbconf->pathprops["TrustedCAsFile"]->get().c_str());
        //QFileInfo key2(QString(conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario.toInt(&ok,16)+QString("-csm.pem")); //TODO: Falta el otro fichero
        if (key1.exists())
            bkeys=false; 
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void SplashWindow::myUpdate()
//      Purpose: Actualiza el estado de los botones. Devuelve falso si todos los botones están desactivados
//      Created: 11/2/08
//
// ------------------

void SplashWindow::myUpdate()
{
    qDebug() << "SplashWindow::myUpdate: Init";
    check_buttons();
    ui.pushButton_3->setEnabled(bconfig);
    ui.pushButton_4->setEnabled(bkeys && !bconfig);
    ui.pushButton->setEnabled(!bkeys && !bconfig);

}

void SplashWindow::finishing()
{
        emit SplashDone();
        QMessageBox::information(0,"Exito","Ha configurado correctamente adelin backup");
        accept();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void SplashWindow::showEvent(QShowEvent * event)
//      Purpose: Redefinición ShowEvent de la clase Padre
//      Created: 14/11/07
//
// ------------------

void SplashWindow::showEvent(QShowEvent * event)
{
    myUpdate();
    QWidget::showEvent(event);
}

