/***************************************
 *            BBInterfaceStatus.h
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
//      Name:    BBInterfaceStatus.h
//      Purpose: Encapsulates every bbackupd interface
//      Created: 20/6/08
//
// ------------------


#ifndef BBINTERFACESTATUS_H_
#define BBINTERFACESTATUS_H_
#include "BBBInterfaceSystem.h"
#include "BBRunningInstanceList.h"
#include "ABConfiguration.h"
#include "ProcessControl.h"
#include <QObject>


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBInterfaceStatus
//      Purpose: Encapsulates everything about bbackupd
//      Created: 20/6/08
//
// ------------------
//

class BBInterfaceStatus: public QObject
{
    Q_OBJECT
    public:
        BBInterfaceStatus(ABConfiguration * conf);
        ~BBInterfaceStatus();
        bool CIIRestart();
        bool CIIStart();
        bool CIIStop();
        bool beginProcedure();
        bool connectToInstance(BBInstance);
        bool isRunningInstance(BBInstance);
        bool launchInstance(BBInstance);
        bool haltCurrentInstance();
        std::list<BBInstance> allInstances();
        std::list<BBInstance> allValidInstances();
        BBInstance currentInstance() const;
    signals:
        void new_event(Event);
    public slots:
        void onBBRunningInstanceListEvent(Event);
    private:
        ABConfiguration * conf;
        BBRunningInstanceList * ril;
        ProcessControl * pc;
};

#endif /*BBINTERFACESTATUS_H_*/
