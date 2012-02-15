/***************************************
 *            BBSNode.h
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
//      Name:    BBSNode.h
//      Purpose: Clase de almacenamiento de las entry del servidor (Similar a BackupStoreDirectory::Entry)
//      Created: 14/12/07
//
// ------------------

#ifndef BBSNODE_H_
#define BBSNODE_H_

#include "Box.h"
#include "BackupStoreDirectory.h"


// --------------------------------------------------------------------------
//
// Class
//      Name:    BBSNode
//      Purpose: Clase de almacenamiento de las entry del servidor (Similar a BackupStoreDirectory::Entry)
//      Created: 14/12/07
//
// ------------------

enum NodeType{
    NT_FILE=0,
    NT_DIR,
    NT_LOCATION,
    NT_ROOT,
};


class BBSNode
{
    public:
        BBSNode(){id=-2;} //Nodo nulo para BBSNode, -1 es el nodo nulo de BoxBackup
        BBSNode(BackupStoreDirectory::Entry, int64_t);
        int64_t get_id() const{return id;}
        int64_t get_parent_id() const{return parentid;}
        int get_size() const{return size;}
        std::string get_name() const{return name;}
        enum NodeType get_type() const {return type;}
        time_t get_modtime() const { return modtime;}
        bool operator==(const BBSNode &other) const ;
        bool operator!=(const BBSNode &other) const ;
    private:
        int64_t parentid, id;
        std::string name;
        NodeType type;
        int size;
        time_t modtime;
};



#endif /*BBSNODE_H_*/
