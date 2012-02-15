/***************************************
 *            KeyWizard.cpp
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
//      Name:    KeyWizard.cpp
//      Purpose: Asistente de Registro
//      Created: 8/11/07
//
// ------------------

#include "KeyWizard.h"
#include "FileUtils.h"

// --------------------------------------------------------------------------
//
// Function
//      Name:    KW::KWIntroPageWidget::KWIntroPageWidget
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

KWIntroPage::KWIntroPageWidget::KWIntroPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

// --------------------------------------------------------------------------
//
// Function
//      Name:    KW::KWInsertSignedKeysPageWidget::KWInsertSignedKeysPageWidget
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

KWInsertSignedKeysPage::KWInsertSignedKeysPageWidget::KWInsertSignedKeysPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

// --------------------------------------------------------------------------
//
// Function
//      Name:    KW::KWEndPageWidget::KWEndPageWidget
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

KWEndPage::KWEndPageWidget::KWEndPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

// --------------------------------------------------------------------------
//
// Function
//      Name:    KWIntroPage::KWIntroPage
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

KWIntroPage::KWIntroPage(QWidget * parent):QWizardPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new KWIntroPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle(tr("Asistente de claves"));
    //setSubTitle(tr("Client Side Keys..."));
    lastcompletestatus=false;
    emit completeChanged();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool KWIntroPage::validatePage
//      Purpose: Acciones a realizar cuando se muestra la pagina
//      Created: 9/2/08
//
// ------------------

void KWIntroPage::initializePage()
{
    KeyWizard * parentwizard = static_cast<KeyWizard*>(wizard());
    if((parentwizard->conf->bbconf->pathprops.find("CertificateFile") != 
                parentwizard->conf->bbconf->pathprops.end()) && 
            (parentwizard->conf->bbconf->pathprops.find("PrivateKeyFile") !=
             parentwizard->conf->bbconf->pathprops.end()) &&
        (parentwizard->conf->bbconf->pathprops["CertificateFile"]->get() != "") &&
        (parentwizard->conf->bbconf->pathprops["PrivateKeyFile"]->get() != "")) 
        //TODO: Verificar que son esas dos claves
    {
        mywidget->ui.label->setText(tr("Las claves de cliente son correctas, vamos a configurar las claves firmadas por el servidor"));
        if (!lastcompletestatus)
        {
            lastcompletestatus = true;
            emit completeChanged();
        }
    }
    else
    {
        mywidget->ui.label->setText(tr("Las claves de cliente no son correcta, utilice el asistente de configuración de Adelin Backup"));
        if (lastcompletestatus)
        {
            lastcompletestatus = false;
            emit completeChanged();
        }
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::connections()
//      Purpose: qt Connections
//      Created: 4/2/08
//
// ------------------
void KWInsertSignedKeysPage::connections()
{
    QObject::connect(mywidget->ui.pushButton, SIGNAL(clicked()), this, SLOT(get_file_name_key1()));
    QObject::connect(mywidget->ui.pushButton_2, SIGNAL(clicked()), this, SLOT(get_file_name_key2()));
    //QObject::connect(mywidget->ui.lineEdit, SIGNAL(textChanged(const QString &)), this, SLOT(myUpdate()));
    //QObject::connect(mywidget->ui.lineEdit_2, SIGNAL(textChanged(const QString &)), this, SLOT(myUpdate()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::get_file_name_key1()
//      Purpose: get first filename
//      Created: 4/2/08
//
// ------------------
void KWInsertSignedKeysPage::get_file_name_key1()
{
    QFileInfo filename = FileUtils::preguntarPorFichero(this, "serverCA.pem", "Key");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_4->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::get_file_name_key2()
//      Purpose: get first filename
//      Created: 4/2/08
//
// ------------------
void KWInsertSignedKeysPage::get_file_name_key2()
{
    QString numerousuario;
    KeyWizard * parentwizard = static_cast<KeyWizard*>(wizard());
    numerousuario.setNum(parentwizard->conf->bbconf->intprops["AccountNumber"]->get(), 16);
    QString key2name=numerousuario+QString("-cert.pem"); 
    QFileInfo filename = FileUtils::preguntarPorFichero(this, key2name, "Clave Firmada");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_5->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    KWInsertSignedKeysPage::KWInsertSignedKeysPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

KWInsertSignedKeysPage::KWInsertSignedKeysPage(QWidget * parent):QWizardPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new KWInsertSignedKeysPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle(tr("Asistente de claves"));
    setSubTitle(tr("Claves de servidor"));
    lastcompletestatus=false;
    emit completeChanged();
    connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::initializePage
//      Purpose: Acciones a realizar cuando se muestra la pagina
//      Created: 9/2/08
//
// ------------------

void KWInsertSignedKeysPage::initializePage()
{
    //init_lineEdits();
    myUpdate();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::myUpdate
//      Purpose: Actualización de contenidos
//      Created: 9/2/08
//
// ------------------

void KWInsertSignedKeysPage::myUpdate()
{
    //Si existe el key 1 y existe el key2 //TODO: Futuras verificaciones
    //Habilitar next
    qDebug() << "KWInsertSignedKeysPage::myUpdate() ui.label_4: " << mywidget->ui.label_4->text();
    if(QFile::exists(mywidget->ui.label_4->text()) && QFile::exists(mywidget->ui.label_5->text()))
    {
        if(!lastcompletestatus)
        {
        lastcompletestatus=true;
        emit completeChanged();
        }
    }
    else
    {
        if(lastcompletestatus)
        {
            lastcompletestatus=false;
            emit completeChanged();
        }
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void KWInsertSignedKeysPage::init_lineEdits
//      Purpose: Inicializa los lineedits con los valores de la configuración
//      Created: 9/2/08
//
// ------------------

//FIXME: Nombre obsoleto
void KWInsertSignedKeysPage::init_lineEdits()
{

    std::string key1;
    if (static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops.find("TrustedCAsFile") !=
            static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops.end())
    {
        key1 = static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops["TrustedCAsFile"]->get();
    }
    std::string key2;
    
    if (static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops.find("CertificateFile") !=
            static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops.end())
    {
    key2 = static_cast<KeyWizard*>(wizard())->conf->bbconf->pathprops["CertificateFile"]->get();
    }
    mywidget->ui.label_4->setText(key1.c_str());
    mywidget->ui.label_5->setText(key2.c_str());
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    KWEndPage::KWEndPage
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

KWEndPage::KWEndPage(QWidget * parent):QWizardPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new KWEndPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle(tr("Keys Wizard"));
    setSubTitle(tr("Finishing"));
    setFinalPage(true);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool KWInsertSignedKeysPage::validatePage
//      Purpose: Verificación en la terminación de la página
//      Created: 10/2/08
//
// ------------------

bool KWInsertSignedKeysPage::validatePage()
{
    //Coger las rutas de los lineEdit
    //Si estan en la ruta de configuración
    //Devolver verdadero
    //Si no, intentar copiar
    //Si se puede, devolver verdadero
    //Si no se puede, mostrar mensaje de aviso y devolver falso
    //TODO: Guardar rutas en la configuración
    KeyWizard * parentwizard = static_cast<KeyWizard*>(wizard());
    QString newkey1=mywidget->ui.label_4->text();
    QString newkey2=mywidget->ui.label_5->text();
    QString numerousuario;
    numerousuario.setNum(parentwizard->conf->bbconf->intprops["AccountNumber"]->get(), 16);
    QFileInfo k1dir(newkey1),k2dir(newkey2);
    QString key1name=QString(parentwizard->conf->bbconf->pathprops["TrustedCAsFile"]->get().c_str());
    QString key2name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario+QString("-cert.pem"); 

    qDebug() << "KWInsertSignedKeysPage::validatePage: " << k1dir.filePath();
    return (FileUtils::copiarFichero(this, newkey1,key1name) &&
            FileUtils::copiarFichero(this, newkey2,key2name));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    KeyWizard::KeyWizard
//      Purpose: Constructor
//      Created: 8/11/07
//
// ------------------

KeyWizard::KeyWizard(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QWizard(parent,f)
{
    conf = config;
    addPage(new KWIntroPage(this));
    addPage(new KWInsertSignedKeysPage(this));
    addPage(new KWEndPage(this));
    setWindowTitle(tr("Keys Wizard"));
}
