/***************************************
 *            BBCInterface.cpp
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
//      Name:    BBCInterface.cpp
//      Purpose: BoxBackup Configuration Interfaces
//      Created: 19/11/07
//
// ------------------
//Boxbackup Includes

#include "Box.h"
#include "BackupDaemonConfigVerify.h"
#include "Configuration.h"


#include "BBCInterface.h"

#include "AB.h"
#include <QFile>
#include <QtDebug>

#include <iostream>
#include <map>
#include <fstream>

#define INIT_PROPS std::string str;
#define PATH_PROP(name) str = name;pathprops[str] = new PathProperty(str,this);
#define INT_PROP(name) str = name;intprops[str] = new IntProperty(str,this);
#define BOOL_PROP(name) str = name;boolprops[str] = new BoolProperty(str,this);
#define STR_PROP(name) str = name;stringprops[str] = new StringProperty(str,this);


// --------------------------------------------------------------------------
//
// Function
//      Name:    BBCInterface::BBCInterface
//      Purpose: Constructor
//      Created: 25/10/07
//
// ------------------

	
BBCInterface::BBCInterface()
{
	init_props();
    state=CIS_UNKNOWN;
    mLocations = new LocationContainer();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void BBCInterface::init_props
//      Purpose: default configuration values map initialization 
//      Created: 26/10/07
//
// ------------------

//Initializes same properties than boxbackup

void BBCInterface::init_props()
{
	INIT_PROPS
    BOOL_PROP("ExtendedLogging")
    BOOL_PROP("AutomaticBackup")
    STR_PROP("StoreHostname")
	INT_PROP("AccountNumber") 
    INT_PROP("UpdateStoreInterval") 
    INT_PROP("MinimumFileAge") 
    INT_PROP("MaxUploadWait") 
    INT_PROP("FileTrackingSizeThreshold") 
    INT_PROP("DiffingUploadSizeThreshold") 
    INT_PROP("MaximumDiffingTime") 
    INT_PROP("MaxFileTimeInFuture") 
    INT_PROP("KeepAliveTime")
    PATH_PROP("CertificateFile")
    PATH_PROP("CommandSocket")
    PATH_PROP("DataDirectory")
    PATH_PROP("PidFile")
    PATH_PROP("KeysFile")
    PATH_PROP("NotifyScript")
    PATH_PROP("PrivateKeyFile")
    PATH_PROP("StoreObjectInfoFile")
    PATH_PROP("SyncAllowScript")
    PATH_PROP("TrustedCAsFile")
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void BBCInterface::load_config
//      Purpose: loads boxbackup configuration
//      Created: 5/11/07
//
// ------------------


void BBCInterface::load_config(std::string filename)
{
    //FIXME: Translate and verify this block
// Boxi:
// std::string errs;
//  Carga el nombre del fichero como un buffer unicode
//	wxCharBuffer buf = rConfigFileName.mb_str(wxConvLibc);
// Llama a la función de carga de Configuration desde un fichero
//    mapConfig = Configuration::LoadAndVerify(buf.data(), NULL, errs);
    state = CIS_OK;
	std::string errs;
    //bool flag = false;
    std::fstream fin;
    fin.open(filename.c_str(),std::ios::in);
    if( !fin.is_open() )
    {
        state =	CIS_NOK;
        return; //File doesn't exist
    }
    fin.close();
    try{
        mapConfig = Configuration::LoadAndVerify(filename.c_str(), NULL, errs);
    }
    catch (...)
    {
        state =	CIS_NOK;
        return;
    }
    if(0!=access(filename.c_str(),W_OK)) state = CIS_NWRITE; // Check Write Perms

    //parameter initialization
    boolprops["ExtendedLogging"]->SetFrom(mapConfig.get());
    boolprops["AutomaticBackup"]->SetFrom(mapConfig.get());
    stringprops["StoreHostname"]->SetFrom(mapConfig.get());
    intprops["AccountNumber"]->SetFrom(mapConfig.get());
    intprops["UpdateStoreInterval"]->SetFrom(mapConfig.get());
    intprops["MinimumFileAge"]->SetFrom(mapConfig.get());
    intprops["MaxUploadWait"]->SetFrom(mapConfig.get());
    intprops["FileTrackingSizeThreshold"]->SetFrom(mapConfig.get());
    intprops["DiffingUploadSizeThreshold"]->SetFrom(mapConfig.get());
    intprops["MaximumDiffingTime"]->SetFrom(mapConfig.get());
    intprops["MaxFileTimeInFuture"]->SetFrom(mapConfig.get());
    intprops["KeepAliveTime"]->SetFrom(mapConfig.get());
    pathprops["CertificateFile"]->SetFrom(mapConfig.get());
    pathprops["CommandSocket"]->SetFrom(mapConfig.get());
    pathprops["DataDirectory"]->SetFrom(mapConfig.get());
    pathprops["KeysFile"]->SetFrom(mapConfig.get());
    pathprops["NotifyScript"]->SetFrom(mapConfig.get());
    pathprops["PrivateKeyFile"]->SetFrom(mapConfig.get());
    pathprops["StoreObjectInfoFile"]->SetFrom(mapConfig.get());
    pathprops["SyncAllowScript"]->SetFrom(mapConfig.get());
    pathprops["TrustedCAsFile"]->SetFrom(mapConfig.get());

    pathprops["PidFile"]->SetFrom(&(mapConfig->GetSubConfiguration("Server")));

    //loading locations
    load_locations();
    if (intprops["AccountNumber"]->get() == 0)
        state=CIS_INVALID_ID;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void BBCInterface::load_default_values()
//      Purpose: Carga la configuración por defecto
//      Created: 16/2/08
//
// ------------------


void BBCInterface::load_default_values()
{
    //TODO: windows portability
    stringprops["StoreHostname"]->set("backup.openadelin.es"); //FIXME: ask user
    pathprops["DataDirectory"]->set(AB::system_bb_data_dir_default_location());
    std::string notify(AB::system_bb_data_dir_default_location());
    notify += std::string("NotifySysadmin.sh");
    pathprops["NotifyScript"]->set(notify); //TODO: arch dependant. should be in AB?, or a class constant?
    intprops["UpdateStoreInterval"]->set(3609);
    intprops["MinimumFileAge"]->set(21600);
    intprops["MaxUploadWait"]->set(86400);
    intprops["FileTrackingSizeThreshold"]->set(65535);
    intprops["MaximumDiffingTime"]->set(20);
    intprops["DiffingUploadSizeThreshold"]->set(8192);
    boolprops["ExtendedLogging"]->set(false);
    pathprops["SyncAllowScript"]->set("");
    std::string socket(AB::system_bb_data_dir_default_location());
    std::string pid(AB::system_bb_data_dir_default_location());
    socket += std::string("bbackupd.sock");
    pid += std::string("bbackupd.pid");
    pathprops["CommandSocket"]->set(socket);
    pathprops["PidFile"]->set(pid);
}

// --------------------------------------------------------------------------
//
// Function
//      Name: BBCStatus BBCInterface::status()    
//      Purpose: Returns current configuration status
//      Created: 5/11/07
//
// ------------------

BBCInterface::BBCStatus BBCInterface::status() const
{
    return state;
}


// --------------------------------------------------------------------------
//
// Function
//      Name: void BBCInterface::notify_property_change(Property *)
//      Purpose: What to do when a property changes
//      Created: 12/11/07
//
// ------------------


void BBCInterface::notify_property_change(const Property *)
{
//TODO
}

// --------------------------------------------------------------------------
//
// Function
//      Name: bool BBCInterface::load_locations()
//      Purpose: Loads locations from configuration
//      Created: 28/11/07
//
// ------------------


bool BBCInterface::load_locations()
{
    std::vector<Location> templocations;
 const Configuration& rLocations =
        mapConfig->GetSubConfiguration("BackupLocations");

    for(std::list<std::pair<std::string, Configuration> >::const_iterator i =
            rLocations.mSubConfigurations.begin();
            i != rLocations.mSubConfigurations.end();
            i++)
    {
        std::string name = i->first;
        std::string path = i->second.GetKeyValue("Path");

        Location *pLoc = new Location(std::string(name.c_str()),
                std::string(path.c_str()));

        RuleContainer* pExcluded = new RuleContainer(i->second);
        pLoc->set_exclude_list(pExcluded);

        templocations.push_back(*pLoc);
    }
    mLocations->clear();
    mLocations->set_locations_from_vector(templocations);

    //SetClean();
    //NotifyListeners();
    return true; //TODO: Handle errors
}

// --------------------------------------------------------------------------
//
// Function
//      Name: bool BBCInterface::write_backup_config(std::string filename)
//      Purpose: Saves configuration in filename arg
//      Created: 3/12/07
//
// ------------------


bool BBCInterface::write_backup_config(std::string filename)
{
    QFile file(filename.c_str());
    qDebug() << filename.c_str();
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text)) { return false; }
    QTextStream out(&file);
    out << "# This Box Backup configuration file was written by AdelinSoft BOS\n";
    out << "# This file is automatically generated. Do not edit by hand!\n";
    out << "\n";

    if (intprops.find("AccountNumber") != intprops.end())
    {
        out << "AccountNumber = 0x";
        out << hex << intprops["AccountNumber"]->get();
        out  << "\n";
    }
    write_backup_config_write_str_prop(out, "StoreHostname");
    write_backup_config_write_path_prop(out, "KeysFile");
    write_backup_config_write_path_prop(out, "CertificateFile");
    write_backup_config_write_path_prop(out, "PrivateKeyFile");
    write_backup_config_write_path_prop(out, "TrustedCAsFile");
    write_backup_config_write_path_prop(out, "DataDirectory");
    write_backup_config_write_path_prop(out, "NotifyScript");
    write_backup_config_write_int_prop(out, "UpdateStoreInterval");
    write_backup_config_write_int_prop(out, "MinimumFileAge");
    write_backup_config_write_int_prop(out, "MaxUploadWait");
    write_backup_config_write_int_prop(out, "FileTrackingSizeThreshold");
    write_backup_config_write_int_prop(out, "DiffingUploadSizeThreshold");
    write_backup_config_write_int_prop(out, "MaximumDiffingTime");
    write_backup_config_write_bool_prop(out, "ExtendedLogging");
    write_backup_config_write_path_prop(out, "SyncAllowScript");
    write_backup_config_write_path_prop(out, "CommandSocket");

    if ((!(pathprops.find("PidFile") == pathprops.end()) && 
                (pathprops["PidFile"]->is_configured()))){
        std::string pidbuf;
        pathprops["PidFile"]->GetInto(pidbuf);
        out << "Server\n{\n\tPidFile = "<< pidbuf.c_str() << "\n}\n";
    }

    const std::vector<Location>& rLocations = mLocations->get_locations_vector();
    out << "BackupLocations\n{\n";

    for (size_t i = 0; i < rLocations.size(); i++) {
        const Location* pLoc = &rLocations[i];
        out << "\t" << pLoc->get_name().c_str() << "\n\t{\n\t\tPath = " << pLoc->get_path().c_str() << "\n";

        const std::vector<Rule> rEntries =
            pLoc->get_exclude_list().get_entries_vector();

        for (size_t l = 0; l < rEntries.size(); l++) {
            Rule pEntry = rEntries[l];
         out << "\t\t" << pEntry.ToString().c_str() << "\n";
        }

    out << "\t}\n";
    }

    out << "}\n";
    //file.Commit();

    //// clean configuration
    //Load(rConfigFileName);

    state = CIS_OK;
    return true;

}


