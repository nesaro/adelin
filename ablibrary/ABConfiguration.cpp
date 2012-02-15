/***************************************
 *            Adelin BackupConfiguration.cpp
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
//      Name:    ABConfiguration.cpp
//      Purpose: Adelin Backup Main Configuration
//      Created: 8/11/07
//
// ------------------




#include "ABConfiguration.h"
#include <unistd.h>
#include <fstream>
#include "AB.h"
#include <QFile>
#include <QFileInfo>
#include <QDir>

// --------------------------------------------------------------------------
//
// Function
//      Name:    ABConfiguration::ABConfiguration
//      Purpose: Constructor
//      Created: 8/11/07
//
// ------------------

ABConfiguration::ABConfiguration(){
    bbconf = new BBCInterface();
    //adelin_config_location = AB::system_config_default_location;
    settings = new QSettings("Adelin","Adelin Backup");
    current_status = ABCS_UNKNOWN;
    if (!load_config())
    {
        write_default_conf();
    }
    /* //FIXME: change to QSettings
    try
    {
        configinterface->readFile(adelin_config_location.c_str());
        current_status = ABCS_OK;
    }
    catch(libconfig::FileIOException)
    {
        //Error en la lectura de fichero
        try{
            write_default_conf();
            configinterface->readFile(adelin_config_location.c_str());
            current_status = ABCS_OK;
        }
        catch(libconfig::ConfigException)
        {
            current_status = ABCS_NOK;
        }
    }
    catch(libconfig::ParseException)
    {
        //Error en el contenido del fichero
        //TODO ¿Que hacer?
        current_status = ABCS_NOK;
    }
    */
    load_bb_config(); 
    if (bbconf->status()==BBCInterface::CIS_OK) return;
    //TODO: Think what to do
}
    
// --------------------------------------------------------------------------
//
// Function
//      Name:    void ABConfiguration::write_default_conf()
//      Purpose: Writes default AdelinBackup config
//      Created: 5/2/07
//
// ------------------

void ABConfiguration::write_default_conf()
{
    //TODO Exception handling
    settings->setValue("config_path", AB::system_bb_config_default_location().c_str());
    if (check_bb_binary_filename(AB::system_bb_binary_default_location))
        settings->setValue("bbackupd_path", AB::system_bb_binary_default_location);
    else if (check_bb_binary_filename(AB::system_bb_binary_location_1))
        settings->setValue("bbackupd_path", AB::system_bb_binary_location_1);
    else
        settings->setValue("bbackupd_path","");
    settings->setValue("bbkeys_dir_path", AB::system_bb_key_dir_default_location().c_str());
    settings->setValue("data_dir_path", AB::system_bb_data_dir_default_location().c_str());
    settings->setValue("openssl_bin_path", AB::system_openssl_bin_default_location);
    //bb_keys_dir
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ABConfiguration::notify_property_change(const Property *)
//      Purpose: What to do when a property changes
//      Created: 12/11/07
//
// ------------------

void ABConfiguration::notify_property_change(const Property * prop)
{
    //TODO
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::write_bb_config()
//      Purpose: Stores BoxBackup Configuration
//      Created: 3/1/08
//
// ------------------

bool ABConfiguration::write_bb_config(bool oninstall)
{ 
    if (!oninstall)
        if (bbconf->status() != BBCInterface::CIS_OK) return false;  //Perform check only if not on install
    return bbconf->write_backup_config(get_bb_config_filename()); 
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::config_file_readable() const
//      Purpose: IS boxbackup config readable?
//      Created: 4/1/08
//
// ------------------

bool ABConfiguration::bb_config_file_readable() const
{ 
    return 0==access(get_bb_config_filename().c_str(),R_OK);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::config_file_writable() const
//      Purpose: Is boxbackup config writable
//      Created: 4/1/08
//
// ------------------

bool ABConfiguration::bb_config_file_writable() const
{ 
    return 0==access(get_bb_config_filename().c_str(),W_OK);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_bb_config_filename() const
//      Purpose: returns boxbackup config filename
//      Created: 6/2/08
//
// ------------------

std::string ABConfiguration::get_bb_config_filename() const
{ 
    std::string output=""; 
    assert(settings!=NULL);
    output = settings->value("config_path").toString().toStdString();
    if (!check_bb_config_filename(output)) return std::string();
    return output;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_bb_config_dirname() const
//      Purpose: returns boxbackup config path
//      Created: 14/7/08
//
// ------------------

std::string ABConfiguration::get_bb_config_dirname() const
{ 
    return AB::system_bb_config_dir_default_location();
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_bb_data_dirname() const
//      Purpose: return boxbackup data directory path
//      Created: 22/2/08
//
// ------------------

std::string ABConfiguration::get_bb_data_dirname() const
{ 
    std::string output=""; 
    assert(settings!=NULL);
    output = settings->value("data_dir_path").toString().toStdString();
    return output;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_bb_keys_dirname() const
//      Purpose: return boxbackup keys directory path
//      Created: 22/2/08
//
// ------------------

std::string ABConfiguration::get_bb_keys_dirname() const
{ 
    std::string output=""; 
    assert(settings!=NULL);
    output = settings->value("bbkeys_dir_path").toString().toStdString();
    return output;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_bb_binary_filename() const
//      Purpose: returns bbackupd path
//      Created: 6/2/08
//
// ------------------

std::string ABConfiguration::get_bb_binary_filename() const
{ 
    std::string output=""; 
    assert(settings!=NULL);
    output = settings->value("bbackupd_path").toString().toStdString();
    return output;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::string ABConfiguration::get_openssl_filename() const
//      Purpose: returns openssl executable path
//      Created: 22/2/08
//
// ------------------

std::string ABConfiguration::get_openssl_filename() const
{ 
    std::string output=""; 
    assert(settings!=NULL);
    output = settings->value("openssl_bin_path").toString().toStdString();
    if (!check_openssl_filename(output)) return std::string();
    return output;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ABConfiguration::set_bb_config_filename() 
//      Purpose: sets boxbackup configuration path
//      Created: 24/2/08
//
// ------------------

bool ABConfiguration::set_bb_config_filename(std::string filename) throw()
{ 
    //Chequear que el fichero existe y tiene permisos de ejecución
    //if (0!=access(filename.c_str(),X_OK))
    //{return false;}
    if (!check_bb_config_filename(filename)) return false;
    settings->setValue("config_path", filename.c_str());
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ABConfiguration::set_bb_binary_filename() 
//      Purpose: sets bbackupd path
//      Created: 6/2/08
//
// ------------------

bool ABConfiguration::set_bb_binary_filename(std::string filename) throw()
{ 
    //Chequear que el fichero existe y tiene permisos de ejecución
    if (0!=access(filename.c_str(),X_OK))
    {return false;}
    settings->setValue("bbackupd_path", filename.c_str());
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::set_bb_keys_dirname() 
//      Purpose: sets boxbackup keys dir location
//      Created: 24/2/08
//
// ------------------

bool ABConfiguration::set_bb_keys_dirname(std::string filename) throw()
{ 
    //Chequear que el fichero existe y tiene permisos de ejecución
    if (0!=access(filename.c_str(),X_OK))
    {return false;}
    settings->setValue("bbkeys_dir_path", filename.c_str());
    return true;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::set_bb_data_dirname() 
//      Purpose: sets boxbackup data dir location
//      Created: 24/2/08
//
// ------------------

bool ABConfiguration::set_bb_data_dirname(std::string filename) throw()
{ 
    //Chequear que el fichero existe y tiene permisos de ejecución
    if (0!=access(filename.c_str(),X_OK))
    {return false;}
    settings->setValue("data_dir_path", filename.c_str());
    return true;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::set_openssl_filename() 
//      Purpose: sets openssl binary location
//      Created: 24/2/08
//
// ------------------

bool ABConfiguration::set_openssl_filename(std::string filename) throw()
{ 
    //Chequear que el fichero existe y tiene permisos de ejecución
    if (0!=access(filename.c_str(),X_OK))
    {return false;}
    settings->setValue("openssl_bin_path", filename.c_str());
    return true;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::load_config() 
//      Purpose: loads AdelinBackup configuration
//      Created: 7/2/08
//
// ------------------

bool ABConfiguration::load_config() throw()
{
    //Actualmente solo comprueba si está vacio o no
    if (settings->value("config_path")==QVariant())
        return false;
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::write_config() 
//      Purpose: stores AdelinBackup configuration
//      Created: 7/2/08
//
// ------------------

bool ABConfiguration::write_config() throw()
{
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_bb_config_filename(std::string filename) 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_bb_config_filename(std::string filename) const throw()
{
    return check_readable_filename(filename);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_bb_config_filename() 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_bb_config_filename() const throw()
{
    std::string filename = get_bb_config_filename();
    return check_bb_config_filename(filename);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_bb_binary_filename(std::string filename) 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_bb_binary_filename(std::string filename) const throw()
{
    return check_binary_filename(filename);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_bb_binary_filename() 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_bb_binary_filename() const throw()
{
    std::string filename = get_bb_binary_filename();
    return check_bb_binary_filename(filename);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_openssl_filename(std::string filename) 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_openssl_filename(std::string filename) const throw()
{
    return check_binary_filename(filename);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_openssl_filename() 
//      Purpose: Checks if filename is ok
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_openssl_filename() const throw()
{
    std::string filename = get_openssl_filename();
    return check_openssl_filename(filename);
}



// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_binary_filename(std::string filename) 
//      Purpose: checks if filename is a binary.
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_binary_filename(std::string filename) throw()
{
    if (filename == std::string()) return false;
    if (!QFile::exists(filename.c_str())) return false;
    QFileInfo myfile(filename.c_str());
    if (!myfile.isExecutable()) return false;
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_readable_filename(std::string filename) 
//      Purpose: 
//      Created: 10/3/08
//
// ------------------

bool ABConfiguration::check_readable_filename(std::string filename) throw()
{
    if (filename == std::string()) return false;
    if (!QFile::exists(filename.c_str())) return false;
    QFileInfo myfile(filename.c_str());
    if (!myfile.isReadable()) return false;
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::check_writable_filename(std::string filename) 
//      Purpose: 
//      Created: 25/3/08
//
// ------------------

bool ABConfiguration::check_writable_filename(std::string filename) throw()
{
    if (filename == std::string()) return false;
    if (!QFile::exists(filename.c_str())) return false;
    QFileInfo myfile(filename.c_str());
    if (!myfile.isWritable()) return false;
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ABConfiguration::installation_procedure() 
//      Purpose: Installation procedure for a new box
//      Created: 14/7/08
//
// ------------------


bool ABConfiguration::installation_procedure() throw()
{
    //TODO: Call to QDebug
    //TODO: Check if exists default configuration
    if (bb_config_file_writable())
    {
        //FIXME: Old restriction, check if new one is needed
        /*
        //-Si existe preguntar por el host al que conecta
        //--openadelin.es: return true
        //--otro: return false
        if(bbconf->stringprops["StoreHostname"]->get() != std::string("backup.openadelin.es"))
            return false;
        else
        {
            return true;
        }
        */

    }
    else
    {
        //Able to create default config?
        //--yes: return true
        //--no: return false
        QDir dir("/");
        dir.mkpath(AB::system_bb_config_dir_default_location().c_str());
        bbconf->load_default_values();
        return bbconf->write_backup_config(AB::system_bb_config_default_location());
    }
}


