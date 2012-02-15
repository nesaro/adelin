/***************************************
 *            FRWFileRestorePanel.cpp
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
//      Name:    FRWFileRestorePanel.cpp
//      Purpose: Widget que muestra las acciones que se pueden realizar con un archivo dado en el FileRestoreWidget
//      Created: 30/11/07
//
// ------------------

#include "Box.h"

#include "FRWFileRestorePanel.h"

#include <QtDebug>
#include <QFileDialog>
#include <QMessageBox>

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWFileRestorePanel::FRWFileRestorePanel(QWidget *, Qt::WindowFlags)
//      Purpose: Constructor
//      Created: 30/11/07
//
// ------------------

FRWFileRestorePanel::FRWFileRestorePanel(ABConfiguration * config, BBSInterface * bbsi,  QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f)
{
    conf = config;
    bbsinterface = bbsi;
    ui.setupUi(this);
    connections();
    ui.pushButton_2->setEnabled(false);
    ui.pushButton->setEnabled(false);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWFileRestorePanel::connections()
//      Purpose: conecta SLOTS con SIGNAL
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::connections()
{
    //Conectar los dos botones
    //pushButton Restaurar
    //pushButton_2 Borrar del servidor
    QObject::connect(ui.pushButton, SIGNAL(clicked()), this, SLOT(restore_element()));
    QObject::connect(ui.pushButton_2, SIGNAL(clicked()), this, SLOT(delete_element()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWFileRestorePanel::restore_element()
//      Purpose: Restablece el elemento
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::restore_element()
{
    //1. Obtener BBSNode del FRWModelIndex
    assert(last_selected_item!=NULL);
    //3. Preguntar al usuario por el nombre de fichero
    QString filename = QFileDialog::getSaveFileName(this, tr("Save File"), "" , "");
    //4. Llamar a las librerias de boxbackup y crear el fichero
    if (filename==QString()) return; //Caso nulo
    qDebug() << "FRWFileRestorePanel::restore_element: Nombre de fichero obtenido:" << filename;
    qDebug() << "FRWFileRestorePanel::restore_element: Nombre de fichero obtenido:" << filename.toStdString().c_str();
    bool result;
    try {
        // Go and restore...
        if(false){ //TODO
        //if (node->isDirectory) {
          //  mRestoreCounter = 0;
          //  result = mpServerConnection->Restore(
          //      boxId, destFileName,
          //      &RestoreProgressCallback,
          //      this, /* user data for callback function */
          //      false /* restore deleted */,
          //      false /* don't undelete after restore! */,
          //      false /* resume? */); 
        } else {

            result= bbsinterface->restore_file(
                last_selected_item->get_parent_id(),
                last_selected_item->get_id(),
                QDir::toNativeSeparators(filename).toStdString());
            //result = Restore_Complete;
        }
    } catch (BoxException &e) { //FIXME: Las excepciones BoxExcepcion solamente deben ser tratadas por el BBSInterface
        //::free(destFileName);
            qDebug() << "Error: Restore failed";

        if (e.GetType()    == ConnectionException::ExceptionType &&
            e.GetSubType() == ConnectionException::TLSReadFailed)
        {
            // protocol object has more details than just TLSReadFailed
            //msg.append(mpServerConnection->ErrorString());

            // connection to server is probably dead, so close it now.
            //bbsinterface->disconnect();
            //node->theServerNode->theTree->CollapseAndReset( node->theServerNode->treeId);
        } else {
            //msg.Append(e.what());
        }
        qDebug() << "FRWFileRestorePanel::restore_element: Capturada Excepcion:" << e.what();
        int ret = QMessageBox::critical(this, tr("Fatal Error"), tr("An error happened"));
    }
        if (result)
            QMessageBox::information(this, tr("Information Message"), tr("File restored successfully"));
        else
        {
               QMessageBox::critical(this, tr("Fatal Error"), tr("An error happened."));
               qDebug() << "Llamada inconclusa a la funcion";
        }

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void FRWFileRestorePanel::delete_element()
//      Purpose: Restablece el elemento
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::delete_element()
{
    //1. Obtener BBSNode del FRWModelIndex
    assert(last_selected_item!=NULL);
    bool result;
    try {
        // Go and restore...
        if(false){ //TODO
        } else {

            result= bbsinterface->delete_file(
                last_selected_item->get_parent_id(),
                last_selected_item->get_name());
            //result = Restore_Complete;
        }
    } catch (BoxException &e) {
        qDebug() << "Error: delete failed";
        qDebug() << "FRWFileRestorePanel::delete_element: Capturada Excepcion:" << e.what();
        int ret = QMessageBox::critical(this, tr("Fatal Error"), tr("An error happened"));
    }
        if (result)
            QMessageBox::information(this, tr("Information Message"), tr("File deleted successfully"));
        else
        {
               QMessageBox::critical(this, tr("Fatal Error"), tr("An error happened."));
               qDebug() << "Llamada inconclusa a la funcion";
        }


}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWFileRestorePanel::show_entry(int64_t id)
//      Purpose: 
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::show_node(BBSNode node)
{
    //Determinar que tipo de elemento es
    //Llamar a la funcion show_tipo()
    QString tmp;
    tmp.setNum(node.get_id());
    ui.label_6->setText(tmp);
    tmp.setNum(node.get_size());
    ui.label_7->setText(tmp);
    time_t mytime;
    mytime = node.get_modtime();
    ui.label_8->setText(ctime(&mytime));
    switch(node.get_type())
    {
        case(NT_FILE):
           ui.label_5->setText(tr("File")); 
           ui.pushButton->setEnabled(true);
           ui.pushButton_2->setEnabled(true);
           break; 
        case(NT_DIR):
           ui.label_5->setText(tr("Folder")); 
           ui.pushButton->setEnabled(false);
           ui.pushButton_2->setEnabled(false);
           break; 
        case(NT_LOCATION):
           ui.label_5->setText(tr("Location")); 
           ui.pushButton->setEnabled(false);
           ui.pushButton_2->setEnabled(false);
           break; 
        case(NT_ROOT):
           ui.label_5->setText(tr("Root"));
           ui.pushButton->setEnabled(false);
           ui.pushButton_2->setEnabled(false);
           break; 
        default:
           ui.label_5->setText(tr("Unknown")); 
           ui.pushButton->setEnabled(false);
           ui.pushButton_2->setEnabled(false);
           break; 
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWFileRestorePanel::show_file(int64_t id)
//      Purpose: 
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::show_file(int64_t id)
{
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWFileRestorePanel::show_dir(int64_t id)
//      Purpose: 
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::show_dir(int64_t id)
{
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    FRWFileRestorePanel::show_location(int64_t id)
//      Purpose: 
//      Created: 13/12/07
//
// ------------------

void FRWFileRestorePanel::show_location(int64_t id)
{
}
