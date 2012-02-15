/***************************************
 *            FRWLocationItem.cpp
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
//      Name:    FRWLocationItem.cpp
//      Purpose: Item perteneciente al modelo de representación de las Location
//      Created: 10/12/07
//
// ------------------


#include "Box.h"
#include "BackupStoreConstants.h"
#include "BackupStoreDirectory.h"

#include "FRWLocationItem.h"

#include <QStringList>
#include <QtDebug>

#include <assert.h>

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationItem::FRWLocationItem(FRWLocationItem *parent)
//      Purpose: Fake Constructor
//      Created: 10/12/07
//
// ------------------


FRWLocationItem::FRWLocationItem(FRWLocationItem *parent)
{
    parentItem = parent;
    id = -1;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationItem::set_data(const QList<QVariant> &data)
//      Purpose: Da valor a los datos
//      Created: 17/12/07
//
// ------------------


void FRWLocationItem::set_data(const QList<QVariant> &data)
{
    itemData = data;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationItem::set_id(int64_t id)
//      Purpose: Establece el valor del id del Elemento
//      Created: 17/12/07
//
// ------------------

void FRWLocationItem::set_id(int64_t myid)
{
    qDebug() << "FRWLocationItem::set_id: Begin";
    id = myid;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationItem::~FRWLocationItem()
//      Purpose: Destructor
//      Created: 10/12/07
//
// ------------------


FRWLocationItem::~FRWLocationItem()
{
    qDeleteAll(childItems);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationItem::appendChild(FRWLocationItem *item)
//      Purpose: Añade un hijo al elemento. 
//      Created: 10/12/07
//
// ------------------


void FRWLocationItem::appendChild(FRWLocationItem *item)
{
    childItems.append(item);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationItem::insertChild(FRWLocationItem *item, int)
//      Purpose: Añade un hijo al elemento en la posicion int
//      Created: 18/12/07
//
// ------------------


void FRWLocationItem::insertChild(FRWLocationItem *item, int position)
{
    childItems.insert(position, item);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationItem* FRWLocationItem::child(int row)
//      Purpose: Devuelve un puntero al hijo cuyo indice es el pasado
//      Created: 10/12/07
//
// ------------------

FRWLocationItem* FRWLocationItem::child(int row)
{
    return childItems.value(row);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int FRWLocationItem::childCount() const
//      Purpose: Devuelve el número de hijos del elemento
//      Created: 10/12/07
//
// ------------------

int FRWLocationItem::childCount() const
{
    return childItems.count();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int FRWLocationItem::columnCount() const
//      Purpose: Devuelve el número de columnas de datos asociadas al elemento
//      Created: 10/12/07
//
// ------------------

//Nota: Las columnas de datos no son las mismas que las columnas que genera la relación padre-hijo entre elementos; son las columnas que aparecen en el view asociado al modelo

int FRWLocationItem::columnCount() const
{
    return 1;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QVariant FRWLocationItem::data(int column) const
//      Purpose: Devuelve el dato asociado a la columna pasada
//      Created: 10/12/07
//
// ------------------

QVariant FRWLocationItem::data(int column) const
{
    return itemData.value(column);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationItem* FRWLocationItem::parent() 
//      Purpose: Devuelve un puntero al elemento padre
//      Created: 10/12/07
//
// ------------------

FRWLocationItem* FRWLocationItem::parent()
{
    if (parentItem)
        return parentItem;
    return NULL;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int FRWLocationItem::row() 
//      Purpose: Devuelve la posición de hijo del elemento respecto a su padre
//      Created: 10/12/07
//
// ------------------

// Nota: Este row si es el referido a la jerarquia padre-hijo-hermano

int FRWLocationItem::row() const
{
    if (parentItem)
        return parentItem->childItems.indexOf(const_cast<FRWLocationItem*>(this));

    return 0;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationFileItem::FRWLocationFileItem(BBSInterface * bbsi, int64_t, FRWLocationItem * parent) 
//      Purpose: Constructor
//      Created: 12/12/07
//
// ------------------

FRWLocationFileDirItem::FRWLocationFileDirItem(BBSInterface * bbsi, FRWLocationItem * parent ):FRWLocationItem(parent)
{
    bbsinterface=bbsi;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationFileDirItem::set_id(int64_t id)  
//      Purpose: Establece el valor del id
//      Created: 17/12/07
//
// ------------------

//void FRWLocationFileDirItem::set_id(int64_t myid)
//{
//    qDebug() << "FRWLocationFileDirItem::set_id: Begin";
//    if (node!=BBSNode()) return;
//    QList<QVariant> tmplist;
//    BackupStoreDirectory parentdir;
//    if(!(bbsinterface->list_directory(get_parent_id(),BackupProtocolClientListDirectory::Flags_EXCLUDE_NOTHING, parentdir)))
//    {
//        qDebug() << "FRWLocationFileItem::FRWLocationFileItem: Error cargando el directorio con id:" << get_parent_id() << "\n";
//    }
//    node = BBSNode(*parentdir.FindEntryByID(myid),get_parent_id());
//    id = myid; //Estableciendo el valor de la variable
//    QVariant * tmp = new QVariant(node.get_name().c_str());
//    qDebug() << "FRWLocationFileItem::FRWLocationFileItem: Nombre de directorio:" << node.get_name().c_str() << "\n";
//    tmplist.append(*tmp);
//    set_data(tmplist);
//}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationFileDirItem::set_id(int64_t id)  
//      Purpose: Establece el valor del id
//      Created: 17/12/07
//
// ------------------

void FRWLocationFileDirItem::set_id(const BackupStoreDirectory & bsd, int64_t myid)
{
    qDebug() << "FRWLocationFileDirItem::set_id: Begin";
    if (node!=BBSNode()) return;
    QList<QVariant> tmplist;
    node = BBSNode(*(bsd.FindEntryByID(myid)),get_parent_id());
    id = myid; //Estableciendo el valor de la variable
    QVariant * tmp = new QVariant(node.get_name().c_str());
    qDebug() << "FRWLocationFileItem::FRWLocationFileItem: Nombre de directorio:" << node.get_name().c_str() << "\n";
    tmplist.append(*tmp);
    set_data(tmplist);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationFileDirItem::set_id(Entry)  
//      Purpose: Establece el valor del id
//      Created: 17/12/07
//
// ------------------

void FRWLocationFileDirItem::set_id(BackupStoreDirectory::Entry myentry)
{
    qDebug() << "FRWLocationFileDirItem::set_id: Begin";
    if (node!=BBSNode()) return;
    QList<QVariant> tmplist;
    node = BBSNode(myentry, get_parent_id());
    id = myentry.GetObjectID(); //Estableciendo el valor de la variable
    QVariant * tmp = new QVariant(node.get_name().c_str());
    qDebug() << "FRWLocationFileItem::FRWLocationFileItem: Nombre de directorio:" << node.get_name().c_str() << "\n";
    tmplist.append(*tmp);
    set_data(tmplist);
}

