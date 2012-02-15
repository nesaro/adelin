/***************************************
 *            FRWLocationModel.h
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
//      Name:    FRWLocationModel
//      Purpose: Modelo de representación de las Location
//      Created: 10/12/07
//
// ------------------

#ifndef FRWLOCATIONMODEL_H_
#define FRWLOCATIONMODEL_H_

#include "Location.h"
#include "BBSInterface.h"

#include <QItemSelectionModel>
#include <QAbstractItemModel>
#include <QModelIndex>
#include <QVariant>

class FRWLocationItem;

// --------------------------------------------------------------------------
//
// Class
//      Name:    FRWLocationModel
//      Purpose: Modelo de representación de las location
//      Created: 10/12/07
//
// ------------------
class FRWLocationModel : public QAbstractItemModel
{
    Q_OBJECT

    public:
        FRWLocationModel(BBSInterface *, QObject *parent = 0);
        ~FRWLocationModel();

        QVariant data(const QModelIndex &index, int role) const;
        Qt::ItemFlags flags(const QModelIndex &index) const;
        QVariant headerData(int section, Qt::Orientation orientation,
                int role = Qt::DisplayRole) const;
        QModelIndex index(int row, int column,
                const QModelIndex &parent = QModelIndex()) const;
        QModelIndex parent(const QModelIndex &index) const;
        int rowCount(const QModelIndex &parent = QModelIndex()) const;
        bool hasChildren ( const QModelIndex & parent = QModelIndex() ) const;
        int columnCount(const QModelIndex &parent = QModelIndex()) const;
        BBSNode get_node(const QModelIndex & index) const;
        //Requeridas para el manejo de la selección
        bool insertRows(int row, int count, const QModelIndex &parent = QModelIndex());
        bool insertColumns(int column, int count, const QModelIndex &parent = QModelIndex());
        bool removeRows(int row, int count, const QModelIndex &parent = QModelIndex());
        bool removeColumns(int column, int count, const QModelIndex &parent = QModelIndex());
        //QModelIndex root_index() const { return createIndex(0,0,rootItem);} //FIXME: Verificar que se devuelve el QModelIndex asociado a root
        FRWLocationItem* get_item(const QModelIndex & theitem) const;
        void setupModelData();

    private slots:
        //Lazy load
        bool populate_at_index(const QModelIndex & parent = QModelIndex());

    private:
        FRWLocationItem *rootItem;
        BBSInterface * bbsinterface;
        QMutex mutex;
};

#endif /*FRWLOCATIONMODEL_H_*/


