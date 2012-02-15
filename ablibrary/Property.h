/***************************************
 *            Property.h
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
//      Name:    Property.h
//      Purpose: Boxbackup config is composed by properties
//      Created: 25/10/07
//
// ------------------

#ifndef PROPERTY_H_
#define PROPERTY_H_
#include "Listeners.h"
#include "Configuration.h"

#include <string>

// --------------------------------------------------------------------------
//
// Class
//      Name:    Property
//      Purpose: Abstract property class
//      Created: 25/10/07
//
// ------------------

class Property
{
	public:
	 Property(std::string, PropertyChangeListener *);
	 //BoxBackup Interface
	 virtual void SetFrom(const Configuration* pConf) = 0;
	 const std::string& GetKeyName() const { return mKeyName; }
     virtual ~Property();
     bool is_configured() const{return mConfigured;};
	 
	protected:
	 bool mConfigured;
     std::string mKeyName;
     PropertyChangeListener * mpListener;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    IntProperty
//      Purpose: Integer format property
//      Created: 07/11/07
//
// ------------------


class IntProperty: public Property
{
	public:
	 IntProperty(std::string, PropertyChangeListener *);
     ~IntProperty();
	 void set(int newValue);
	 int get() const;
	 void SetFrom(const Configuration* pConf);
     bool GetInto(int& rDest) const;
	private:
	 int mValue;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    BoolProperty
//      Purpose: Boolean format property
//      Created: 07/11/07
//
// ------------------

class BoolProperty: public Property
{
	public:
	 BoolProperty(std::string, PropertyChangeListener *);
     ~BoolProperty();
	 void SetFrom(const Configuration* pConf);
     bool get() const;
     void set(bool);
    private:
     bool mValue;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    StringProperty
//      Purpose: String format property
//      Created: 07/11/07
//
// ------------------

class StringProperty: public Property
{
	public:
	 StringProperty(std::string, PropertyChangeListener *);
     ~StringProperty();
     void set(std::string);
	 void SetFrom(const Configuration* pConf);
     bool GetInto(std::string& rDest) const;
     std::string get() const;
    protected:
     std::string mValue;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    PathProperty
//      Purpose: Path format propery
//      Created: 07/11/07
//
// ------------------


class PathProperty: public StringProperty
{
	public:
	 PathProperty(std::string, PropertyChangeListener *);
     ~PathProperty();
};


#endif /*PROPERTY_H_*/
