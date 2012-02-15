/***************************************
 *            Property.cpp
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
//      Name:    Property.cpp
//      Purpose: Configurable properties
//      Created: 25/10/07
//
// ------------------

#include "Property.h"

#include <iostream>

Property::Property(std::string name, PropertyChangeListener * config)
{	
    mKeyName = name;
    mpListener = config;
    mConfigured = false;
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    Property::~Property
//      Purpose: Destructor 
//      Created: 12/11/07
//
// ------------------


Property::~Property()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    PathProperty::PathProperty
//      Purpose: Constructor
//      Created: 12/11/07
//
// ------------------


PathProperty::PathProperty(std::string name, PropertyChangeListener * config):StringProperty(name,config)
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    PathProperty::~PathProperty
//      Purpose: Destructuor
//      Created: 12/11/07
//
// ------------------
PathProperty::~PathProperty()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    IntProperty::IntProperty
//      Purpose: Constructor
//      Created: 12/11/07
//
// ------------------

IntProperty::IntProperty(std::string name, PropertyChangeListener * config):Property(name,config)
{
	mConfigured=false;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    IntProperty::~IntProperty
//      Purpose: Destructuor
//      Created: 12/11/07
//
// ------------------
IntProperty::~IntProperty()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    IntProperty::set(int)
//      Purpose: sets property value
//      Created: 12/11/07
//
// ------------------

void IntProperty::set(int newValue)
{
	mValue = newValue;	
	mConfigured = true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    IntProperty::get(int)
//      Purpose: return property value
//      Created: 12/11/07
//
// ------------------

int IntProperty::get() const
{
	if (mConfigured) return mValue;
	return -1; //Exception?	
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void IntProperty::SetFrom(const Configuration* pConf)
//      Purpose: loads an integer value from config
//      Created: 29/10/07
//
// ------------------

void IntProperty::SetFrom(const Configuration* pConf)
{
    if (pConf->KeyExists(mKeyName.c_str())) {
        mValue = pConf->GetKeyValueInt(mKeyName.c_str());
        mConfigured = true;
    } else {
        mConfigured = false;
    }
    //SetClean();

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool IntProperty::GetInto(int& rDest);
//      Purpose: loads an integer from parameter (as reference)
//      Created: 13/11/07
//
// ------------------

bool IntProperty::GetInto(int& rDest) const
{
    if (!mConfigured) return false;
    rDest = mValue;
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BoolProperty::BoolProperty
//      Purpose: Constructor
//      Created: 13/11/07
//
// ------------------


BoolProperty::BoolProperty(std::string name, PropertyChangeListener * config):Property(name,config)
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BoolProperty::~BoolProperty
//      Purpose: Destructuor
//      Created: 12/11/07
//
// ------------------
//
BoolProperty::~BoolProperty()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BoolProperty::SetFrom(const Configuration *)
//      Purpose: loads an bool value from config
//      Created: 12/11/07
//
// ------------------
//
void BoolProperty::SetFrom(const Configuration* pConf)
{
    if (pConf->KeyExists(mKeyName.c_str())) {
        mValue = pConf->GetKeyValueInt(mKeyName.c_str());
        mConfigured = true;
    } else {
        mConfigured = false;
    }
    //SetClean();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void BoolProperty::set(bool value)
//      Purpose: sets property value
//      Created: 19/2/08
//
// ------------------
//
void BoolProperty::set(bool newValue)
{
	mValue = newValue;	
	mConfigured = true;
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    bool BoolProperty::get()
//      Purpose: return property value
//      Created: 3/12/07
//
// ------------------
bool BoolProperty::get() const
{
    if(!mConfigured) return false; //FIXME El mensaje  de error no puede ser ni verdadero ni falso. Throw exception
    return mValue;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    StringProperty::StringProperty
//      Purpose: Constructor
//      Created: 12/11/07
//
// ------------------
StringProperty::StringProperty(std::string name, PropertyChangeListener * config):Property(name,config)
{
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    StringProperty::~StringProperty
//      Purpose: Destructor
//      Created: 12/11/07
//
// ------------------
StringProperty::~StringProperty()
{
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void StringProperty::SetFrom(const Configuration *)
//      Purpose: loads a string value from config
//      Created: 12/11/07
//
// ------------------
void StringProperty::SetFrom(const Configuration* pConf)
{
    if (pConf->KeyExists(mKeyName.c_str())) {
        this->mValue = pConf->GetKeyValue(mKeyName.c_str());
        this->mConfigured = true;
    } else {
        this->mConfigured = false;
    }
    //SetClean();

}



// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string StringProperty::get()
//      Purpose: return property value
//      Created: 3/12/07
//
// ------------------
std::string StringProperty::get() const
{
    if(!mConfigured) return std::string();
    return mValue;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void StringProperty::set(std::string)
//      Purpose: sets property value
//      Created: 10/2/07
//
// ------------------

void StringProperty::set(std::string newValue)
{
	mValue = newValue;	
	mConfigured = true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool StringProperty::GetInto(std::string& rDest)
//      Purpose: 
//      Created: 3/12/07
//
// ------------------
bool StringProperty::GetInto(std::string& rDest) const
{
    //TODO 
    if (!mConfigured) return false;
    rDest = mValue;
    return true;
}

