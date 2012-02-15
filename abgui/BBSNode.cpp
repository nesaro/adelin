/***************************************
 *            BBSNode.cpp
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
//      Name:    BBSNode.cpp
//      Purpose: Clase de almacenamiento de las entry del servidor (Similar a BackupStoreDirectory::Entry)
//      Created: 14/12/07
//
// ------------------



#include "BBSNode.h"

#include <QtDebug>

// --------------------------------------------------------------------------
//
// File
//      Name:    BBSNode::BBSNode(BackupStoreDirectory::Entry)
//      Purpose: Constructor desde un Entry
//      Created: 18/12/07
//
// ------------------

BBSNode::BBSNode(BackupStoreDirectory::Entry entry, int64_t myparentid){
    qDebug() << "BBSNode::BBSNode:Begin";
    BackupStoreFilenameClear clear(entry.GetName());
    id = entry.GetObjectID();
    name = clear.GetClearFilename();
    size = entry.GetSizeInBlocks();
    parentid = myparentid;
    modtime = BoxTimeToSeconds(entry.GetModificationTime());
    type = NT_LOCATION;
    if ((entry.GetFlags() & BackupStoreDirectory::Entry::Flags_Dir)!=0) type = NT_DIR;
    if ((entry.GetFlags() & BackupStoreDirectory::Entry::Flags_File)!=0) type = NT_FILE;
    if (parentid == -1)
    {
        type = NT_ROOT;
    }
    qDebug() << type;
}

bool BBSNode::operator==(const BBSNode &other) const {
    return id==other.id;
}
bool BBSNode::operator!=(const BBSNode &other) const {
    return !(*this==other);
}
