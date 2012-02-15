/***************************************
 *            AB.h
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
//      Name:    AB.h
//      Purpose: Global constants
//      Created: 5/2/08
//
// ------------------

#ifndef AB_H_
#define AB_H_
#include <string>

//There are 2 ways to do multiple definitions:
//- const vars
//- extern vars, must be defined in .cpp (linker will do rest of the job)

namespace AB 
{

    const char ABVERSION[] = "0"; //this is ab version, each component has its own
    const char ABLIBRARYVERSION[] = "0.1"; //this is library version. You should read version as ABVERSION.ABXXXVERSION
    extern const std::string system_bb_config_dir_default_location() ;
    extern const std::string system_bb_config_default_location() ;
    extern const std::string system_bb_key_dir_default_location() ;
    extern const std::string system_bb_data_dir_default_location() ;
#ifdef WIN32
    const char system_bb_binary_default_location[] = "C:\\Program Files\\Adelin\\Backup\\Box Backup\\bbackupd.exe";
    const char system_bb_binary_location_1[] = "C:\\Program Files\\Box Backup\\bbackupd.exe";
    const char system_bb_binary_dir_default_location[] = "C:\\Program Files\\Adelin\\Backup\\Box Backup\\";
    const char system_openssl_bin_default_location[] = "C:\\Program Files\\Adelin\\Backup\\Box Backup\\Openssl.exe";
    const char system_ab_interface_default_location[] = "C:\\Program Files\\Adelin\\Backup\\AdelinBackup.exe";
    const char system_publiclient_binary_location[] = "C:\\Program Files\\Adelin\\publiclient\\publiclient.exe";
#else
    const char system_bb_binary_default_location[] = "/usr/bin/bbackupd";
    const char system_bb_binary_location_1[] = "/usr/sbin/bbackupd";
    const char system_bb_binary_dir_default_location[] = "/usr/bin/";
    const char system_openssl_bin_default_location[] = "/usr/bin/openssl"; 
    const char system_ab_interface_default_location[] = "/usr/bin/adelinbackup";
    const char system_publiclient_binary_location[] = "/usr/local/bin/publiclient";
#endif
} 

#endif /*AB_H_*/
