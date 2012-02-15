/***************************************
*            InstanceSelectorWindow.h
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
//      Name:    InstanceSelectorWindow.h
//      Purpose: Manejador del fichero de logs de BB
//      Created: 3/3/08
//
// ------------------


#ifndef INSTANCESELECTORWINDOW_H_
#define INSTANCESELECTORWINDOW_H_
#include <QDialog>
#include "ui_InstanceSelectorWindow.h"
#include "BBInterfaceStatus.h"

class InstanceSelectorWindow: public QDialog
{
    Q_OBJECT
    public:
        InstanceSelectorWindow(BBInterfaceStatus *);
    private slots:
        void updateInstanceList();
    private:
        bool appendInstancetoList(BBInstance);
        Ui::InstanceSelectorWindow ui;
        void connections();
        BBInterfaceStatus * bbis;
};

#endif /*INSTANCESELECTORWINDOW_H_*/
