/***************************************
 *            ProgramInstallWizard.cpp
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
//      Name:    ProgramInstallWizard.cpp
//      Purpose: Asistente de Registro de BoxBackup
//      Created: 8/11/07
//
// ------------------

#include "ProgramInstallWizard.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    PIWIntroPage::PIWIntroPage
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

PIWIntroPage::PIWIntroPage(QWidget * parent):QWizardPage(parent)
{
    QWizardPage *page = new QWizardPage();
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new PIWIntroPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle(tr("Where is BoxBackup?"));
    setSubTitle(tr("Find BoxBackup Wizard"));
    setFinalPage(true);
    lastcompletestatus=false;
    emit completeChanged();
    connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    PIWIntroPage::PIWIntroPageWidget::PIWIntroPageWidget
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

PIWIntroPage::PIWIntroPageWidget::PIWIntroPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }


// --------------------------------------------------------------------------
//
// Function
//      Name:    void PIWIntroPage::connections()
//      Purpose: qt Connections
//      Created: 4/2/08
//
// ------------------
void PIWIntroPage::connections()
{
    QObject::connect(mywidget->ui.pushButton, SIGNAL(clicked()), this, SLOT(get_bbackupd_filename()));
    QObject::connect(mywidget->ui.pushButton_2, SIGNAL(clicked()), this, SLOT(get_bbackupconf_filename()));
    QObject::connect(mywidget->ui.pushButton_3, SIGNAL(clicked()), this, SLOT(get_openssl_filename()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void PIWIntroPage::get_bbackupd_filename()
//      Purpose: SLOT. que hacer cuando se le da al botón de obtener archivo
//      Created: 4/2/08
//
// ------------------
void PIWIntroPage::get_bbackupd_filename()
{
    QFileDialog dialog(this);
    QString filename;
    //dialog.setFileMode(QFileDialog::DirectoryOnly);
    filename = dialog.getOpenFileName(this, tr("Searching bbackupd"), "/etc", "BBackupd (bbackupd, bbackupd.exe)");//TODO: Sacar de la config la ruta por defecto
    //qDebug() << dialog.getExistingDirectory(this, tr("Abrir Directorio"), "/etc");//tr("Image Files (*.png *.jpg *.bmp)"));
    if (!(static_cast<ProgramInstallWizard*>(wizard())->conf->set_bb_binary_filename(filename.toStdString())))
    {
        qDebug() << "PIWIntroPage::get_bbackupd_filename: ABConfiguration no pudo guardar la ruta del binario";
        return;
    }
    myUpdate();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void PIWIntroPage::get_bbackupconf_filename()
//      Purpose: SLOT. que hacer cuando se le da al botón de obtener archivo (configuración)
//      Created: 24/3/08
//
// ------------------
void PIWIntroPage::get_bbackupconf_filename()
{
    QFileDialog dialog(this);
    QString filename;
    //dialog.setFileMode(QFileDialog::DirectoryOnly);
    filename = dialog.getOpenFileName(this, tr("Searching bbackupd.conf"), "/etc", "BBackupd (bbackupd.conf)");//TODO: Sacar de la config la ruta por defecto
    //qDebug() << dialog.getExistingDirectory(this, tr("Abrir Directorio"), "/etc");//tr("Image Files (*.png *.jpg *.bmp)"));
    if (!(static_cast<ProgramInstallWizard*>(wizard())->conf->set_bb_config_filename(filename.toStdString())))
    {
        QMessageBox::warning(this, tr("Warning"), tr("ABConfiguration was unable to write configuration path. Please check file Permissions"));
        return;
    }
    myUpdate();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void PIWIntroPage::get_openssl_filename()
//      Purpose: SLOT. que hacer cuando se le da al botón de obtener archivo (configuración)
//      Created: 24/3/08
//
// ------------------
void PIWIntroPage::get_openssl_filename()
{
    QFileDialog dialog(this);
    QString filename;
    //dialog.setFileMode(QFileDialog::DirectoryOnly);
    filename = dialog.getOpenFileName(this, tr("Searching openssl"), "/usr/bin", "openssl (openssl)");//TODO: Sacar de la config la ruta por defecto
    //qDebug() << dialog.getExistingDirectory(this, tr("Abrir Directorio"), "/etc");//tr("Image Files (*.png *.jpg *.bmp)"));
    if (!(static_cast<ProgramInstallWizard*>(wizard())->conf->set_openssl_filename(filename.toStdString())))
    {
        qDebug() << "PIWIntroPage::get_openssl_filename: ABConfiguration no pudo guardar la ruta del binario";
        return;
    }
    myUpdate();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void PIWIntroPage::myUpdate()
//      Purpose: Actualiza botones y etiquetas
//      Created: 4/2/08
//
// ------------------
void PIWIntroPage::myUpdate()
{
    qDebug() << "PIWIntroPage::myUpdate: Valor obtenido de la ruta de bbackupd de la configuracion:" << static_cast<ProgramInstallWizard*>(wizard())->conf->get_bb_binary_filename().c_str();
    bool binok=false,confok=false,opensslok=false;
    if (static_cast<ProgramInstallWizard*>(wizard())->conf->get_bb_binary_filename()!=std::string())
    {
        mywidget->ui.lineEdit->setText(static_cast<ProgramInstallWizard*>(wizard())->conf->get_bb_binary_filename().c_str());
        binok=true;
    }
    if  (static_cast<ProgramInstallWizard*>(wizard())->conf->get_bb_config_filename()!=std::string())
    {
        mywidget->ui.lineEdit_2->setText(static_cast<ProgramInstallWizard*>(wizard())->conf->get_bb_config_filename().c_str());
        confok=true;
    }
    if (static_cast<ProgramInstallWizard*>(wizard())->conf->get_openssl_filename()!=std::string())
    {
        mywidget->ui.lineEdit_3->setText(static_cast<ProgramInstallWizard*>(wizard())->conf->get_openssl_filename().c_str());
        opensslok=true;
    }
    if (binok && confok && opensslok)
    {
        if (!lastcompletestatus)
        {
            lastcompletestatus=true;
            emit completeChanged();
        }
        mywidget->ui.label_2->setText(tr("Boxbackup installation and configuration has been found successfully"));
    }
    else
    {
        mywidget->ui.label_2->setText(tr("Current path is not ok. Please install and configure boxbackup correctly"));
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    PIWIntroPage::showEvent(QShowEvent * event)
//      Purpose: Acciones a realizar con el refresco de la ventana
//      Created: 6/2/07
//
// ------------------

void PIWIntroPage::showEvent(QShowEvent * event) 
{
    myUpdate();
    QWidget::showEvent(event);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    ProgramInstallWizard::ProgramInstallWizard
//      Purpose: Constructor
//      Created: 8/11/07
//
// ------------------

ProgramInstallWizard::ProgramInstallWizard(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QWizard(parent,f)
{
    addPage(new PIWIntroPage(this));
    setWindowTitle(QString::fromUtf8("Asistente para la configuración de BoxBackup"));
    conf = config;
    connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void ProgramInstallWizard::connections()
//      Purpose: qt Connections
//      Created: 7/2/08
//
// ------------------
void ProgramInstallWizard::connections()
{
    QObject::connect(this, SIGNAL(accepted()), this, SLOT(on_accept()));
}

