/***************************************
 *            TrayIconHandler.cpp
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
//      Name:    TrayIconHandler.cpp
//      Purpose: Gestión del system Tray
//      Created: 3/3/08
//
// ------------------

#include "TrayIconHandler.h"
#include "AB.h"
// --------------------------------------------------------------------------
//
// Function
//      Name:    TrayIconHandler::TrayIconHandler()
//      Purpose: Constructor
//      Created: 3/3/08
//
// ------------------
TrayIconHandler::TrayIconHandler(LogWindow * mylogwindow, ABBSDStatus * mystatus, BBInterfaceStatus * mybbis)
{
    logwindow = mylogwindow;
    instancewindow = new InstanceSelectorWindow(mybbis);
    createActions();
    createTrayIcon();
    trayicon->show();
    typeComboBox = new QComboBox;
    icon = QSystemTrayIcon::MessageIcon( typeComboBox->itemData(typeComboBox->currentIndex()).toInt());
    //apppubli = myapppubli;
    //connect(apppubli, SIGNAL(TextChanged(QString)), this, SLOT(showAd(QString)));
    //QTimer *timer = new QTimer(this);
    //connect(timer, SIGNAL(timeout()), apppubli, SLOT(requestAd()));
    //timer->start(10009);
    abbsdstatus = mystatus;
    bbis = mybbis;
    lastad = "Adelin Publicidad";
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    TrayIconHandler::~TrayIconHandler()
//      Purpose: Destructor
//      Created: 3/3/08
//
// ------------------
TrayIconHandler::~TrayIconHandler()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    TrayIconHandler::createActions()
//      Purpose: crea las acciones del menu
//      Created: 3/3/08
//
// ------------------
void TrayIconHandler::createActions()
{
    showlogwindowAction = new QAction(tr("Show Log Window"), this);
    connect(showlogwindowAction, SIGNAL(triggered()), logwindow, SLOT(show()));
    showinstancewindowAction = new QAction(tr("Show instances Window"), this);
    connect(showinstancewindowAction, SIGNAL(triggered()), instancewindow, SLOT(show()));
    launchInterfaceAction = new QAction(tr("Launch Interface"), this);
    connect(launchInterfaceAction, SIGNAL(triggered()), this, SLOT(launchInterface()));
    quitAction = new QAction(tr("Quit"), this);
    connect(quitAction, SIGNAL(triggered()), this, SLOT(quitApp()));

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    TrayIconHandler::createTrayIcon()
//      Purpose: crea el icono de sistema
//      Created: 3/3/08
//
// ------------------
void TrayIconHandler::createTrayIcon()
{
    trayIconMenu = new QMenu();
    //trayIconMenu->addAction(showlogwindowAction);
    //trayIconMenu->addAction(showinstancewindowAction);
    trayIconMenu->addAction(launchInterfaceAction);
    trayIconMenu->addAction(quitAction);
    trayicon = new QSystemTrayIcon(this);
    trayicon->setContextMenu(trayIconMenu);
    trayicon->setIcon(QIcon(":/files/trayicon.png"));

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void TrayIconHandler::show_message(QString)
//      Purpose: Muestra la ventana de logs
//      Created: 3/3/08
//
// ------------------
void TrayIconHandler::handle_user_msg(QString message)
{
    QString publi;
    int random_integer = rand(); 
    if (random_integer>0)
    {
        publi += lastad;
    }
    trayicon->showMessage(message, publi , icon, 2400);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void TrayIconHandler::showAd(QString)
//      Purpose: Muestra un anuncio
//      Created: 18/6/08
//
// ------------------
void TrayIconHandler::showAd(QString message)
{
    //FIXME: Realmente solo guarda el anuncio para que sea mostrado junto a los mensajes de evento
    lastad = message;
//    trayicon->showMessage(message, "Anuncio por Adelin", icon, 2400);
}


void TrayIconHandler::quitApp()
{
    //TODO: Preguntar por si el usuario esta seguro bla bla bla bla
    qApp->quit(); 

}

void TrayIconHandler::launchInterface()
{
    if(!QProcess::startDetached(AB::system_ab_interface_default_location))
    {
        QMessageBox::warning(0,QString("Atención"),QString("No se ha podido lanzar la interfaz"));
    } //TODO: Registrarlo en la configuración y lanzarlo
}
