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

#include "CheckBoxDirModel.h"

//#include <util/StringUtil.h>
#include <QMessageBox>


//////////////////////////////////////////////////////////////////

CheckBoxDirModel::CheckBoxDirModel()
:  QDirModel(),
   numberFilesChecked (0),
   numberFoldersChecked (0)
//   totalFilesSelected(0),
//   totalSizeSelected(0)
{
}


// Stores the checkbox state

bool CheckBoxDirModel::setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole )
{
   qint64 indexId = index.internalId ();
   int oldMapValue, newMapValue;
   QModelIndex lastIndexChanged = index;
   QModelIndex firstIndexChanged = index;


   if (index.isValid())  //&& role == Qt::EditRole)
    {
      //if (value == Qt::Checked)
      //   changeSelection ( index, true );
      //else if (value == Qt::Unchecked)
      //{
      //   changeSelection ( index, false );
      //}

      if( !checkBoxMap.contains(indexId) )
      {
         checkBoxMap.insert( indexId, true );
         firstIndexChanged = checkParents(index, true);
         if( QAbstractItemModel::hasChildren(index) && expandedMap.value(indexId) == 1)
            lastIndexChanged = checkChildren( index, true );
      }
      else
      {
         oldMapValue = checkBoxMap.value(indexId);
         if (oldMapValue == 0)
            newMapValue = 1;
         else
            newMapValue = 0;

         checkBoxMap.insert(indexId, newMapValue);
         firstIndexChanged = checkParents(index, newMapValue);

         if (QAbstractItemModel::hasChildren(index) && expandedMap.value(indexId) == 1)
         {
            lastIndexChanged = checkChildren( index, newMapValue );
         }
      }

      emit dataChanged(index, lastIndexChanged);//firstIndexChanged
      emit dataChanged(firstIndexChanged, index);
      emit selectionChanged();

      return true;

   }
   return false;
}

Qt::ItemFlags CheckBoxDirModel::flags ( const QModelIndex & index ) const
{
     return QDirModel::flags(index) | Qt::ItemIsUserCheckable | Qt::ItemIsTristate;
     // |Qt::ItemIsEditable;
}

////////////////////////////////////////////////////////////////////////////////

// setData() returns the checkbox state from the Hashtable
QVariant CheckBoxDirModel::data ( const QModelIndex & index, int role = Qt::DisplayRole ) const
{
    qint64 indexId = index.internalId ();

    if (role == Qt::CheckStateRole && index.isValid() && index.column() == 0)
    {
      if ( !checkBoxMap.isEmpty() && checkBoxMap.contains(indexId) )
      {
          return convertState(checkBoxMap.value(indexId));
      }
      else
      {
         return Qt::Unchecked;
      }
    }
    return QDirModel::data ( index, role );
}

////////////////////////////////////////////////////////////////////////////////

// returns the top-most index modified
QModelIndex CheckBoxDirModel::checkChildren(const QModelIndex& modelIndex, bool value)
{
   QDir directory;
   QModelIndex tmpIndex;

   if( modelIndex.isValid() && QAbstractItemModel::hasChildren(modelIndex) )
   {
      int a = 0;
      tmpIndex = QDirModel::index(a, 0, modelIndex);

      do
      {
         checkBoxMap.insert( tmpIndex.internalId(), value );

         // check the children recursively
         if ( QAbstractItemModel::hasChildren(tmpIndex) && expandedMap.value(tmpIndex.internalId()) == 1)
            checkChildren ( tmpIndex, value);

         a++;
         tmpIndex = QDirModel::index(a, 0, modelIndex);
      } while ( tmpIndex.isValid() );

      return index(a-1, 0, modelIndex);  //returns the last valid child
   }

   else if (modelIndex.isValid())
      return modelIndex;
   else
   {
      QMessageBox::critical(0,"CheckBoxDirModel", "EXCEPTION in DirModel. Model Index not valid.");
      return modelIndex;
   }
}

////////////////////////////////////////////////////////////////////////////////

// returns the top-most index modified
QModelIndex CheckBoxDirModel::checkParents(const QModelIndex& index, bool value)
{
   QModelIndex currentIndex = index;
   QModelIndex parentIndex = parent(index);
   int childState;

   //to check
   if (value)
   {
      childState = true;

      while( parentIndex.isValid() )
      {
         if( hasSiblings(currentIndex) )
         {
            if ( areSiblingsChecked(currentIndex) )
               checkBoxMap.insert( parentIndex.internalId(), childState );

            else
            {
               checkBoxMap.insert( parentIndex.internalId(), 2 );
               childState = 2;
            }
         }
         else
         {
            checkBoxMap.insert( parentIndex.internalId(), childState );
         }
         //emit dataChanged(parentIndex, parentIndex);

         currentIndex = parentIndex;
         parentIndex = currentIndex.parent();
      }

   }

   //to uncheck
   else
   {
      childState = false;

      while( parentIndex.isValid() )
      {
         if( hasSiblings(currentIndex) )
         {
            if ( areSiblingsUnchecked(currentIndex) )
               checkBoxMap.insert( parentIndex.internalId(), false );

            else
            {
               checkBoxMap.insert( parentIndex.internalId(), 2 );
               childState = 2;
            }
         }
         else
         {
            checkBoxMap.insert( parentIndex.internalId(), childState );
         }
         //emit dataChanged(parentIndex, parentIndex);

         currentIndex = parentIndex;
         parentIndex = currentIndex.parent();
      }
   }

   return currentIndex; //it's the last valid index
}

////////////////////////////////////////////////////////////////////////////////

QVariant CheckBoxDirModel::convertState(int value) const
{
   switch (value)
   {
      case 0:
         return Qt::Unchecked;
      case 1:
         return Qt::Checked;
      default:
         return Qt::PartiallyChecked;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool CheckBoxDirModel::hasSiblings(const QModelIndex& index)
{
   if (index.row() ==0)
   {
      if ( QDirModel::index(index.row()+1, index.column(), index.parent()).isValid())
         return true;
      else
         return false;
   }
   else
   {
      return true;
   }
}

////////////////////////////////////////////////////////////////////////////////

bool CheckBoxDirModel::areSiblingsChecked(const QModelIndex& index)
{
    QModelIndex tmpIndex = QDirModel::index( 0, index.column(),index.parent() );
    int i = 0;

    while ( tmpIndex.isValid() )
    {
       if ( checkBoxMap.contains(tmpIndex.internalId()) )
       {
          if( !checkBoxMap.value(tmpIndex.internalId()) )
             return false;
       }
       else
          return false;

       i++;
       tmpIndex = QDirModel::index( i, index.column(), index.parent() );
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

bool CheckBoxDirModel::areSiblingsUnchecked(const QModelIndex& index)
{
    QModelIndex tmpIndex = QDirModel::index( 0, index.column(),index.parent() );
    int i = 0;

    while ( tmpIndex.isValid() )
    {
       if ( checkBoxMap.contains(tmpIndex.internalId()) && checkBoxMap.value(tmpIndex.internalId()) )
             return false;

       i++;
       tmpIndex = QDirModel::index( i, index.column(), index.parent() );
    }
    return true;
}

////////////////////////////////////////////////////////////////////////////////

//Completes the vector of folders and files recursively
void CheckBoxDirModel::fillFileVectors(QModelIndex root)
{
   QModelIndex tmpIndex = index(0,0,root);
   int i = 0;
   std::string tmp;

   while( tmpIndex.isValid() )
   {
      if ( checkBoxMap.contains(tmpIndex.internalId()) && checkBoxMap.value(tmpIndex.internalId()) )
      {
         if( isDir(tmpIndex) )
            if ( checkBoxMap.value(tmpIndex.internalId()) == 1) //means it's checked
            {
               folderVector.push_back( QDirModel::filePath(tmpIndex).toStdString() );
               numberFoldersChecked++;
            }
            else
               fillFileVectors(tmpIndex);
         else
         {
            //Files can only be totally checked
            fileVector.push_back( "FILE=" + QDirModel::filePath(tmpIndex).toStdString() );
            numberFilesChecked++;
         }
      }

      i++;
      tmpIndex = index(i,0,root);
   }

}

//resets the counting variables: files/folders names and number of files/folders
void CheckBoxDirModel::clearVectors()
{
   fileVector.clear();
   folderVector.clear();
   numberFilesChecked = 0;
   numberFoldersChecked = 0;
}

//Completes the vector of folders and files, applying a filter.
//Returns true when all the files under a given folder have a right extension.
bool CheckBoxDirModel::fillFileVectorsFiltered(QModelIndex root,
                                        std::vector<std::string> extensions)
{
   bool isFolderComplete = true;
   QModelIndex tmpIndex = index(0,0,root);
   int i = 0;
   std::string tmp;

   // Iterates the folder
   while( tmpIndex.isValid() )
   {
      if ( checkBoxMap.contains(tmpIndex.internalId()) && checkBoxMap.value(tmpIndex.internalId()) )
      {
         if( isDir(tmpIndex) )
         {
            bool isDownTreeComplete = fillFileVectorsFiltered ( tmpIndex, extensions);
            if( isDownTreeComplete )
            {
               folderVector.push_back( "FOLDER=" + QDirModel::filePath(tmpIndex).toStdString() );
               numberFoldersChecked++;
            }
            else
               isFolderComplete = false;
         }
         else
         {
            if( isExtensionValid(tmpIndex, extensions) )
            {
               fileVector.push_back( "FILE=" + QDirModel::filePath(tmpIndex).toStdString() );
               numberFilesChecked++;
            }
            else
               isFolderComplete = false;
         }
      }
      else
      {
         isFolderComplete = false;
      }

      i++;
      tmpIndex = index(i,0,root);
   }

   return isFolderComplete;
}

bool CheckBoxDirModel::isExtensionValid(QModelIndex index, std::vector<std::string> extensions)
{
    std::string extension = getExtension( QDirModel::fileName(index).toStdString() );
   for ( unsigned int a = 0; a < extensions.size(); a++)
   {
      if ( extensions[a].compare(extension) == 0 )
         return true;
   }
   return false;
}

std::string CheckBoxDirModel::getExtension(std::string fileName)
{
   int dotPosition = fileName.find_last_of(".");
   return fileName.substr(dotPosition + 1, fileName.size() - (dotPosition + 1) );
}

std::vector <std::string> CheckBoxDirModel::getFileVector()
{
   return fileVector;
}

std::vector <std::string> CheckBoxDirModel::getFolderVector()
{
   return folderVector;
}

int CheckBoxDirModel::getNumberFilesChecked()
{
   return numberFilesChecked;
}

int CheckBoxDirModel::getNumberFoldersChecked()
{
   return numberFoldersChecked;
}

void CheckBoxDirModel::setSelection( std::vector<std::string>& paths )
{
   // For each path
   for( unsigned int i = 0; i < paths.size(); ++i )
   {
      QString path = QString::fromStdString( paths[i] );
      QModelIndex index = QDirModel::index( path );
      setData(index, Qt::Checked, Qt::EditRole);
   }

}


void CheckBoxDirModel::collapsed ( const QModelIndex & index )
{
   expandedMap.insert(index.internalId(), 0);      // collapsed
}

void CheckBoxDirModel::expanded ( const QModelIndex & index )
{
   expandedMap.insert(index.internalId(), 1);      // expanded

   if (checkBoxMap.contains(index.internalId()))
   {
      int val = checkBoxMap.value(index.internalId());
      if (val == 0 || val == 1)
      {
         QModelIndex last = checkChildren(index, val);
         emit dataChanged(index, last);
      }

   }
}

/*

void CheckBoxDirModel::changeSelection( const QModelIndex& index, bool add )
{
   if (index.isValid())
   {
      bool selectAll;
      if (add)
         selectAll = true;
      else
      {
         qint64 indexId = index.internalId();
         if (checkBoxMap.contains(indexId) && checkBoxMap.value(indexId) == 1)
            selectAll = true;
         else
            selectAll = false;
      }

      if(isDir(index))
      {
         folderSelectionChanged(index, add, selectAll);
      }
      else
      {
         fileSelectionChanged(index, add, selectAll);
      }
   }

   emit totalNumFilesUpdated(totalFilesSelected);
   emit sizeUpdated(totalSizeSelected);
}

void CheckBoxDirModel::fileSelectionChanged( const QModelIndex& root, bool add, bool selectAll )
{
   QFileInfo info = QDirModel::fileInfo(root);
   qint64 size = info.size();

   if (add)                            // add
   {
      totalSizeSelected += size;
      totalFilesSelected++;
   }
   else if (selectAll)                 // remove only if it was previously selected
   {
      totalSizeSelected -= size;
      totalFilesSelected--;
   }
}

void CheckBoxDirModel::folderSelectionChanged( const QModelIndex& root, bool add, bool selectAll )
{
   int i=0;

   QModelIndex tmpIndex = index(0,0,root);
   bool selAll = selectAll;

   while (tmpIndex.isValid())
   {
      qint64 indexId = tmpIndex.internalId();

      if (!add)
      {
         if (checkBoxMap.contains(indexId) && checkBoxMap.value(indexId) == 1)
            selAll = true;
         else
            selAll = selectAll;
      }

      if (isDir(tmpIndex))
         folderSelectionChanged( tmpIndex, add, selAll );
      else
         fileSelectionChanged( tmpIndex, add, selAll );

      i++;
      tmpIndex = index(i,0,root);
   }
}
*/

// --------------------------------------------------------------------------
//
// Function
//      Name:    QDir CheckBoxDirModel::dir_from_modelindex(const QModelIndex &index)
//      Purpose: Extrae el directorio a partir de un nodo
//      Created: 6/12/07
//
// ------------------

QDir CheckBoxDirModel::dir_from_modelindex(const QModelIndex &index){
    std::string pathstr;
    QModelIndex tempindex=index.sibling(index.row(),0);
    while(tempindex!=QModelIndex())
    {
        QVariant tmp = data(tempindex,0);
        pathstr = tmp.toString().toStdString()+"/"+pathstr;
        tempindex = tempindex.parent();
    }
    if(pathstr != std::string()){ pathstr = pathstr.substr(1,pathstr.length()-1); }
    return QDir(pathstr.c_str());
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool CheckBoxDirModel::is_checked(const QModelIndex & index) const
//      Purpose: Determina si el nodo esta seleccionado
//      Created: 8/1/08
//
// ------------------

bool CheckBoxDirModel::is_checked(const QModelIndex &index) const{
      return  checkBoxMap.contains(index.internalId()) && 
              checkBoxMap.value(index.internalId());
}
