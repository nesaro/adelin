/***************************************
 *            Location.h
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
//      Name:    Location.h
//      Purpose: Locations are a path abstraction required for boxbackup
//      Created: 30/10/07
//
// ------------------

#ifndef LOCATION_H_
#define LOCATION_H_


#include "Configuration.h"
#include <string>

enum ExcludeSense {
    ES_UNKNOWN = 0,
    ES_EXCLUDE,
    ES_ALWAYSINCLUDE,
};

enum ExcludeFileDir {
    EFD_UNKNOWN = 0,
    EFD_FILE,
    EFD_DIR,
};

enum ExcludeMatch {
    EM_UNKNOWN = 0,
    EM_EXACT,
    EM_REGEX,
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    RuleType
//      Purpose: Define los atributos de una regla de una Location
//      Created: 30/10/07
//
// ------------------

class RuleType{
    public:
        RuleType(ExcludeSense sense, ExcludeFileDir filedir, ExcludeMatch match)
        {
            this-> sense = sense;
            this-> filedir = filedir;
            this-> match = match;
        }
        std::string GetSenseString() const{
            std::string temp;
            switch (sense) {
                case ES_EXCLUDE:        temp = "Exclude";       break;
                case ES_ALWAYSINCLUDE:  temp = "AlwaysInclude"; break;
                default:                temp = "Unknown";       break;
            }
            return temp;
        }
        std::string GetFileDirString() const{
            std::string temp;
            switch (filedir) {
                case EFD_FILE:  temp = "File";      break;
                case EFD_DIR:   temp = "Dir";       break;
                default:        temp = "Unknown";   break;
            }
            return temp;
        }
        std::string GetMatchString() const{
            std::string temp;
            switch (match) {
                case EM_EXACT:  temp = "";          break;
                case EM_REGEX:  temp = "sRegex";    break;
                default:        temp = "Unknown";   break;
            }
            return temp;
        }
        std::string ToString() const
        {
            std::string buffer = "";
            buffer.append(GetSenseString());
            buffer.append(GetFileDirString());
            buffer.append(GetMatchString());
            return buffer;
        }
    private:
        ExcludeSense sense;
        ExcludeFileDir filedir;
        ExcludeMatch match;
};

const RuleType AllRuleTypes[] = {
    RuleType(ES_EXCLUDE,       EFD_DIR,    EM_EXACT),
    RuleType(ES_EXCLUDE,       EFD_DIR,    EM_REGEX),
    RuleType(ES_EXCLUDE,       EFD_FILE,   EM_EXACT),
    RuleType(ES_EXCLUDE,       EFD_FILE,   EM_REGEX),
    RuleType(ES_ALWAYSINCLUDE, EFD_DIR,    EM_EXACT),
    RuleType(ES_ALWAYSINCLUDE, EFD_DIR,    EM_REGEX),
    RuleType(ES_ALWAYSINCLUDE, EFD_FILE,   EM_EXACT),
    RuleType(ES_ALWAYSINCLUDE, EFD_FILE,   EM_REGEX),
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    Rule
//      Purpose: Padre de definición reglas de exclusión de ficheros y directorios
//      Created: 30/10/07
//
// ------------------


    class Rule {
        public:
        Rule(RuleType *type, std::string value)
        {
            this->mType=new RuleType(*type); //FIXME: Hace falta generar la copia con new, es preferible usar un RuleType en vez de RuleType *
            this->mValue=value;
        }
        std::string ToString() {
            std::string buffer;
            buffer.append(ToStringType());
            buffer.append(" = ");
            buffer.append(mValue);
            return buffer;
        }

        private:
        const std::string ToStringType() const { return mType->ToString(); }
        std::string mValue;
        RuleType * mType;
    };


// --------------------------------------------------------------------------
//
// Class
//      Name:    RuleContainer
//      Purpose: Contenedor de Reglas
//      Created: 28/11/07
//
// ------------------


class RuleContainer {
    public:
        RuleContainer();
        RuleContainer(const Configuration & conf);
        std::vector<Rule> get_entries_vector();
        void add_rule(Rule);
        void remove_rule(int);
        void import_from_config(const Configuration &);
    private:
        std::vector <Rule*> listrules;
        void parse_and_add_separated_list(std::string,RuleType);
        void import_type_from_config(const Configuration &,RuleType);
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    Location
//      Purpose: Encapsulado de las rutas sobre las que se realiza copias de seguridad
//      Created: 30/10/07
//
// ------------------

class Location{
    public:
        Location();
        Location(std::string name, std::string path);
        bool operator!=(const Location&);
        std::string get_path() const{return path;};
        std::string get_name() const{return name;};
        RuleContainer get_exclude_list() const;
        void set_path(std::string newpath);
        void set_name(std::string newname){name=newname;}
        void set_exclude_list(RuleContainer *);

    private:
        std::string name;
        std::string path;
        RuleContainer * rules;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    LocationContainer
//      Purpose: Clase encargada de la gestion de las Location almacenadas en memoria
//      Created: 8/1/08
//
// ------------------

class LocationContainer{
    public:
        LocationContainer();
        std::vector<Location> get_locations_vector() const;
        void set_locations_from_vector(std::vector<Location>);
        bool delete_location(Location);
        bool delete_location_by_name(std::string location_name);
        bool delete_location_by_path(std::string location_path);
        bool append_location(Location);
        bool append_location(std::string name, std::string path);
        Location find_by_path(std::string) const;
        Location find_by_name(std::string) const;
        void clear(){locationvector.clear();}

    private:
        std::vector<Location> locationvector;
};

#endif /*LOCATION_H_*/
