/***************************************
 *            StatusWidget.cpp
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
//      Name:    StatusWidget.cpp
//      Purpose: Widget que muestra el estado de ejecución
//      Created: 29/10/07
//
// ------------------

#include "StatusWidget.h"
#include <QtDebug>

// --------------------------------------------------------------------------
//
// Function
//      Name:    StatusWidget::StatusWidget
//      Purpose: Constructor
//      Created: 29/10/07
//
// ------------------

StatusWidget::StatusWidget(BBBInterfaceSocket * bc, ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QWidget(parent,f)
{
		ui.setupUi(this);
        bbbi = bc;
        conf = config;
        myUpdate();
//        QPalette palette;
//        palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
        ui.label_2->setAutoFillBackground(true);
        ui.label_4->setAutoFillBackground(true);
//        ui.label_6->setPalette(palette);
        ui.label_6->setAutoFillBackground(true);
        update_progress_bar(-1); 
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::showEvent(QShowEvent * event)
//      Purpose: Redefinición ShowEvent de la clase Padre
//      Created: 14/11/07
//
// ------------------

void StatusWidget::showEvent(QShowEvent * event)
{
    myUpdate();
    QWidget::showEvent(event);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::myUpdate()
//      Purpose: Actualiza el contenido del Widget
//      Created: 14/11/07
//
// ------------------

void StatusWidget::myUpdate()
{
    //Estado de la configuracion
    QString label_content="";
    QPalette palette;
    label_content += "<html><head></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"> <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">";
    switch(conf->bbconf->status())
    {
        case BBCInterface::CIS_UNKNOWN:
            label_content += tr("Unknown");
            palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
            break;
        case BBCInterface::CIS_OK:
            label_content += tr("Ok");
            palette.setColor(QPalette::Active,QPalette::Window,Qt::green);
            break;
        case BBCInterface::CIS_NWRITE:
            label_content += tr("Read Only");
            palette.setColor(QPalette::Active,QPalette::Window,Qt::yellow);
            break;
        case BBCInterface::CIS_NOK:
            label_content += tr("Error reading configuration");
            palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
            break;
    }
    label_content += "</p></body></html>";
    ui.label_4->setText(label_content); 
    ui.label_4->setPalette(palette);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::slot_bbb_interface_state_change()
//      Purpose: Realiza los cambios necesarios en el widget asociados a un cambio de estado de BBBInterfaceSocket
//      Created: 27/11/07
//
// ------------------

void StatusWidget::slot_bbb_interface_state_change(){
    QPalette palette;
    QString label_content="";
    label_content += "<html><head></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"> <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">";
    switch ( bbbi->interface_status())
    {
        case BBBInterfaceSocket::BBBIS_CONNECTED: label_content+=tr("Connected"); 
                              palette.setColor(QPalette::Active,QPalette::Window,Qt::green);
                              break;
        case BBBInterfaceSocket::BBBIS_CONNECTING: label_content+=tr("Connecting"); 
                             palette.setColor(QPalette::Active,QPalette::Window,Qt::yellow);
                              break;
        case BBBInterfaceSocket::BBBIS_REQ_START: label_content+=tr("Request Start"); break;
        case BBBInterfaceSocket::BBBIS_STARTING: label_content+=tr("Starting"); 
                             palette.setColor(QPalette::Active,QPalette::Window,Qt::yellow);
                             break;
        case BBBInterfaceSocket::BBBIS_STARTWAIT: label_content+=tr("Waiting"); break;
        case BBBInterfaceSocket::BBBIS_REQ_STOP: label_content+=tr("Request Stop"); break;
        case BBBInterfaceSocket::BBBIS_STOPPING: label_content+=tr("Stopping"); break;
        case BBBInterfaceSocket::BBBIS_SHUTDOWN: label_content+=tr("Stopped"); break;
        case BBBInterfaceSocket::BBBIS_RESTARTING: label_content+=tr("Restarting"); break;
        default: label_content+="Unknown"; 
                 palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
                 break;

    }
    label_content+="</span></p>";
    //switch( bbbi->last_error() )
    //{
    //    case BBBInterface::ERR_NONE: break;
    //    case BBBInterface::ERR_DISCONNECTED: label_content+=tr("Disconnected"); break;
    //    case BBBInterface::ERR_ACCESSDENIED: label_content+=tr("Access denied"); break;
    //    case BBBInterface::ERR_NORESPONSE: label_content+=tr("No response"); break;
    //    case BBBInterface::ERR_INTERRUPTED: label_content+=tr("Interrupted"); break;
    //    case BBBInterface::ERR_NOSOCKETCONFIG: label_content+=tr("Unable te read socket location from config"); break;
    //    case BBBInterface::ERR_CMDFAILED: label_content+=tr("Command failed"); break;
    //    case BBBInterface::ERR_BADRESPONSE: label_content+=tr("Bad Response"); break;
    //    default: label_content+=tr("Unknown Error"); 
    //             qDebug() << "ERROR: "<< bbbi->last_error();
    //             break;
    //}
    label_content += "</body></html>";
    ui.label_2->setPalette(palette);
    ui.label_2->setText(label_content);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::notify_bbb_error()
//      Purpose: Realiza los cambios necesarios en el widget asociados a un error generado por BBBInterfaceSocket
//      Created: 27/11/07
//
// ------------------

void StatusWidget::notify_bbb_error(){
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::notify_bbb_client_state_change()
//      Purpose: Realiza los cambios necesarios en el widget asociados  a un cambio de estado de bbackupd. BBBInterfaceSocket lo notifica
//      Created: 27/11/07
//
// ------------------

void StatusWidget::notify_bbb_client_state_change(){

}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::update_progress_bar()
//      Purpose: Pregunta a BBSServer por el espacio en la cuenta y actualiza la barra de progreso
//      Created: 7/12/07
//
// ------------------

void StatusWidget::update_progress_bar(int value){
    qDebug() << "StatusWidget::update_progress_bar: begin. Value:" << value;
    if (value == -1)
    {
        ui.progressBar->setValue(0); 
        ui.progressBar->setEnabled(false);
        return;
    }
    ui.progressBar->setEnabled(true);
    ui.progressBar->setValue(value); 
}

// --------------------------------------------------------------------------
//
// Function
//      Name:   void StatusWidget::set_server_status()
//      Purpose: Establece el valor del label de estado de servidor
//      Created: 31/1/08
//
// ------------------

void StatusWidget::set_server_status(BBSInterface::BBSStatus status){
    QString labeltext;
    QPalette palette;
    labeltext += "<html><head></head><body style=\" font-family:'Sans Serif'; font-size:9pt; font-weight:400; font-style:normal;\"> <p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><span style=\" font-size:14pt;\">";
    switch(status)
    {
        case BBSInterface::BBSS_AVAILABLE:
            labeltext+="Available";
            palette.setColor(QPalette::Active,QPalette::Window,Qt::green);
            break;
        case BBSInterface::BBSS_UNAVAILABLE:
            labeltext+="Unavailable";
            palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
            break;
        case BBSInterface::BBSS_UNKNOWN:
            labeltext+="Unknown";
            palette.setColor(QPalette::Active,QPalette::Window,Qt::red);
            break;
        default:
            qDebug() << "StatusWidget::set_server_status: Unknown state";

    }
    labeltext += "</body></html>";
    ui.label_6->setPalette(palette);
    ui.label_6->setText(labeltext); 
}

