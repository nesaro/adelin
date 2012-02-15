/***************************************
 *            BBDaemonInterfaces.h
 *   Copyright 2007-2008 Adelin 
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
//      Name:    BBDaemonInterfaces.h
//      Purpose: Boxbackup interfaces
//      Created: 12/11/07
//
// ------------------

#ifndef BBDAEMONINTERFACES_H_
#define BBDAEMONINTERFACES_H_



// --------------------------------------------------------------------------
//
// Class
//      Name:    BBDaemonInterface
//      Purpose: Client and server interfaces abstract class
//      Created: 25/10/07
//
// ------------------

//FIXME: Maybe is deprecated

class BBDaemonInterface
{
	public:
     BBDaemonInterface();
	 virtual ~BBDaemonInterface()=0;
	 bool isconnected() const;
    protected:
     bool connected;
};


#endif /*BBDAEMONINTERFACES_H_*/

