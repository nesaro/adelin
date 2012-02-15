/***************************************
 *            ServiceRegistrationHelper.h
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
//      Name:    ServiceRegistrationHelper.h
//      Purpose: Permite a la aplicación registrarse en el arranque del sistema
//      Created: 18/7/08
//
// ------------------


#ifndef SERVICEREGISTRATIONHELPER_H_
#define SERVICEREGISTRATIONHELPER_H_
#include <QString>
#include <QDir>

class ServiceRegistrationHelper
{
    public:
        ServiceRegistrationHelper(QString nombre, QString descripcion, QString binarypath);
        bool registerService();
        bool unregisterService();
    private:
        enum DesktopType{
            DT_UNKNOWN=0,
            DT_WINDOWS,
            DT_GNOME,
            DT_KDE
        };
        static DesktopType guessDesktopType();
        bool registerGNOMEService();
        bool unregisterGNOMEService();
        QString name;
        QString description;
        QDir binarypath;
};

#endif /*SERVICEREGISTRATIONHELPER_H_*/
