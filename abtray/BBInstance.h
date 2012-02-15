/***************************************
 *            BBInstance.h
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
//      Name:    BBInstance.h
//      Purpose: Representacion de las instancias de bbackupd en memoria
//      Created: 20/6/08
//
// ------------------


#ifndef BBINSTANCE_H_
#define BBINSTANCE_H_
#include <QFileInfo>

class BBInstance
{
    public:
        enum BBInstanceStatus
        {
            BBIS_UNKNOWN=0,
            BBIS_OFF,
            BBIS_BADUSER,
            BBIS_BADCONFIG,
            BBIS_READONLY,
            BBIS_OK,
        };
        BBInstance(){} //TODO Constructor nulo
        //BBInstance(QString, QString);
        BBInstance(QString, QString, QString);
        BBInstanceStatus currentStatus() const;
        QString getUserName() const {return username;}
        QFileInfo getConfigPath() const {return confpath;}
        QFileInfo getBinaryPath() const {return binarypath;}
    private:
        QFileInfo binarypath;
        QFileInfo confpath;
        QString username;
};

#endif /*BBINSTANCE_H_*/
