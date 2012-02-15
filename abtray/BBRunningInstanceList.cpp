/***************************************
 *            BBRunningInstanceList.cpp
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
//      Name:    BBRunningInstanceList.cpp
//      Purpose: Generates lists of bbackupd instances
//      Created: 26/6/08
//
// ------------------


#include "BBRunningInstanceList.h"
#include "AB.h"
#include <QtDebug>
#ifdef WIN32
#include <windows.h>
#endif

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBRunningInstanceList::BBRunningInstanceList
//      Purpose: Constructor
//      Created: 26/6/08
//
// ------------------

BBRunningInstanceList::BBRunningInstanceList(ABConfiguration * myconf, ProcessControl * mypc)
{
    qDebug() << "BBRunningInstanceList::BBRunningInstanceList: Begin()";
    conf = myconf;
    pc = mypc;
}

void BBRunningInstanceList::onBBBInterfaceSystemEvent(Event eh)
{
    qDebug() << "BBRunningInstanceList::onBBBInterfaceSystemEvent: Begin()";
    emit new_event(eh);
}

bool BBRunningInstanceList::beginProcedure()
{
    qDebug() << "BBInterfaceStatus::beginProcedure(): begin";
    std::list<BBInstance> lista = pc->allValidInstances();
    //Si existe la instancia perfecta, 
    for (std::list<BBInstance>::const_iterator i=lista.begin(); i!=lista.end(); i++)
    {
        currentInterface.reset(new BBBInterfaceSystem(conf, (*i), pc));
        QObject::connect(currentInterface.get(), SIGNAL(new_event(Event)), this, SLOT(onBBBInterfaceSystemEvent(Event)));
        Event myevent(abbsd::bbackupdinstancechanged);
        myevent.add_source(Event::ES_INTERNAL);
        emit new_event(myevent);
        currentInterface->start();
        //Conectar
        return true;
    }

    qDebug() << "BBRunningInstanceList::beginProcedure(): No se ha encontrado instancia alguna... arrancando";
    BBInstance myinstance(conf->get_bb_config_filename().c_str(),ProcessControl::currentUserName(),AB::system_bb_binary_location_1);
    if(launchInstance(myinstance))
    {
#ifndef WIN32
        sleep(1);
#else
	Sleep(1000);
#endif
        if(ProcessControl::isRunningInstance(myinstance))
        {
            qDebug() << "BBRunningInstanceList::beginProcedure(): Instancia lanzada con exito";
            currentInterface.reset(new BBBInterfaceSystem(conf, myinstance, pc));
            QObject::connect(currentInterface.get(), SIGNAL(new_event(Event)), this, SLOT(onBBBInterfaceSystemEvent(Event)));
            Event myevent(abbsd::bbackupdinstancechanged);
            myevent.add_source(Event::ES_INTERNAL);
            emit new_event(myevent);
            currentInterface->start();
            //Conectar
            return true;
        }
        else
        {
            qDebug() << "BBRunningInstanceList::beginProcedure(): Error, no se detecto la instancia lanzada";
            return false;
        }
    }
    else
    {
        qDebug() << "BBRunningInstanceList::beginProcedure(): Error, no se pudo lanzar la instancia";
        return false;
    }

    return false;
}

bool BBRunningInstanceList::launchInstance(BBInstance myinstance)
{
    return ProcessControl::launchInstance(myinstance);
}


BBInstance BBRunningInstanceList::currentInstance()
{
    if (currentInterface.get() == NULL) return BBInstance();
    return currentInterface->instance;
}
