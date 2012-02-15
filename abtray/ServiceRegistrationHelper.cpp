/***************************************
 *            ServiceRegistrationHelper.cpp
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of Adelin Backup Controller.
 *   Adelin Backup Controller is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   Adelin Backup Controller is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Adelin Backup Controller.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    ServiceRegistrationHelper.cpp
//      Purpose: Permite a la aplicación registrarse en el arranque del sistema
//      Created: 18/7/08
//
// ------------------


#include "ServiceRegistrationHelper.h"
#include <QTextStream>
#include <QtDebug>

ServiceRegistrationHelper::ServiceRegistrationHelper(QString nombre, QString descripcion, QString mybinarypath):binarypath(mybinarypath)
{
    name = nombre;
    description = descripcion;
}


bool ServiceRegistrationHelper::registerService()
{
    switch(guessDesktopType())
    {
        case DT_WINDOWS:
            return false;
        case DT_GNOME:
            return registerGNOMEService();
        default:
            return false;
    }
}

bool ServiceRegistrationHelper::unregisterService()
{
    switch(guessDesktopType())
    {
        case DT_WINDOWS:
            return false;
        case DT_GNOME:
            return unregisterGNOMEService();
        default:
            return false;
    }
}

ServiceRegistrationHelper::DesktopType ServiceRegistrationHelper::guessDesktopType()
{
#ifdef WIN32
    return DT_WINDOWS;
#else
    return DT_GNOME;
#endif
}

bool ServiceRegistrationHelper::registerGNOMEService()
{
    qDebug() << "ServiceRegistrationHelper::registerGNOMEService: Begin"; 
    //Crear fichero
    //Si se pudo return true;
    QString filepath(QDir::homePath());
    filepath += QString("/.config/autostart/") + name + QString(".desktop");
    //Obtener HOME. Añadir /.config/autostart/applicationname
    QFile file(filepath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) { 
        qDebug() << "ServiceRegistrationHelper::registerGNOMEService: No se pudo abrir el fichero" << filepath;
        return false; }
    QTextStream out(&file);
    out << "[Desktop Entry]\n";
    out << "Encoding=UTF-8\n";
    out << "Version=1.0\n";
    out << "Name=Sin nombre\n";
    out << "Name[es_ES]=" << name << "\n";
    out << "Comment[es_ES]=" << description << "\n";
    out << "Comment=" << description << "\n";
    out << "Exec=" << binarypath.absolutePath() << "\n";
    out << "X-GNOME-Autostart-enabled=true\n";
    return true;
}

bool ServiceRegistrationHelper::unregisterGNOMEService()
{
    qDebug() << "ServiceRegistrationHelper::unregisterGNOMEService: Begin";
    QString filepath(QDir::homePath());
    filepath += QString("/.config/autostart/");
    QDir myfilepath(filepath);
    return myfilepath.remove(name+QString(".desktop"));
}


