/***************************************
 *            Listeners.h
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
//      Name:    Listeners.h
//      Purpose: Clases que emulan interrupciones
//      Created: 25/10/07
//
// ------------------

#ifndef LISTENERS_H_
#define LISTENERS_H_


class Property; //Forward declaration

// --------------------------------------------------------------------------
//
// Class
//      Name:    Listener
//      Purpose: Parent for every class that listen changes like interruptions
//      Created: 25/10/07
//
// ------------------

class Listener{};


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBCInterfaceListener
//      Purpose: virtual class used by dialogs that listens to configuration changes 
//      Created: 25/10/07
//
// ------------------


class BBCInterfaceListener: public Listener
{
	public:
	 virtual ~BBCInterfaceListener() {}
	 virtual void notify_config_change()=0;	
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    PropertyChangeListener
//      Purpose: Class that is able to listen when a property is changed
//      Created: 25/10/07
//
// ------------------



class PropertyChangeListener: public Listener
{
	public:
        virtual void notify_property_change(const Property *)=0;
        virtual ~PropertyChangeListener() {}
	
};


#endif /*LISTENERS_H_*/
