/***************************************
 *            LogWindow.h
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
//      Name:    BBBInterfaceSystem.h
//      Purpose: bbackupd interface that uses system tools instead of sockets
//      Created: 21/4/08
//
// ------------------


#ifndef BBBINTERFACESYSTEM_H_
#define BBBINTERFACESYSTEM_H_

#include "ProcessControl.h"
#include "BBBInterface.h"
#include "BBInstance.h"
#include "Event.h"
#include <QThread>

// --------------------------------------------------------------------------
//
// Class
//      Name:    BBBInterfaceSystem
//      Purpose: Implementación de BBDaemonInterface que se comunica con el cliente bbackupd sin sockets
//      Created: 21/4/08
//
// ------------------
//
// Excepciones en BBBInterfaceSocket
// Al ser un hilo, todas las excepciones que genere deben ser capturadas en el bucle principal del hilo,

class BBBInterfaceSystem: public QThread, public BBBInterface
{
    Q_OBJECT
    public:
        BBBInterfaceSystem(ABConfiguration *, BBInstance, ProcessControl *);
        void run();
        void RequestRestartClient();
        void RequestStopClient();
        void RequestStartClient();
        bool checkInstance();
        BBInstance instance;
    signals:
        void new_event(Event);
    private:
        void run2();
        void DoStartClient();
        void DoRestartClient();
        void DoStopClient();
        void OnIdle();
        void OnStopClient();
        BBInstance::BBInstanceStatus lastbetterstatus; //Stores status of better instance (See BBInstanceStatus order)
        BBInstance::BBInstanceStatus previousstatus;
        ProcessControl * pc;
};

#endif /*BBBINTERFACESYSTEM_H_*/
