/***************************************
 *            FRWLocationModel.cpp
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
//      Name:    FRWLocationModel.cpp
//      Purpose: Modelo de representación de las Location
//      Created: 10/12/07
//
// ------------------


#include "FRWLocationItem.h"
#include "FRWLocationModel.h"

#include <QtGui>

#include <assert.h>
// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationModel::FRWLocationModel(const QString &data, QObject *parent) 
//      Purpose: Constructor
//      Created: 10/12/07
//
// ------------------

FRWLocationModel::FRWLocationModel(BBSInterface * bbsi, QObject *parent) : QAbstractItemModel(parent)
{
    bbsinterface = bbsi;
    QList<QVariant> rootData;
    rootData << tr("Path");
    rootItem = new FRWLocationItem();
    rootItem->set_data(rootData);
    rootItem->set_id(bbsinterface->root_directory_id());
    
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationModel::~FRWLocationModel()
//      Purpose: Destructor
//      Created: 10/12/07
//
// ------------------

FRWLocationModel::~FRWLocationModel()
{
    delete rootItem;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int FRWLocationModel::columnCount(const QModelIndex &parent)
//      Purpose: Devuelve el número de columnas del elemento asociado al indice
//      Created: 10/12/07
//
// ------------------

//Nota: Se refiere al número de columnas que se muestran, no a las jerarquicas

int FRWLocationModel::columnCount(const QModelIndex &parent) const
{
    return 1;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QVariant FRWLocationModel::data(const QModelIndex &index, int role)
//      Purpose: Devuelve el contenido del elemento asociado al indice.
//      Created: 10/12/07
//
// ------------------

QVariant FRWLocationModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole && role != Qt::UserRole) return QVariant();
    if (!index.isValid()) return QVariant();
    //qDebug() << "FRWLocationModel::data: fila y columna solicitada:" << index.row() << "," << index.column();
    FRWLocationItem *item = get_item(index);
    if (role == Qt::DisplayRole) { 
        if (index.column()==0) 
        {
        //    qDebug() << "FRWLocationModel::data: Valor devuelto:" << item->data(0);
            return item->data(0);
        }
        qDebug() << "FRWLocationModel::data: Columna incorrecta";
        return QVariant();
    }
    if (role == Qt::UserRole) { 
        //qDebug() << "FRWLocationModel::data: Valor devuelto:" << item->get_id();
        return QVariant((int)item->get_id());
    }
    return QVariant();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    Qt::ItemFlags FRWLocationModel::flags(const QModelIndex &index) const
//      Purpose: Devuelve las flags asociadas al item
//      Created: 10/12/07
//
// ------------------

Qt::ItemFlags FRWLocationModel::flags(const QModelIndex &index) const
{
    if (!index.isValid())
        return 0;
    //if(index==createIndex(0,0,rootItem)) return Qt::ItemIsEnabled; //Hace falta?

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QVariant FRWLocationModel::headerData(int section, Qt::Orientation orientation, int role) const
//      Purpose: Si se indica el rol y la sección correcta, se devuelve el contenido del elemento raíz
//      Created: 10/12/07
//
// ------------------

QVariant FRWLocationModel::headerData(int section, Qt::Orientation orientation,
        int role) const
{
    if (section!=0) return QVariant();
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return rootItem->data(section);

    return QVariant();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QModelIndex FRWLocationModel::index(int row, int column, const QModelIndex &parent) const
//      Purpose: Devuelve el indice asociado a row,column partiendo de la posicion del elemento parent
//      Created: 10/12/07
//
// ------------------

QModelIndex FRWLocationModel::index(int row, int column, const QModelIndex &parent) const
{
    qDebug() << "FRWLocationModel::index: Init";
    if (column!=0 || row < 0 || parent.column()>0) return QModelIndex();
    if (!hasIndex(row, column, parent)) return QModelIndex();

    FRWLocationItem *parentItem = get_item(parent);
    FRWLocationItem *childItem = parentItem->child(row);
    if (childItem) return createIndex(row, column, childItem);
    else return QModelIndex();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QModelIndex FRWLocationModel::parent(const QModelIndex &index) const
//      Purpose: Devuelve enlace al elemento padre del elemento pasado
//      Created: 10/12/07
//
// ------------------

QModelIndex FRWLocationModel::parent(const QModelIndex &index) const
{
    qDebug() << "FRWLocationModel::parent: Init";
    if (!index.isValid()) return QModelIndex();
    FRWLocationItem *childItem = get_item(index);
    FRWLocationItem *parentItem = childItem->parent();
    if (parentItem == rootItem) return QModelIndex();
    return createIndex(parentItem->row(), 0, parentItem);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int FRWLocationModel::rowCount(const QModelIndex &parent) const
//      Purpose: Devuelve el número de hermanos de un elemento (Contando el propio elemento)
//      Created: 10/12/07
//
// ------------------

int FRWLocationModel::rowCount(const QModelIndex &parent) const
{
    qDebug() << "FRWLocationModel::rowCount: Comenzando Función";
    FRWLocationItem *parentItem;
    if (parent.column() > 0) return 0;
    parentItem=get_item(parent);
    qDebug() << "FRWLocationModel::rowCount: Id solcitado:"<< parentItem->get_id();
    qDebug() << "FRWLocationModel::rowCount: Fila, columna:"<< parent.column() << "," <<  parent.row();
    qDebug() << "FRWLocationModel::rowCount: Valor devuelto:" << parentItem->childCount();
    return parentItem->childCount();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWLocationModel::setupModelData(FRWLocationItem *parent)
//      Purpose: Rellena inicialmente el modelo con las Locations
//      Created: 10/12/07
//
// ------------------

void FRWLocationModel::setupModelData()
{
   qDebug() << "FRWLocationModel::setupModelData: Init";
   BackupStoreDirectory dir;
   int64_t id = bbsinterface->root_directory_id();
   QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug() << "FRWLocationModel::setupModelData: Vamos a cargar un directorio";
   if(!(bbsinterface->list_directory(id,BackupProtocolClientListDirectory::Flags_EXCLUDE_NOTHING, dir)))
   {
       qDebug() << "FRWLocationModel::setupModelData: Error cargando el directorio Raíz del servidor";
   }
   BackupStoreDirectory::Iterator i = BackupStoreDirectory::Iterator(dir);
   BackupStoreDirectory::Entry *en = 0;
   int j=0;
   while ((en = i.Next()) != 0)
   {
       BackupStoreFilenameClear clear(en->GetName());
       qDebug() << "FRWLocationModel::setupModelData: Location evaluado:" << clear.GetClearFilename().c_str();
       FRWLocationFileDirItem * item = new FRWLocationFileDirItem(bbsinterface,rootItem);
       rootItem->insertChild(static_cast<FRWLocationItem*>(item),j);
       //rootItem->child(j)->set_id(en->GetObjectID());
       static_cast<FRWLocationFileDirItem*>(rootItem->child(j))->set_id_from_entry(*en);
       j++;
   }
   QApplication::restoreOverrideCursor();
   emit layoutChanged();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBSNode FRWLocationModel::get_node(FRWLocationItem *parent)
//      Purpose: Devuelve información sobre el nodo asociado
//      Created: 14/12/07
//
// ------------------

BBSNode FRWLocationModel::get_node(const QModelIndex & index) const
{
    qDebug() << "FRWLocationModel::get_node: Init";
    if (!index.isValid())
        assert(false);
    FRWLocationItem *item = get_item(index);
    assert(item!=rootItem);
    FRWLocationItem *parent2 = item->parent();
    assert(item!=NULL); //FIXME Excepcion
    assert(parent2!=NULL); //FIXME Excepcion
    int64_t id1,id2;
    id1=parent2->get_id();
    id2=item->get_id();
    return bbsinterface->get_bbsnode(id1, id2);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool FRWLocationModel::insertRows(int row, int count, const QModelIndex & parent)
//      Purpose: Inserta una Fila
//      Created: 17/12/07
//
// ------------------

bool FRWLocationModel::insertRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "FRWLocationModel::insertRows: Comienzo de función";
    if (parent==QModelIndex())
    {
        qDebug() << "FRWLocationModel::insertRows: Invalid parent";
        return false;
    }
    FRWLocationItem * tmpparent;
    tmpparent = static_cast<FRWLocationItem*>(parent.internalPointer()); 
    assert (tmpparent != NULL); //FIXME Excepción
    qDebug() << "FRWLocationModel::insertRows: Llamando a beginInsertRows";
    beginInsertRows(parent,row, row+count-1);
    for (int i=row;i<(row+count);i++)
    {
        FRWLocationFileDirItem * item = new FRWLocationFileDirItem(bbsinterface,tmpparent);
        tmpparent->insertChild(static_cast<FRWLocationItem*>(item),i);
    }
    endInsertRows();
    qDebug() << "FRWLocationModel::insertRows: Llamando a endInsertRows";
    return true;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool FRWLocationModel::insertColumns(int column, int count, const QModelIndex & parent)
//      Purpose: Inserta una Columna
//      Created: 17/12/07
//
// ------------------

bool FRWLocationModel::insertColumns(int column, int count, const QModelIndex &parent)
{
    qDebug() << "FRWLocationModel::insertColumns: Comienzo de función";
    return false;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool FRWLocationModel::removeRows(int row, int count, const QModelIndex & parent)
//      Purpose: Removea una Fila
//      Created: 17/12/07
//
// ------------------

bool FRWLocationModel::removeRows(int row, int count, const QModelIndex &parent)
{
    qDebug() << "FRWLocationModel::removeRows: Comienzo de función";
    beginRemoveRows(parent,row, row+count);
    endRemoveRows();
    return false;
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    bool FRWLocationModel::removeColumns(int row, int count, const QModelIndex & parent)
//      Purpose: Removea una Columna
//      Created: 17/12/07
//
// ------------------

bool FRWLocationModel::removeColumns(int column, int count, const QModelIndex &parent)
{
    qDebug() << "FRWLocationModel::removeColumns: Init";
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool FRWLocationModel::hasChildren(const QModelIndex &parent) const
//      Purpose: Determina si el nodo tiene hijos o no. Reimplementación de QAbstractItemModel
//      Created: 20/12/07
//
// ------------------

bool FRWLocationModel::hasChildren(const QModelIndex &parent) const
{
    qDebug() << "FRWLocationModel::hasChildren: Init";
    FRWLocationItem *parentItem;
    if (parent.column() > 0)
    {
	    qDebug() << "FRWLocationModel::hasChildren: Columna Incorrecta";
        return 0;
    }
    parentItem=get_item(parent);
    if (parentItem==rootItem)
    {
	    qDebug() << "FRWLocationModel::hasChildren: rootItem";
	    return (rootItem->childCount()>0);
    }
    return (parentItem->get_node().get_type()!=NT_FILE);
}

bool FRWLocationModel::populate_at_index(const QModelIndex & parent)
{
    qDebug() << "FRWLocationModel::populate_at_index: Init";
    FRWLocationItem *parentItem;
    //if (parent == QModelIndex()){ return false;}
    if (parent.column() != 0){ return false;}
    parentItem = get_item(parent);
    if (parentItem->childCount()!=0)  return false;
    BackupStoreDirectory dir2;
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    qDebug() << "FRWLocationModel::populate_at_index: Vamos a cargar un directorio";
    if(!(bbsinterface->list_directory(parentItem->get_id(),BackupProtocolClientListDirectory::Flags_EXCLUDE_NOTHING, dir2)))
    {
        qDebug() << "FRWLocationModel::populate_at_index: Error cargando el directorio";
    }
    BackupStoreDirectory::Iterator localdir(dir2);
    BackupStoreDirectory::Entry * localen = 0;
    int k=0;
    while ((localen = localdir.Next()) != 0)
    {
        qDebug() << "FRWLocationModel::populate_at_index: Tocando:" << parentItem->row() << ",0";
        insertRows(k,1,createIndex(parentItem->row(),0,parentItem));
        qDebug() << "FRWLocationModel::populate_at_index: Añadiendo hijo" << k;
        try { 
            QMutexLocker tmpmutex(&mutex);
            //parentItem->child(k)->set_id(dir2,localen->GetObjectID());
            //parentItem->child(k)->set_id(localen->GetObjectID());
            
            if (static_cast<FRWLocationFileDirItem*>(parentItem->child(k))->get_id() < 0)
            {
                static_cast<FRWLocationFileDirItem*>(parentItem->child(k))->set_id_from_entry(*localen);
            }
        }
        catch (BoxException& e)  {
            qDebug() << "FRWLocationModel::populate_at_index: Error while fetching object property";
            QApplication::restoreOverrideCursor();
            return false;
        }
        k++;
    }
    QApplication::restoreOverrideCursor();
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWLocationItem* FRWLocationModel::get_item(const QModelIndex &theitem) const
//      Purpose: Determina si el nodo tiene hijos o no. Reimplementación de QAbstractItemModel
//      Created: 25/2/08
//
// ------------------

FRWLocationItem* FRWLocationModel::get_item(const QModelIndex &theitem) const
{
    //qDebug() << "FRWLocationModel::get_item: begin";
    if (theitem.isValid()) {
        FRWLocationItem *item = static_cast<FRWLocationItem*>(theitem.internalPointer());
        if (item) return item;
    }
    return rootItem;
}

