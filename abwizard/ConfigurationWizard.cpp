/***************************************
 *            ConfigurationWizard.cpp
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
//      Name:    ConfigurationWizard.cpp
//      Purpose: Asistente de Registro
//      Created: 24/10/07
//
// ------------------

#include "ConfigurationWizard.h"
#include "FileUtils.h"
#include <regex.h>



// --------------------------------------------------------------------------
//
// Function
//      Name:    CW*Page::CW*Widget::CW*Widget
//      Purpose: Constructor
//      Created: 4/2/08
//
// ------------------

CWIntroPage::CWIntroPageWidget::CWIntroPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWRequestRegisterPage::CWRequestRegisterPageWidget::CWRequestRegisterPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWLoginAdelinPage::CWLoginAdelinPageWidget::CWLoginAdelinPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWConfigureBBPage::CWConfigureBBPageWidget::CWConfigureBBPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWCommitConfigPage::CWCommitConfigPageWidget::CWCommitConfigPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWQueryAllKeysPage::CWQueryAllKeysPageWidget::CWQueryAllKeysPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWFinishedRegisteredPage::CWFinishedRegisteredPageWidget::CWFinishedRegisteredPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWIsNewUserPage::CWIsNewUserPageWidget::CWIsNewUserPageWidget(QWidget * parent):QWidget(parent)
{ ui.setupUi(this); }

CWSelectDirsPage::CWSelectDirsPageWidget::CWSelectDirsPageWidget(QWidget * parent):QWidget(parent)
{ 
    ui.setupUi(this); 
    model = new CheckBoxDirModel;
    ui.treeView->setModel(model);
    ui.treeView->setColumnWidth(0,150);
    ui.treeView->setColumnWidth(1,50);
    ui.treeView->setColumnWidth(2,50);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    CWRequestRegisterPage::CWRequestRegisterPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

CWRequestRegisterPage::CWRequestRegisterPage(QWidget * parent):QWizardPage(parent)
{
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWRequestRegisterPageWidget(static_cast<QWidget*>(this));
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Registra tu usuario Adelin");
        //setSubTitle("????");
        setFinalPage(true);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    CWLoginAdelinPage::CWLoginAdelinPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

CWLoginAdelinPage::CWLoginAdelinPage(QWidget * parent):QWizardPage(parent)
{
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWLoginAdelinPageWidget(static_cast<QWidget*>(this));
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle(QString::fromUtf8("Introduce tu código Adelin"));
        setSubTitle("Introduce el identificador que te ha suministrado Adelin para el backup");
        lastcompletestatus=false; //TODO: Expresión regular en c++
        emit completeChanged();
        registerField("CuentaAdelin", mywidget->ui.lineEdit);
        connections();
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    bool CWLoginAdelinPage::check_valid_id() const
//      Purpose: Chequea la validez de la entrada aportada por el usuario
//      Created: 7/2/08
//
// ------------------

bool CWLoginAdelinPage::check_valid_id() const
{  
    regex_t preg;
    size_t rm;
    if ((rm = regcomp (&preg,"^(0x)?[a-fA-F0-9]{7}$" , REG_EXTENDED|REG_NOSUB)) != 0)
    {
        qDebug() << "CWLoginAdelinPage::check_valid_id: Error creando Expresión regular";
    }
    if (REG_NOMATCH==(regexec(&preg,mywidget->ui.lineEdit->text().toStdString().c_str(),0,0,0)))
    {
        regfree(&preg);
        qDebug() << "CWLoginAdelinPage::check_valid_id: Expresión regular Incorrecta"<< mywidget->ui.lineEdit->text();
        return false;
    }
    qDebug() << "CWLoginAdelinPage::check_valid_id: Expresión regular correcta";
    regfree(&preg);
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWLoginAdelinPage::connections
//      Purpose: conexiones qt
//      Created: 7/2/08
//
// ------------------

void CWLoginAdelinPage::connections()
{  
    QObject::connect(mywidget->ui.lineEdit, SIGNAL(textChanged(const QString&)), this, SLOT(on_editing_finished())); 
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWLoginAdelinPage::on_editing_finished
//      Purpose: Determina si se debe dejar seguir a la siguiente página
//      Created: 7/2/08
//
// ------------------

void CWLoginAdelinPage::on_editing_finished()
{  
    if (check_valid_id()){
        if (!lastcompletestatus)
        {
            lastcompletestatus=true;
            emit completeChanged();
        }
    }
    else
    {
        if (lastcompletestatus)
        {
            lastcompletestatus=false;
            emit completeChanged();
        }
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool CWLoginAdelinPage::validatePage()
//      Purpose: Función llamada cuando se le da al boton "Siguiente"
//      Created: 8/2/08
//
// ------------------

bool CWLoginAdelinPage::validatePage()
{  
    static_cast<ConfigurationWizard*>(wizard())->config_parammeters["boxbackup_id"] = 
        mywidget->ui.lineEdit->text().toStdString();
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    CWSelectDirsPage::CWSelectDirsPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

CWSelectDirsPage::CWSelectDirsPage(QWidget * parent):QWizardPage(parent)
{
        setFinalPage(false);
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWSelectDirsPageWidget(static_cast<QWidget*>(this));
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Elige los directorios");
        setSubTitle("Directorios en los que deseas realizar backup");
        lastcompletestatus = false; 
        emit completeChanged();
        connections();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWSelectDirsPage::connections()
//      Purpose: Conexiones QT
//      Created: 8/2/08
//
// ------------------

void CWSelectDirsPage::connections()
{ 
    QObject::connect(mywidget->ui.treeView, SIGNAL(expanded(QModelIndex)), this, SLOT(resizeFilenameColumn()));//Connect is not allowed for nested objects
	QObject::connect(mywidget->model, SIGNAL(selectionChanged()), this, SLOT(model_selection_changed()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool CWSelectDirsPage::validatePage()
//      Purpose: Función llamada cuando se le da al boton "Siguiente"
//      Created: 8/2/08
//
// ------------------

bool CWSelectDirsPage::validatePage()
{  
    std::vector<std::string> path_vector;
    static_cast<ConfigurationWizard*>(wizard())->config_directories.clear();
    path_vector = mywidget->model->get_dir_selection();
    for (std::vector<std::string>::const_iterator i = path_vector.begin(); i!= path_vector.end();i++)
    {
        (static_cast<ConfigurationWizard*>(wizard()))->config_directories.push_back(*i);
    }
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    CWConfigureBBPage::CWConfigureBBPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

CWConfigureBBPage::CWConfigureBBPage(QWidget * parent):QWizardPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new CWConfigureBBPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle("Finalizando asistente");
    setSubTitle("Generando claves");
    setFinalPage(true);
    myprocess = new QProcess(this);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWConfigureBBPage::initializePage();
//      Purpose: Función llamada cuando se presenta la página
//      Created: 8/2/08
//
// ------------------

void CWConfigureBBPage::initializePage()
{  
    /*
     * Usage:
     *         bbackupd-config config-dir backup-mode account-num server-hostname working-dir backup-dir [more backup directories]
     *
     */
    //TODO: Cambiar a un modelo no asistido por el script en perl
    //1. Llamar a la funcion conf->bbconf->load_default_values();
    //2. Modificar la configuracion en aquellas cosas que haya introducido el usuario
    //3. Llamar a la funcion conf->generate_keys();
    //4. Llamar a la funcion conf->write_bb_config();
    ConfigurationWizard * parentwizard = static_cast<ConfigurationWizard*>(wizard());
    parentwizard->conf->bbconf->load_default_values();
    QString boxid = parentwizard->config_parammeters["boxbackup_id"].c_str();
    bool ok;
    parentwizard->conf->bbconf->intprops["AccountNumber"]->set(boxid.toInt(&ok,16)); //TODO: check ok
    QString basepath = parentwizard->conf->get_bb_keys_dirname().c_str();
    if (boxid.startsWith("0x"))
        basepath+=boxid.remove(0,2);
    else
        basepath+=boxid;
    parentwizard->conf->bbconf->pathprops["KeysFile"]->set((basepath+QString("-FileEncKeys.raw")).toStdString());
    parentwizard->conf->bbconf->pathprops["CertificateFile"]->set((basepath+QString("-cert.pem")).toStdString());
    parentwizard->conf->bbconf->pathprops["PrivateKeyFile"]->set((basepath+QString("-key.pem")).toStdString());
    parentwizard->conf->bbconf->pathprops["TrustedCAsFile"]->set((QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("serverCA.pem")).toStdString());
    

    int j=0;
    for ( std::list<std::string>::const_iterator i = parentwizard->config_directories.begin();
            i!= parentwizard->config_directories.end();
            i++)
    {
       parentwizard->conf->bbconf->mLocations->append_location((QString("location_")+QString::number(j)).toStdString(), (*i).c_str());
       j++;
    }


    //Crear el directorio de configuración...
    FileUtils::crearRutaPrevia(QString(parentwizard->conf->get_bb_data_dirname().c_str())); //FIXME: Temporalmente puesto aqui, deberia ser verificado como el resto
    QFile file(parentwizard->conf->get_bb_config_dirname().c_str());
    QDir mydir("/");
    if (!file.exists())
            if (!mydir.mkpath(parentwizard->conf->get_bb_config_dirname().c_str()))
                {
                    mywidget->ui.label->setText(QString::fromUtf8("No se pudo crear el directorio"));
                    return;
                }
    if (!file.setPermissions(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser|QFile::ReadGroup|QFile::ExeGroup|QFile::ReadOther|QFile::ExeOther))
    {
        mywidget->ui.label->setText(QString::fromUtf8("No se pudo cambiar los permisos"));
        return;
    }   

    //Creando el fichero de configuración...
    //QFile conffile(AB::system_bb_config_default_location);
    QFile conffile(parentwizard->conf->get_bb_config_filename().c_str());
    conffile.write("",1); //Touch
    conffile.setPermissions(QFile::ReadUser|QFile::WriteUser|QFile::ReadGroup|QFile::ReadOther);


    //Crear el directorio de claves
    mydir.cd(parentwizard->conf->get_bb_config_dirname().c_str()); 
    QFile file2(parentwizard->conf->get_bb_keys_dirname().c_str());
    if (!file2.exists())
        if (!mydir.mkdir("bbackupd")) //FIXME: se asume que esta es la diferencia
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo crear el directorio"));
            return;
        }
    if (!file2.setPermissions(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser))
    {
        mywidget->ui.label->setText(QString::fromUtf8("No se pudo cambiar los permisos"));
        return;
    }   
    //Crear El WorkingDirectory
    QFile file3(parentwizard->conf->get_bb_data_dirname().c_str());
    if (!file3.exists())
        if (!mydir.mkdir("data"))
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo crear el directorio"));
            return;
        }
    if (!file3.setPermissions(QFile::ReadUser|QFile::WriteUser|QFile::ExeUser))
    {
        mywidget->ui.label->setText(QString::fromUtf8("No se pudo cambiar los permisos"));
        return;
    }   

    //Crear la clave privada
    QFile privatekey(basepath+QString("-key.pem"));
    if (!privatekey.exists())
    {
        QStringList arguments;
        arguments << "genrsa" << "-out" << basepath+QString("-key.pem") << "2048" ;
        myprocess->start("openssl",arguments);
        if (!myprocess->waitForFinished() || myprocess->exitCode()!=0)
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo ejecutar el comando de claves con éxito"));
            return;
        }
    }
    //Crear la peticion de certificado
    QFile certreq(basepath+QString("-csr.pem"));
    if (!certreq.exists())
    {
        QStringList arguments;
        arguments << "req" << "-new" << "-key" <<  basepath+QString("-key.pem") << "-sha1" << "-out" << basepath+QString("-csr.pem") ;
        myprocess->start("openssl",arguments);
        if (!myprocess->waitForStarted())
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo ejecutar el comando de claves con éxito"));
            return;
        }
        myprocess->write(".\n");
        myprocess->write(".\n");
        myprocess->write(".\n");
        myprocess->write(".\n");
        myprocess->write(".\n");
        myprocess->write(QString(QString("BACKUP-")+boxid+QString("\n")).toStdString().c_str());
        myprocess->write(".\n");
        myprocess->write(".\n");
        myprocess->write(".\n\n");
        myprocess->closeWriteChannel();
        if (!myprocess->waitForFinished() || myprocess->exitCode()!=0)
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo ejecutar el comando de claves con éxito"));
            return;
        }
    }
    QFile encodingkey(basepath+QString("-FileEncKeys.raw"));
    if (!encodingkey.exists())
    {
        QStringList arguments;
        arguments << "rand" << "-out" << basepath+QString("-FileEncKeys.raw") << "1024" ;
        myprocess->start("openssl",arguments);
        if (!myprocess->waitForFinished() || myprocess->exitCode()!=0)
        {
            mywidget->ui.label->setText(QString::fromUtf8("No se pudo ejecutar el comando de claves con éxito"));
            return;
        }
    }

    QString mytext = QString::fromUtf8("<html>Se ha creado la configuración y claves con éxito. <br> Envie el fichero:<br>") + basepath+ QString("-csr.pem <br> a <a href=\"http://www.openadelin.es\">Adelin</a> y siga las instrucciones que se le indique.</html>");
    mywidget->ui.label->setText(mytext);
    parentwizard->conf->write_bb_config(true);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    CWCommitConfigPage::CWCommitConfigPage
//      Purpose: Constructor
//      Created: 9/2/08
//
// ------------------

CWCommitConfigPage::CWCommitConfigPage(QWidget * parent):QWizardPage(parent)
{
    QVBoxLayout *layout = new QVBoxLayout;
    mywidget = new CWCommitConfigPageWidget(static_cast<QWidget*>(this));
    layout->addWidget(mywidget);
    setLayout(layout);
    setTitle(QString::fromUtf8("Confirme la configuración"));
    //setSubTitle(QString::fromUtf8("??"));
    setCommitPage(true);
}
// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWCommitConfigPage::initializePage();
//      Purpose: Función llamada cuando se presenta la página
//      Created: 8/2/08
//
// ------------------

void CWCommitConfigPage::initializePage()
{  
    QString directorystring;
    ConfigurationWizard * parentwizard = static_cast<ConfigurationWizard*>(wizard());
    mywidget->ui.label_4->setText(parentwizard->config_parammeters["boxbackup_id"].c_str());
    for (std::list<std::string>::const_iterator i = parentwizard->config_directories.begin(); 
            i!= parentwizard->config_directories.end();
            i++)
    {
       directorystring+= (*i).c_str();
       directorystring+= "\n";
    }
    mywidget->ui.label_5->setText(directorystring);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::connections()
//      Purpose: qt Connections
//      Created: 14/8/08
//
// ------------------
void CWQueryAllKeysPage::connections()
{
    QObject::connect(mywidget->ui.pushButton, SIGNAL(clicked()), this, SLOT(get_file_name_key1()));
    QObject::connect(mywidget->ui.pushButton_2, SIGNAL(clicked()), this, SLOT(get_file_name_key2()));
    QObject::connect(mywidget->ui.pushButton_3, SIGNAL(clicked()), this, SLOT(get_file_name_key3()));
    QObject::connect(mywidget->ui.pushButton_4, SIGNAL(clicked()), this, SLOT(get_file_name_key4()));
    QObject::connect(mywidget->ui.pushButton_5, SIGNAL(clicked()), this, SLOT(get_file_name_key5()));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::myUpdate()
//      Purpose: Actualiza el estado del boton
//      Created: 14/8/08
//
// ------------------
void CWQueryAllKeysPage::myUpdate()
{
    if(QFile::exists(mywidget->ui.label_6->text()) 
            && QFile::exists(mywidget->ui.label_7->text())
            && QFile::exists(mywidget->ui.label_8->text())
            && QFile::exists(mywidget->ui.label_9->text())
            && QFile::exists(mywidget->ui.label_10->text()))
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
//      Name:    void CWQueryAllKeysPage::get_file_name_key1()
//      Purpose: Obtener fichero 1
//      Created: 14/8/08
//
// ------------------
void CWQueryAllKeysPage::get_file_name_key1()
{
    QFileInfo filename = FileUtils::preguntarPorFichero(this, "serverCA.pem", "Key");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_6->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::get_file_name_key2()
//      Purpose: Obtener fichero 2
//      Created: 24/8/08
//
// ------------------
void CWQueryAllKeysPage::get_file_name_key2()
{
    QString numerousuario;
    bool ok;
    //numerousuario.setNum(field("CuentaAdelin").toString().toInt(ok, 16), 16);
    numerousuario = field("CuentaAdelin").toString();
    QString key2name=numerousuario+QString("-cert.pem"); 
    QFileInfo filename = FileUtils::preguntarPorFichero(this, key2name, "Clave Firmada");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_7->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::get_file_name_key3()
//      Purpose: Obtener fichero 3
//      Created: 34/8/08
//
// ------------------
void CWQueryAllKeysPage::get_file_name_key3()
{
    QString numerousuario;
    bool ok;
    numerousuario = field("CuentaAdelin").toString();
    QString key2name=numerousuario+QString("-csr.pem"); 
    QFileInfo filename = FileUtils::preguntarPorFichero(this, key2name, "Certificado");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_8->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::get_file_name_key4()
//      Purpose: Obtener fichero 4
//      Created: 44/8/08
//
// ------------------
void CWQueryAllKeysPage::get_file_name_key4()
{
    QString numerousuario;
    bool ok;
    numerousuario = field("CuentaAdelin").toString();
    QString key2name=numerousuario+QString("-key.pem"); 
    QFileInfo filename = FileUtils::preguntarPorFichero(this, key2name, "Certificado");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_9->setText(filename.filePath());
        myUpdate();
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void CWQueryAllKeysPage::get_file_name_key5()
//      Purpose: Obtener fichero 5
//      Created: 54/8/08
//
// ------------------
void CWQueryAllKeysPage::get_file_name_key5()
{
    QString numerousuario;
    bool ok;
    numerousuario = field("CuentaAdelin").toString();
    QString key2name=numerousuario+QString("-FileEncKeys.raw"); 
    QFileInfo filename = FileUtils::preguntarPorFichero(this, key2name, "Certificado");
    if (filename!=QFileInfo())
    {
        mywidget->ui.label_10->setText(filename.filePath());
        myUpdate();
    }
}
//
// --------------------------------------------------------------------------
//
// Function
//      Name:    bool CWQueryAllKeysPage::validatePage()
//      Purpose: Función llamada cuando se le da al boton "Siguiente"
//      Created: 14/8/08
//
// ------------------

bool CWQueryAllKeysPage::validatePage()
{  
    //TODO: Esto para los 5 ficheros
    ConfigurationWizard * parentwizard = static_cast<ConfigurationWizard*>(wizard());
    QString newkey1=mywidget->ui.label_6->text();
    QString newkey2=mywidget->ui.label_7->text();
    QString newkey3=mywidget->ui.label_8->text();
    QString newkey4=mywidget->ui.label_9->text();
    QString newkey5=mywidget->ui.label_10->text();
    QString numerousuario;
    numerousuario = field("CuentaAdelin").toString();
    FileUtils::crearRutaPrevia(QString(parentwizard->conf->get_bb_keys_dirname().c_str()));
    FileUtils::crearRutaPrevia(QString(parentwizard->conf->get_bb_data_dirname().c_str()));
    QString key1name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/serverCA.pem");
    QString key2name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario+QString("-cert.pem"); 
    QString key3name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario+QString("-csr.pem"); 
    QString key4name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario+QString("-key.pem"); 
    QString key5name=QString(parentwizard->conf->get_bb_keys_dirname().c_str())+QString("/")+numerousuario+QString("-FileEncKeys.raw"); 
    parentwizard->conf->write_bb_config(true);
    if (FileUtils::copiarFichero(this, newkey1,key1name) &&
            FileUtils::copiarFichero(this, newkey2,key2name) &&
            FileUtils::copiarFichero(this, newkey3,key3name) &&
            FileUtils::copiarFichero(this, newkey4,key4name) &&
            FileUtils::copiarFichero(this, newkey5,key5name))
    {
        QString boxid = parentwizard->config_parammeters["boxbackup_id"].c_str();
        bool ok;
        parentwizard->conf->bbconf->intprops["AccountNumber"]->set(numerousuario.toInt(&ok,16)); //TODO: check ok
        parentwizard->conf->bbconf->pathprops["TrustedCAsFile"]->set(key1name.toStdString());
        parentwizard->conf->bbconf->pathprops["CertificateFile"]->set(key2name.toStdString());
        parentwizard->conf->bbconf->pathprops["PrivateKeyFile"]->set(key4name.toStdString());
        parentwizard->conf->bbconf->pathprops["KeysFile"]->set(key5name.toStdString());
        parentwizard->conf->write_bb_config(true);
        return true;
    }
    return false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    ConfigurationWizard::ConfigurationWizard
//      Purpose: Constructor
//      Created: 24/10/07
//
// ------------------

ConfigurationWizard::ConfigurationWizard(ABConfiguration * config, QWidget * parent, Qt::WindowFlags f) : QWizard(parent,f)
{
    //    setPage(PAGE_Intro, new CWIntroPage(this));
    setPage(PAGE_LoginAdelin, new CWLoginAdelinPage(this));
    setPage(PAGE_RequestRegister, new CWRequestRegisterPage(this));
    setPage(PAGE_ConfigureBB, new CWConfigureBBPage(this));
    setPage(PAGE_SelectDirs, new CWSelectDirsPage(this));
    setPage(PAGE_CommitConfig, new CWCommitConfigPage(this));
    setPage(PAGE_IsNewUser, new CWIsNewUserPage(this));
    setPage(PAGE_QueryAllKeys, new CWQueryAllKeysPage(this));
    setPage(PAGE_Finished_Registered, new CWFinishedRegisteredPage(this));
    setStartId(PAGE_LoginAdelin);
    setWindowTitle(QString::fromUtf8("Asistente de configuración Adelin"));
    conf = config;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    int ConfigurationWizard::nextId()
//      Purpose: What is the next page?
//      Created: 25/1/08
//
// ------------------

int ConfigurationWizard::nextId() const
{
    switch (currentId()) {
        //case PAGE_Intro:
        //    if (field("CuentaAdelin").toBool()) {
        //        return PAGE_LoginAdelin;
        //    } else {
        //        return PAGE_RequestRegister;
        //    }
        case PAGE_LoginAdelin:
            return PAGE_IsNewUser;
        case PAGE_IsNewUser:
            if (field("Registrado").toBool()) {
                return PAGE_QueryAllKeys;
            } else {
                return PAGE_SelectDirs;
            }
        case PAGE_QueryAllKeys:
            return PAGE_Finished_Registered;
        case PAGE_SelectDirs:
            return PAGE_CommitConfig;
        case PAGE_CommitConfig:
            return PAGE_ConfigureBB;
        case PAGE_ConfigureBB:
            return -1;
        case PAGE_RequestRegister:
            return -1;
        case PAGE_Finished_Registered:
            return -1;
    }
}
