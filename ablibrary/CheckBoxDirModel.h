//
// Cleversafe open-source code header - Version 1.1 - December 1, 2006
//
// Cleversafe Dispersed Storage(TM) is software for secure, private and
// reliable storage of the world's data using information dispersal.
//
// Copyright (C) 2005-2007 Cleversafe, Inc.
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License
// as published by the Free Software Foundation; either version 2
// of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
// USA.
//
// Contact Information: Cleversafe, 10 W. 35th Street, 16th Floor #84,
// Chicago IL 60616
// email licensing@cleversafe.org
//
// END-OF-HEADER

#ifndef CHECKBOXDIRMODEL_H
#define CHECKBOXDIRMODEL_H

#include <QDirModel>
#include <QModelIndex>
#include <QHash>
#include <QtGlobal>
#include <QDir>
#include <QVariant>
#include <QStringList>
#include <QThread>

#include <vector>
#include <string>

class CheckBoxDirModel : public QDirModel
{
    Q_OBJECT
    public:

      CheckBoxDirModel();
      bool setData ( const QModelIndex &, const QVariant &, int role );
        Qt::ItemFlags flags ( const QModelIndex & ) const;
        QVariant data ( const QModelIndex &, int role ) const;

        void fillFileVectors(QModelIndex root);  //Doesn't take into account filters
      bool fillFileVectorsFiltered(QModelIndex root,
                           std::vector<std::string> extensions);

      bool is_checked(const QModelIndex &) const;
      void clearVectors();

      std::vector <std::string> getFileVector();
      std::vector <std::string> getFolderVector();
      std::vector <std::string> get_dir_selection(){ clearVectors(); fillFileVectors(index("/")); return getFolderVector(); } //FIXME: Translate and verify this: FIXME: La ruta tiene que ser via macro, por aquello de los diferentes SO
      int get_number_folders_checked(){ clearVectors(); fillFileVectors(index("/")); return getNumberFoldersChecked(); } //FIXME: Translate and verify this: FIXME: La ruta tiene que ser via macro, por aquello de los diferentes SO. TODO: Documentar

      int getNumberFilesChecked();
      int getNumberFoldersChecked();

      void setSelection( std::vector<std::string>& paths );
      QDir dir_from_modelindex(const QModelIndex &);

   public slots:
      void collapsed ( const QModelIndex & index );
      void expanded ( const QModelIndex & index );

   signals:
      //void sizeUpdated ( qint64 size );
      //void totalNumFilesUpdated ( int numTotalFiles );
      void selectionChanged();

   private:

      QModelIndex checkChildren(const QModelIndex& modelIndex, bool value);
      QModelIndex checkParents(const QModelIndex& modelIndex, bool value);
      QVariant convertState(int value) const;
      bool hasSiblings(const QModelIndex& index);
      bool areSiblingsChecked(const QModelIndex& index);
      bool areSiblingsUnchecked(const QModelIndex& index);

      bool isExtensionValid(QModelIndex index, std::vector<std::string> extensions);
      std::string getExtension(std::string fileName);

      /*void changeSelection( const QModelIndex& index, bool add );
      void fileSelectionChanged( const QModelIndex& root, bool add, bool selectAll );
      void folderSelectionChanged( const QModelIndex& root, bool add, bool selectAll );
      */

      QHash <qint64, int > checkBoxMap;
      QHash <qint64, int > expandedMap;

      std::vector <std::string> fileVector;
      std::vector <std::string> folderVector;
      int numberFilesChecked;       // total top level files checked
      int numberFoldersChecked;     // total top level folders checked

      //int totalFilesSelected;       // total files checked
      //qint64 totalSizeSelected;     // size in bytes
      //int currentlyShownTotalFiles;
      //int currentlyShownTotalSize;
};
#endif

