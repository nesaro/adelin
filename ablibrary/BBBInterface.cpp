/***************************************
 *            BBBInterface.cpp
 *   Copyright 2007-2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of Adelin Backup.
 *   Adelin Backup is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   Adelin Backup is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Adelin Backup.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    BBBInterface.cpp
//      Purpose: bbackupd binary interfaces
//      Created: 20/11/07
//
// ------------------

#include "BBBInterface.h"
#include <QtDebug>


// --------------------------------------------------------------------------
//
// Function
//      Name:  BBBInterface::BBBInterface()  
//      Purpose: Constructor
//      Created: 16/11/07
//
// ------------------


BBBInterface::BBBInterface(ABConfiguration * configuration)
{	
    CurrentDaemonStatus = BBBDS_UNKNOWN;
    conf = configuration;
    mutex = new QMutex();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBBInterface::~BBBInterface()  
//      Purpose: Destructor
//      Created: 22/4/08
//
// ------------------


BBBInterface::~BBBInterface()
{	
    delete mutex;
}


