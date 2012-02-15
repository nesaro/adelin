/***************************************
 *            BBCInterface.h
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
//      Name:    BBCInterface.h
//      Purpose: Boxbackup configuration interfaces
//      Created: 19/11/07
//
// ------------------

#ifndef BBCINTERFACE_H_
#define BBCINTERFACE_H_

#include "Property.h"
#include "Location.h"
#include "Listeners.h"
#include "Configuration.h"


#include <QTextStream>

#include <map>
#include <assert.h>
// --------------------------------------------------------------------------
//
// Class
//      Name:    BBCInterface
//      Purpose: Boxbackup configuration interface class
//      Created: 25/10/07
//
// ------------------

//It is a PropertyChangeListener, because it must send a signal to dialogs when a property is changed
//
//FIXME: verify and translate this:
// Las consultas de estado se realizan despues de la llamada a cualquier funcion con status(), por lo que las excepciones ocurriran de forma muy puntual. Más adelante se podría implementar un mecanismo de excepciones para diferenciar el motivo del error
//

class BBCInterface: public PropertyChangeListener
{
	public:
// --------------------------------------------------------------------------
//
// Enum
//      Name:    BBCStatus
//      Purpose: Allowed configuration states
//      Created: 7/11/07
//
// ------------------

     enum BBCStatus
     {
         CIS_UNKNOWN = 0,
         CIS_INVALID_ID,
         CIS_OK,
         CIS_NWRITE,
         CIS_NOK,
     };
	 BBCInterface();
	 std::map<std::string, IntProperty *> intprops; //Stores int Properties
	 std::map<std::string, PathProperty *> pathprops; //Stores path Properties
	 std::map<std::string, BoolProperty *> boolprops; //Stores bool Properties
	 std::map<std::string, StringProperty *> stringprops; //Stores string Properties

	 void load_config(std::string filename);
     BBCStatus status() const;
     void notify_property_change(const Property *);
     bool write_backup_config(std::string filename);
     LocationContainer * mLocations;
     void load_default_values(); 



	private:
	 void init_props();
     bool load_locations();
	 std::auto_ptr<Configuration> mapConfig;
     BBCStatus state;
     inline void write_backup_config_write_str_prop(QTextStream & out, std::string prop)
     {
         if(!(stringprops.find(prop) == stringprops.end())) //FIXME throw exception
             out << prop.c_str() << " = "<< stringprops[prop]->get().c_str() << "\n"; 
     };
     inline void write_backup_config_write_path_prop(QTextStream & out, std::string prop)
     {
         assert(pathprops.find(prop)!=pathprops.end()); //FIXME Provisional
         if(!(pathprops.find(prop) == pathprops.end())) //FIXME throw exception
             out << prop.c_str() << " = "<< pathprops[prop]->get().c_str() << "\n"; 
     };
     inline void write_backup_config_write_int_prop(QTextStream & out, std::string prop)
     {
         if(!(intprops.find(prop) == intprops.end())) //FIXME throw exception
             out << prop.c_str() << " = "<< dec << intprops[prop]->get() << "\n"; 
     };
     inline void write_backup_config_write_bool_prop(QTextStream & out, std::string prop)
     {
         std::string value;
         if(!(boolprops.find(prop) == boolprops.end())) //FIXME throw exception
         {
             if(boolprops[prop]->get()) value = "yes";
             else value = "no";
             out << prop.c_str() << " = "<< value.c_str() << "\n"; 
         }
     };
	 
};


#endif /*BBCINTERFACE_H_*/


