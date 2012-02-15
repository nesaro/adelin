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
//      Purpose: Main Program
//      Created: 30/03/09
//
// ------------------
	

#include <QtDebug>
#include <QApplication>
#include "SplashWindow.h"
#include "ABConfiguration.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    main
//      Purpose: Main Program
//      Created: 30/03/09
//
// ------------------
	

int main(int argc, char *argv[])
{
    QTranslator myTranslator, qtTranslator;
	QApplication * app = new QApplication(argc,argv);
    qtTranslator.load(QString(":/qt_") + QLocale::system().name());
    app->installTranslator(&qtTranslator);
    myTranslator.load(QString(":/abwizard_") + QLocale::system().name());
    app->installTranslator(&myTranslator);
    //Q_INIT_RESOURCE(adelinbackup);
    ABConfiguration * conf = new ABConfiguration();
    SplashWindow * splashwindow = new SplashWindow(conf);
    qDebug() << "main: Iniciando Bucle Qt";
    return splashwindow->exec();
	//return app->loop();

}
