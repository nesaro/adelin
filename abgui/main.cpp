/***************************************
 *            main.cpp
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
//      Name:    main.cpp
//      Purpose: Programa principal
//      Created: 23/10/07
//
// ------------------
	

#include <QtDebug>
#include "ABConfiguration.h"
#include "UserInterface.h"
#include "BBBInterfaceSocket.h"
#include "BBSInterface.h"

#include <QApplication>

// --------------------------------------------------------------------------
//
// Function
//      Name:    main
//      Purpose: Main Program
//      Created: 23/10/07
//
// ------------------
	

int main(int argc, char *argv[])
{
    QTranslator myTranslator, qtTranslator;
	ABConfiguration * conf = new ABConfiguration();
#ifdef WIN32
	WSADATA info;
	assert(WSAStartup(0x0101, &info)!=SOCKET_ERROR);
#endif
	QApplication * app = new QApplication(argc,argv);
    qRegisterMetaType<BBSInterface::BBSStatus>("BBSInterface::BBSStatus"); //Permite usar los BBSStatus como objetos a pasar en Conexiones Qt
    qtTranslator.load(QString(":/qt_") + QLocale::system().name());
    app->installTranslator(&qtTranslator);
    myTranslator.load(QString(":/adelinbackup_") + QLocale::system().name());
    app->installTranslator(&myTranslator);
    Q_INIT_RESOURCE(adelinbackup);
    qDebug() << "MainApp::MainApp: Iniciando SSLLib";
    SSLLib::Initialise();
    qDebug() << "MainApp::MainApp: Iniciando BBBInterfaceSocket";
	BBBInterfaceSocket * bbbi = new BBBInterfaceSocket(conf);
    qDebug() << "MainApp::MainApp: Iniciando BBSInterface";
    BBSInterface * bbsi = new BBSInterface(conf);
    qDebug() << "MainApp::MainApp: Iniciando UserInterface";
	UserInterface * ui = new UserInterface(app, argc, argv, conf, bbbi, bbsi);
    //Añadir el conector con autenticacion para solicitar el id de usuario
    qDebug() << "MainApp::MainApp: Comprobando estado de la configuración";
    //if(conf->bbconf->status() != BBCInterface::CIS_OK) 
    //{
    //    int returncode = QMessageBox::warning(0,"Error","No ha configurado correctamente adelin backup. Ejecutar el controlador backup?",QMessageBox::Yes|QMessageBox::Close,QMessageBox::Yes);
    //    if (returncode == QMessageBox::Yes)
    //    {
    //        QProcess::startDetached("/usr/bin/abcontroller");
    //        return 0;
    //    }
    //    else
    //    {
    //        return -1;
    //    }
    //}
    qDebug() << "MainApp::MainApp: Iniciando Hilo BBBInterfaceSocket";
    bbbi->start();
    qDebug() << "MainApp::mainloop: Iniciando Bucle Qt";
	return ui->loop();

    //FIXME: stop threads
    /*
    bbbi->shutdown();
    while(bbbi->isRunning())
    {
        sleep(1);
    }
    delete bbsi;
    delete bbbi;
    */
}
