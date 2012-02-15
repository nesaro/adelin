/***************************************
 *            BBBInterfaceSystem.cpp
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
//      Name:    BBBInterfaceSystem.cpp
//      Purpose: Interfaz bbackupd (sin sockets)
//      Created: 21/4/08
//
// ------------------

#include "BBBInterfaceSystem.h"
#include <QtDebug>

BBBInterfaceSystem::BBBInterfaceSystem(ABConfiguration * conf, BBInstance myinstance, ProcessControl * mypc):BBBInterface(conf)
{
    previousstatus = BBInstance::BBIS_UNKNOWN;
    instance = myinstance;
    pc = mypc;
}

void BBBInterfaceSystem::run()
{
    try{
        run2();
    }
    catch(...)
    {
        qDebug() << "BBBInterfaceSystem::run: Excepción capturada";
        sleep(1);
        exit();
        return;
    }
}

void BBBInterfaceSystem::run2()
{
    qDebug() << "BBBInterfaceSystem::run2(): Begin";
    BBInstance::BBInstanceStatus tmpstatus;
    std::list<int> processpids;
    Event badevent(abbsd::daemonbadstatus);
    badevent.add_source(Event::ES_SYSTEM);
    Event changeevent(abbsd::daemonstatuschanged);
    changeevent.add_source(Event::ES_SYSTEM);
    while (true) //TODO estudiar si hace falta condicion de salida o con la señal es suficiente
    {
        qDebug() << "BBBInterfaceSystem::run2(): Beggining iteration";
        lastbetterstatus = pc->instanceStatus(instance);
        qDebug() << "BBBInterfaceSystem::run2(): lastbetterstatus" << lastbetterstatus;
        switch(lastbetterstatus) //Daemon Status
        {
            case BBInstance::BBIS_OFF:
                qDebug() << "BBBInterfaceSystem::run2(): Trying to start instance";
                if (!ProcessControl::launchInstance(instance))
                {
                    emit new_event(badevent);
                }
                break;
            case BBInstance::BBIS_UNKNOWN:
            case BBInstance::BBIS_BADUSER:
            case BBInstance::BBIS_BADCONFIG:
                qDebug() << "BBBInterfaceSystem::run2(): Generating event:" << "BadEvent";
                emit new_event(badevent);
                break;
            case BBInstance::BBIS_OK:
                break;
        }
        if (previousstatus != lastbetterstatus)
        {
            previousstatus = lastbetterstatus;
            qDebug() << "BBBInterfaceSystem::run2(): Generating event:" << "ChangeStatusEvent";
            emit new_event(changeevent);
        }
        sleep(60); 
    }
}

