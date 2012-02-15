/***************************************
 *            TrayIconHandler.h
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdserver.
 *   abbsdserver is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdserver is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdserver.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    TrayIconHandler.h
//      Purpose: Manejador del trayicon
//      Created: 3/3/08
//
// ------------------


#ifndef TRAYICONHANDLER_H_
#define TRAYICONHANDLER_H_
#include <QSystemTrayIcon>
#include <QAction>
#include <QMenu>
#include <QComboBox>
#include "LogWindow.h"
#include "InstanceSelectorWindow.h"
//#include "publiAPI/AppPubliHTMLnonGUI.h"
#include "ABBSDStatus.h"


class TrayIconHandler: public QObject
{
    Q_OBJECT
    public:
        TrayIconHandler(LogWindow *, ABBSDStatus *, BBInterfaceStatus *);
        ~TrayIconHandler();
    public slots:
        void handle_user_msg(QString);
        void showAd(QString);
        void launchInterface();
    private slots:
        void quitApp();
    private:
        QSystemTrayIcon * trayicon;
        QAction * showlogwindowAction;
        QAction * showinstancewindowAction;
        QAction * launchInterfaceAction;
        QAction * quitAction;
        QMenu *trayIconMenu;
        void createActions();
        void createTrayIcon();
        QComboBox *typeComboBox;
        QSystemTrayIcon::MessageIcon icon; 
        LogWindow * logwindow;
        InstanceSelectorWindow * instancewindow;
        //AppPubliHTMLnonGUI * apppubli;
        ABBSDStatus * abbsdstatus;
        BBInterfaceStatus * bbis;
        QString lastad ;
        
};

#endif /*TRAYICONHANDLER_H_*/
