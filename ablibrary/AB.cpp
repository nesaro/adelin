/***************************************
 *            AB.cpp
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
//      Name:    AB.cpp
//      Purpose: Global constants
//      Created: 14/7/08
//
// ------------------

#include "AB.h"
#if __GNUC__ >= 4
#include <cstdlib>
#endif


const std::string AB::system_bb_config_dir_default_location()
{
#ifdef WIN32
    return std::string("C:\\Program Files\\Adelin\\Backup\\Box Backup\\");
#else
    char * home = getenv("HOME");
    std::string shome(home);
    return shome + std::string("/.adelin/backup/");
#endif
}

const std::string AB::system_bb_config_default_location()
{
    return system_bb_config_dir_default_location() + std::string("bbackupd.conf");
}

const std::string AB::system_bb_key_dir_default_location()
{
#ifdef WIN32
    return system_bb_config_dir_default_location() + std::string("\\bbackupd\\");
#else
    return system_bb_config_dir_default_location() + std::string("/bbackupd/");
#endif
}

const std::string AB::system_bb_data_dir_default_location()
{
#ifdef WIN32
    return system_bb_config_dir_default_location() + std::string("\\data\\");
#else
    return system_bb_config_dir_default_location() + std::string("/data/");
#endif
}
