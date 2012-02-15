/***************************************
 *            ABConfiguration.h
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
//      Name:    ABConfiguration.h
//      Purpose: Program Configuration main class
//      Created: 8/11/07
//
// ------------------

#ifndef ABCONFIGURATION_H_
#define ABCONFIGURATION_H_

#include "Listeners.h"
#include "BBCInterface.h"
#include <QSettings>

// --------------------------------------------------------------------------
//
// Enum
//      Name:    ABCStatus
//      Purpose: ABConfiguration states
//      Created: 5/2/08
//
// ------------------

enum ABCStatus
{
    ABCS_UNKNOWN = 0,
    ABCS_OK,
    ABCS_NWRITE,
    ABCS_NOK,
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    ABConfiguration
//      Purpose: Program Configuration main class
//      Created: 8/11/07
//
// ------------------


class ABConfiguration: public PropertyChangeListener
{
    public:
     ABConfiguration();
     std::string get_bb_config_filename() const;
     std::string get_bb_config_dirname() const;
     std::string get_bb_binary_filename() const;
     std::string get_bb_data_dirname() const;
     std::string get_bb_keys_dirname() const;
     std::string get_openssl_filename() const;
     bool set_bb_config_filename(std::string filename) throw();
     bool set_bb_binary_filename(std::string filename) throw();
     bool set_bb_keys_dirname(std::string filename) throw();
     bool set_bb_data_dirname(std::string filename) throw();
     bool set_openssl_filename(std::string filename) throw();
     bool check_bb_config_filename(std::string filename) const throw();
     bool check_bb_binary_filename(std::string filename) const throw();
     bool check_bb_keys_dirname(std::string filename) const throw();
     bool check_bb_data_dirname(std::string filename) const throw();
     bool check_openssl_filename(std::string filename) const throw();
     bool check_bb_config_filename() const throw();
     bool check_bb_binary_filename() const throw();
     bool check_bb_keys_dirname() const throw();
     bool check_bb_data_dirname() const throw();
     bool check_openssl_filename() const throw();
     void load_bb_config() { bbconf->load_config(get_bb_config_filename()); } //FIXME: What if get_bb_config_filename returnts ""?
     bool write_bb_config(bool oninstall = false);
     bool load_config() throw();
     bool write_config() throw();
     bool bb_config_file_readable() const;
     bool bb_config_file_writable() const;
     bool installation_procedure() throw();
     ABCStatus status() const{return current_status;}
     BBCInterface * bbconf; //TODO must be private
    private:
     void notify_property_change(const Property *);
     void write_default_conf();
     static bool check_binary_filename(std::string) throw();
     static bool check_readable_filename(std::string) throw();
     static bool check_writable_filename(std::string) throw();
     ABCStatus current_status;
     QSettings * settings;
};

#endif /*ABCONFIGURATION_H_*/
