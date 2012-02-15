/***************************************
 *            FRWLocationItem.h
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
//      Name:    FRWLocationItem
//      Purpose: Item perteneciente al modelo de representación de las Location
//      Created: 10/12/07
//
// ------------------

#ifndef FRWLOCATIONITEM_H_
#define FRWLOCATIONITEM_H_


#include "Box.h"
#include "BackupStoreConstants.h"
#include "BackupStoreDirectory.h"

#include "BBSInterface.h"

#include <QStringList>
#include <QVariant>

#include "assert.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    FRWLocationItem
//      Purpose: Item perteneciente al modelo de representación de las location
//      Created: 10/12/07
//
// ------------------

class FRWLocationItem
{
    public:
        FRWLocationItem(FRWLocationItem * parent = 0);
        ~FRWLocationItem();

        void appendChild(FRWLocationItem *child);
        void insertChild(FRWLocationItem *child, int);
        void set_data(const QList<QVariant> &data);
        FRWLocationItem *child(int row);
        int childCount() const;
        int columnCount() const;
        QVariant data(int column) const;
        int row() const;
        FRWLocationItem * parent();
        void set_id(int64_t);
        int64_t get_id() const{return id;} //TODO: Comprobar si tiene valor válido
        void setSelected(bool status){selected = status;};
        bool isSelected() const{return selected;}
        BBSNode get_node(){return node;}

    protected:
        BBSNode node;
        FRWLocationItem *parentItem;
        int64_t id;

    private:
        QList<FRWLocationItem*> childItems;
        QList<QVariant> itemData;
        bool selected;
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    FRWLocationFileDirItem
//      Purpose: Item Especifico para la representación de Ficheros
//      Created: 10/12/07
//
// ------------------

class FRWLocationFileDirItem: public FRWLocationItem
{
    public: 
      FRWLocationFileDirItem(){assert(false);}
      FRWLocationFileDirItem(BBSInterface *, FRWLocationItem * parent);
      //void set_id(int64_t);
      void set_id(const BackupStoreDirectory&, int64_t);
      void set_id(BackupStoreDirectory::Entry);
      void set_id_from_entry(BackupStoreDirectory::Entry myEntry) {set_id(myEntry);}
      virtual int64_t get_parent_id() const{return parentItem->get_id();}

    protected:
      BBSInterface * bbsinterface;
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    FRWLocationLocationItem
//      Purpose: Item Especifico para la representación de Locations
//      Created: 10/12/07
//
// ------------------

class FRWLocationLocationItem: public FRWLocationFileDirItem
{
    public: 
      FRWLocationLocationItem(){assert(false);}
      int64_t get_parent_id() const {return bbsinterface->root_directory_id();}
};

#endif /*FRWLOCATIONITEM_H_*/
