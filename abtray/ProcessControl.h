/***************************************
 *            ProcessControl.h
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
//      Name:    ProcessControl.h
//      Purpose: Control de procesos
//      Created: 23/4/08
//
// ------------------

#ifndef PROCESSCONTROL_H_
#define PROCESSCONTROL_H_

#include <list>
#include <QString>
#include "BBInstance.h"
#include "ABConfiguration.h"

class ProcessControl
{
    public:
        ProcessControl(ABConfiguration *);
        static bool launchInstance(BBInstance);
        BBInstance::BBInstanceStatus instanceStatus(BBInstance) const; 
        std::list<BBInstance> allValidInstances();
        static bool haltInstance(BBInstance);
        static bool isRunningInstance(BBInstance);
        static QString currentUserName();
    private:
        static std::list<BBInstance> allInstances();
        static unsigned int getInstancePID(BBInstance);
        static QString getPIDUserName(unsigned int pid);
        static QString userNamefromUID(unsigned int uid);
        static unsigned int getUID(int pid);
        static std::list<int> getPidProcessByName(std::string name);
        static bool checkProcessUser(int pid, int uid);
        static bool checkProcessUser(int pid, std::string name);
        static QString getConfigPath(int pid);
        static QString getBinaryPath(int pid);
        static BBInstance generateBBInstanceFromPID(int pid);
        ABConfiguration * conf;
};


#endif /*PROCESSCONTROL_H_*/
