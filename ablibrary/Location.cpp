/***************************************
 *            Location.cpp
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
//      Name:    Location.cpp
//      Purpose: Locations are a path abstraction required for boxbackup
//      Created: 28/11/07
//
// ------------------

#include "Utils.h" 
#include "Location.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    RuleContainer::RuleContainer()
//      Purpose: empty container constructor
//      Created: 9/1/08
//
// ------------------

RuleContainer::RuleContainer()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    RuleContainer::RuleContainer(const Configuration & conf)
//      Purpose: Constructor from boxbackup configuration
//      Created: 28/11/07
//
// ------------------

RuleContainer::RuleContainer(const Configuration & conf)
{
    import_from_config(conf);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::vector<Rule*> RuleContainer::get_entries_vector()
//      Purpose: Constructor 
//      Created: 3/12/07
//
// ------------------

std::vector<Rule> RuleContainer::get_entries_vector()
{
    std::vector<Rule> temp;
    for (size_t l = 0; l < listrules.size(); l++)
    {
        temp.push_back(*listrules[l]);
    }
    return temp; //TODO Verificar si es copia
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void RuleContainer::add_rule(Rule)
//      Purpose: Add Rule
//      Created: 4/12/07
//
// ------------------

void RuleContainer::add_rule(Rule rule)
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void RuleContainer::remove_rule(int)
//      Purpose: remove Rule
//      Created: 4/12/07
//
// ------------------

void RuleContainer::remove_rule(int index)
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void RuleContainer::import_from_config(const Configuration)
//      Purpose: Imports locations from configuration
//      Created: 4/12/07
//
// ------------------

void RuleContainer::import_from_config(const Configuration & conf)
{
    for (size_t i = 0; i < sizeof(AllRuleTypes) / sizeof(RuleType); i++)
    {
        const RuleType& t = AllRuleTypes[i]; 
        import_type_from_config(conf, t);
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void RuleContainer::import_type_from_config(const Configuration, RuleType)
//      Purpose: Import a RuleType from configuration's locations 
//      Created: 4/12/07
//
// ------------------

void RuleContainer::import_type_from_config(const Configuration & conf, RuleType type)
{
    if(!conf.KeyExists(type.ToString().c_str())) return;
    std::string value = conf.GetKeyValue(type.ToString().c_str());
    parse_and_add_separated_list(value, type);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void RuleContainer::parse_and_add_separated_list(std::string, RuleType)
//      Purpose: remove Rule
//      Created: 4/12/07
//
// ------------------

void RuleContainer::parse_and_add_separated_list(std::string list, RuleType type)
{
    std::vector<std::string> temp;
    SplitString(list, Configuration::MultiValueSeparator, temp);
    for (std::vector<std::string>::const_iterator i = temp.begin(); i != temp.end(); i++)
    {
        std::string temp = *i;
        listrules.push_back(new Rule(&type, temp));
    }
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    Location::Location()
//      Purpose: empty Constructor
//      Created: 9/1/08
//
// ------------------

Location::Location()
{
    name = std::string();
    path = std::string();
    rules = NULL;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    Location::Location(std::string name, std::string path)
//      Purpose: Constructor
//      Created: 28/11/07
//
// ------------------

Location::Location(std::string thename, std::string thepath)
{
    name = thename;
    path = thepath;
    rules = NULL;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void Location::set_exclude_list(RuleContainer * rulecont)
//      Purpose: sets exclude list value
//      Created: 28/11/07
//
// ------------------

void Location::set_exclude_list(RuleContainer * rulecont)
{
    rules = rulecont;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    RuleContainer Location::get_exclude_list() const
//      Purpose: returns a container copy
//      Created: 3/12/07
//
// ------------------

RuleContainer Location::get_exclude_list() const
{
    if (rules)
        return *rules; //TODO: Check if a copy is returned
    return RuleContainer();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool Location::operator!=(const Location &)
//      Purpose: != Operator overload
//      Created: 9/1/08
//
// ------------------

bool Location::operator!=(const Location & rhs)
{
    if(name!=rhs.name || path!=rhs.path) // TODO: Falta Rules
        return true;
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    LocationContainer::LocationContainer()
//      Purpose: Constructor
//      Created: 9/1/08
//
// ------------------

LocationContainer::LocationContainer()
{
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    std::vector<Location> LocationContainer::get_locations_vector() const
//      Purpose: Returns a vector with all locations
//      Created: 9/1/08
//
// ------------------
//
std::vector<Location> LocationContainer::get_locations_vector() const
{
    return locationvector;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool LocationContainer::delete_location(Location)
//      Purpose: delete a location
//      Created: 9/1/08
//
// ------------------
//
bool LocationContainer::delete_location(Location location)
{
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool LocationContainer::delete_location_by_name(std::string location_name)
//      Purpose: deletes a location (by name)
//      Created: 9/1/08
//
// ------------------

bool LocationContainer::delete_location_by_name(std::string location_name)
{
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool LocationContainer::delete_location_by_path(std::string location_path)
//      Purpose: deletes a location (by path)
//      Created: 9/1/08
//
// ------------------

bool LocationContainer::delete_location_by_path(std::string location_path)
{
    int i=0;
    std::vector<Location>::iterator lviterator;
    for (lviterator=locationvector.begin();lviterator!=locationvector.end();lviterator++)
    {
        Location tmploc=*lviterator;
        std::string path2 = tmploc.get_path();
        if(location_path==path2)
        {
            locationvector.erase(lviterator);
            return true;
        }
        i++;
    }
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void LocationContainer::append_location(Location)
//      Purpose: adds a location to list
//      Created: 9/1/08
//
// ------------------
//
bool LocationContainer::append_location(Location location)
{
    if (find_by_name(location.get_name())!=Location() || find_by_path(location.get_path())!=Location())
        return false;
    locationvector.push_back(location);
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool LocationContainer::append_location(std::sting name, std::string path)
//      Purpose: adds a location to list with passed args
//      Created: 9/1/08
//
// ------------------
//
bool LocationContainer::append_location(std::string name, std::string path)
{
    Location tmplocation(name,path);
    return append_location(tmplocation);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    Location LocationContainer::find_by_path(std::string path)
//      Purpose: returns a location (located by path)
//      Created: 9/1/08
//
// ------------------
//
Location LocationContainer::find_by_path(std::string location_path) const
{
    std::vector<Location>::const_iterator lviterator;
    for (lviterator=locationvector.begin();lviterator!=locationvector.end();lviterator++)
    {
        Location tmploc=*lviterator;
        std::string path2 = tmploc.get_path();
        if(location_path==path2)
        {
            return *lviterator;
        }
    }
    return Location();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    LocationContainer::find_by_name(std::string name)
//      Purpose: returns a location (located by name)
//      Created: 9/1/08
//
// ------------------
//
Location LocationContainer::find_by_name(std::string name) const
{
    std::vector<Location>::const_iterator lviterator;
    for (lviterator=locationvector.begin();lviterator!=locationvector.end();lviterator++)
    {
        Location tmploc=*lviterator;
        std::string name2 = tmploc.get_name();
        if(name==name2)
        {
            return *lviterator;
        }
    }
    return Location();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void LocationContainer::set_locations_from_vector(std::vector<Location>)
//      Purpose: loads container values from vector
//      Created: 9/1/08
//
// ------------------
//
void LocationContainer::set_locations_from_vector(std::vector<Location> vector)
{
    locationvector = vector;
}
