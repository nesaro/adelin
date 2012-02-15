/***************************************
 *            main.cpp
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
//      Name:    main.cpp
//      Purpose: Programa principal
//      Created: 3/3/08
//
// ------------------

#include <QApplication>
#include "BBLogHandler.h"
#include "EventHandler.h"
#include "BBInterfaceStatus.h"
#include "TrayIconHandler.h"
#include "LogWindow.h"
#include "ABBSDClientListener.h"
#include "ABConfiguration.h"
#include "ABBSDStatus.h"
#include "SingleApplication.h"
#include "ServiceRegistrationHelper.h"

void destructor(BBInterfaceStatus * bbis, EventHandler * eh)
{
    //ABBSDServer Destructor:
    if (bbis!=NULL) bbis->haltCurrentInstance();
    if (eh!=NULL) delete eh;
    if (bbis!=NULL) delete bbis;
}

int main(int argc, char * argv[])
{
    //ABBSDServer Constructor:
    BBInterfaceStatus * bbis = NULL;
    EventHandler * eh = NULL;
    SingleApplication * app = new SingleApplication(argc, argv, "AdelinBackupTray");
    if (app->isRunning())
    {
        //    app.sendMessage("message from other instance.");
        qDebug() << "Already Running";
        delete(app);
        return -1; 
    }
    qRegisterMetaType<Event>("Event"); //Permite usar los event como objetos a pasar en Conexiones Qt
    ABConfiguration * conf = new ABConfiguration();
    //BBLogHandler * bblh;
    eh = new EventHandler();
    ABBSDClientListener * abbsdcl = new ABBSDClientListener(eh);
    LogWindow * logwindow = new LogWindow(eh);
    bbis = new BBInterfaceStatus(conf);
    ABBSDStatus * abbsdstatus = new ABBSDStatus(bbis);
    TrayIconHandler * tih = new TrayIconHandler(logwindow, abbsdstatus, bbis);
    ServiceRegistrationHelper * srh = new ServiceRegistrationHelper("ABTray","Backup Tray Agent", "/usr/bin/abtray"); //FIXME: Store in constants file
    app->connect(eh, SIGNAL(user_msg(QString)), tih, SLOT(handle_user_msg(QString)));
    app->connect(eh, SIGNAL(registered_event(Event)), logwindow, SLOT(handle_registered_event(Event)));
    app->setQuitOnLastWindowClosed(false);
    app->connect(bbis, SIGNAL(new_event(Event)), eh, SLOT(handle_new_event(Event)));
    //app->connect(sessionapiinterface, SIGNAL(new_event(Event)), eh, SLOT(handle_new_event(Event)));
    eh->loadXML();

    bool result;

    //ABBSDServer mainloop

    //if (!QProcess::startDetached(AB::system_publiclient_binary_location,QStringList()))
    //{
    //    qDebug() << "ABBSDServer::mainloop: no se pudo lanzar el asistente de publicidad" << AB::system_publiclient_binary_location;
    //    return -1;
    //}
    bool comienzocorrecto = bbis->beginProcedure();
    if (!comienzocorrecto) 
    {
        if (!conf->installation_procedure())
        {
            qDebug() << "ABBSDServer::mainloop: Configuration generation error" ;
            destructor(bbis, eh);
            delete(app);
            return -1; //Fallo al crear la config (o estaba mal) //TODO Un aviso o algo asi
        }
        //TODO: launch abwizard
        /*
           if (!splashwindow->exec())
           {
           qDebug() << "ABBSDServer::mainloop: Error al salir del dialogo splash" ;
           return -1;
           }
           */
    }
    if (!srh->registerService())
    {
        qDebug("ABBSDServer::mainloop: Unable to launch as service");
    }
    result =  app->exec();
    destructor(bbis, eh);
    delete(app);
    return result;
}
