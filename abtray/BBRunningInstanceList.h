/***************************************
 *            BBRunningInstanceList.h
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
//      Name:    BBRunningInstanceList.h
//      Purpose: Generates lists of bbackupd instances
//      Created: 26/6/08
//
// ------------------


#ifndef BBRUNNINGINSTANCELIST_H_
#define BBRUNNINGINSTANCELIST_H_
#include "BBInstance.h"
#include "BBBInterfaceSystem.h"
#include "Event.h"
#include "ABConfiguration.h"


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBRunningInstanceList
//      Purpose: Listas de instancias de bbackupd
//      Created: 26/6/08
//
// ------------------

class BBRunningInstanceList: public QObject
{
    Q_OBJECT
    public:
        BBRunningInstanceList(ABConfiguration *, ProcessControl * );
        std::list<BBInstance> allInstances();
        std::list<BBInstance> allValidConfigInstances();
        BBInstance haltInstance(BBInstance);
        bool launchInstance(QString Configpath);
        bool launchInstance(BBInstance);
        bool isRunningInstance(BBInstance);
        bool connectToInstance(BBInstance); //TODO Generar evento de cambio de interfaz si procede
        bool beginProcedure();
        BBInstance currentInstance();
    public slots:
        void onBBBInterfaceSystemEvent(Event);
    signals:
        void new_event(Event);
    private:
        std::auto_ptr<BBBInterfaceSystem> currentInterface;
        bool checkConfigServer(BBInstance);
        ABConfiguration * conf;
        ProcessControl * pc;
};

#endif /*BBBINTERFACESYSTEM_H_*/
