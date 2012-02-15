/***************************************
 *            ConfigurationWizard.h
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
//      Name:    ConfigurationWizard.h
//      Purpose: Asistente de Registro de usuario y configuración
//      Created: 24/10/07
//
// ------------------

#ifndef CONFIGURATIONWIZARD_H_
#define CONFIGURATIONWIZARD_H_
#include <QtGui>
#include "ui_CWIntroPage.h"
#include "ui_CWRequestRegisterPage.h"
#include "ui_CWLoginAdelinPage.h"
#include "ui_CWConfigureBBPage.h"
#include "ui_CWSelectDirsPage.h"
#include "ui_CWCommitConfigPage.h"
#include "ui_CWIsNewUserPage.h"
#include "ui_CWQueryAllKeysPage.h"
#include "ui_CWFinishedRegisteredPage.h"
#include "CheckBoxDirModel.h"
#include "ABConfiguration.h" 

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWIntroPage
//      Purpose: Crea la página de petición de introducción
//      Created: 25/1/08
//
// ------------------

class CWIntroPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWIntroPage(QWidget * parent):QWizardPage(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWIntroPageWidget(static_cast<QWidget*>(this));
        //QCheckBox * cb = new QCheckBox("Tiene CuentaAdelin",this );
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Registre su cuenta");
        setSubTitle(QString::fromUtf8("¿Dispone de cuenta de BoxBackup en Adelin?"));
        //registerField("CuentaAdelin", mywidget->ui.checkBox);
    }
    private:
        class CWIntroPageWidget:public QWidget
        {
            public:
                CWIntroPageWidget(QWidget * parent);
                Ui::CWIntroPage ui;
        };
        CWIntroPageWidget * mywidget;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWCommitConfigPage
//      Purpose: Pagina de confirmación de la configuración
//      Created: 8/2/08
//
// ------------------

class CWCommitConfigPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWCommitConfigPage(QWidget * parent);
        void initializePage();
    private:
        class CWCommitConfigPageWidget:public QWidget
        {
            public:
                CWCommitConfigPageWidget(QWidget * parent);
                Ui::CWCommitConfigPage ui;
        };
        CWCommitConfigPageWidget * mywidget;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWSelectDirsPage
//      Purpose: Crea la página de petición de registro de usuario adelin
//      Created: 5/2/08
//
// ------------------

class CWSelectDirsPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWSelectDirsPage(QWidget * parent);
        bool isComplete() const { return lastcompletestatus; }
        bool validatePage();
    private slots:
        void resizeFilenameColumn(){mywidget->ui.treeView->resizeColumnToContents(0);}
        void model_selection_changed(){
            if (mywidget->model->get_number_folders_checked())
            {
                if (!lastcompletestatus)
                {
                    lastcompletestatus = true;
                    emit completeChanged();
                }
                //TODO: Guardar valores en alguna parte
            }
            else
            {
                if(lastcompletestatus)
                {
                    lastcompletestatus = false;
                    emit completeChanged();
                }
            }
            ;}
    private:
        class CWSelectDirsPageWidget:public QWidget
        {
            public:
                CWSelectDirsPageWidget(QWidget * parent);
                Ui::CWSelectDirsPage ui;
                CheckBoxDirModel * model;
        };
        CWSelectDirsPageWidget * mywidget;
        void connections();
        bool lastcompletestatus;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWRequestRegisterPage
//      Purpose: Crea la página de petición de registro de usuario adelin
//      Created: 5/2/08
//
// ------------------

class CWRequestRegisterPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWRequestRegisterPage(QWidget * parent);

    private:
        class CWRequestRegisterPageWidget:public QWidget
        {
            public:
                CWRequestRegisterPageWidget(QWidget * parent);
                Ui::CWRequestRegisterPage ui;
        };
        CWRequestRegisterPageWidget * mywidget;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWLoginAdelinPage
//      Purpose: Crea la página de petición de registro de usuario adelin
//      Created: 5/2/08
//
// ------------------

class CWLoginAdelinPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWLoginAdelinPage(QWidget * parent);
        bool isComplete() const { return lastcompletestatus; }
        bool validatePage();
    private slots:
        void on_editing_finished();

    private:
        class CWLoginAdelinPageWidget:public QWidget
        {
            public:
                CWLoginAdelinPageWidget(QWidget * parent);
                Ui::CWLoginAdelinPage ui;
        };
        void connections();
        bool check_valid_id() const;
        CWLoginAdelinPageWidget * mywidget;
        bool lastcompletestatus;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWConfigureBBPage
//      Purpose: Crea la página de configuración de BoxBackup
//      Created: 6/2/08
//
// ------------------

class CWConfigureBBPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWConfigureBBPage(QWidget * parent);
        void initializePage();
    private:
        class CWConfigureBBPageWidget:public QWidget
        {
            public:
                CWConfigureBBPageWidget(QWidget * parent);
                Ui::CWConfigureBBPage ui;
        };
        CWConfigureBBPageWidget * mywidget;
        QProcess * myprocess;
        bool processfinished;
        int processexitstatus;

};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWIsNewUserPage
//      Purpose: Pregunta si el usuario tiene ya claves registradas o no
//      Created: 13/8/08
//
// ------------------

class CWIsNewUserPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWIsNewUserPage(QWidget * parent):QWizardPage(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWIsNewUserPageWidget(static_cast<QWidget*>(this));
        //QCheckBox * cb = new QCheckBox("Tiene CuentaAdelin",this );
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Generación de claves");
        setSubTitle("Generación de nuevas claves de usuario");
        //setSubTitle(QString::fromUtf8(""));
        registerField("Registrado", mywidget->ui.checkBox);
    }
    private:
        class CWIsNewUserPageWidget: public QWidget
        {
            public:
                CWIsNewUserPageWidget(QWidget * parent);
                Ui::CWIsNewUserPage ui;
        };
        CWIsNewUserPageWidget * mywidget;
};


// --------------------------------------------------------------------------
//
// Class
//      Name:    CWQueryAllKeysPage
//      Purpose: Pregunta al usuario por todas las claves
//      Created: 13/8/08
//
// ------------------

class CWQueryAllKeysPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWQueryAllKeysPage(QWidget * parent):QWizardPage(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWQueryAllKeysPageWidget(static_cast<QWidget*>(this));
        //QCheckBox * cb = new QCheckBox("Tiene CuentaAdelin",this );
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Introduzca todas las claves");
        lastcompletestatus = false;
        connections();
    }
        bool validatePage();
        bool isComplete() const { return lastcompletestatus; }
    private slots:
        void get_file_name_key1();
        void get_file_name_key2();
        void get_file_name_key3();
        void get_file_name_key4();
        void get_file_name_key5();
    private:
        class CWQueryAllKeysPageWidget: public QWidget
        {
            public:
                CWQueryAllKeysPageWidget(QWidget * parent);
                Ui::CWQueryAllKeysPage ui;
        };
        CWQueryAllKeysPageWidget * mywidget;
        void connections();
        void myUpdate();
        bool lastcompletestatus;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    CWFinishedRegisteredPage
//      Purpose: Se acabo para el usuario registrado
//      Created: 13/8/08
//
// ------------------

class CWFinishedRegisteredPage:public QWizardPage  
{
    Q_OBJECT
    public:
        CWFinishedRegisteredPage(QWidget * parent):QWizardPage(parent)
    {
        QVBoxLayout *layout = new QVBoxLayout;
        mywidget = new CWFinishedRegisteredPageWidget(static_cast<QWidget*>(this));
        //QCheckBox * cb = new QCheckBox("Tiene CuentaAdelin",this );
        layout->addWidget(mywidget);
        setLayout(layout);
        setTitle("Se acabo");
    }
    private:
        class CWFinishedRegisteredPageWidget: public QWidget
        {
            public:
                CWFinishedRegisteredPageWidget(QWidget * parent);
                Ui::CWFinishedRegisteredPage ui;
        };
        CWFinishedRegisteredPageWidget * mywidget;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    ConfigurationWizard
//      Purpose: Asistente de Registro
//      Created: 24/10/07
//
// ------------------

class ConfigurationWizard : public QWizard
{
	Q_OBJECT
	public:
	ConfigurationWizard(ABConfiguration *, QWidget * parent = 0, Qt::WindowFlags f = 0);
    std::map<std::string,std::string> config_parammeters; //Stores settings
    std::list<std::string> config_directories; //Stores Initial backup dirs
    ABConfiguration * conf;
    private:
    enum { PAGE_LoginAdelin, PAGE_Intro, PAGE_IsNewUser, PAGE_QueryAllKeys, PAGE_CommitConfig,PAGE_RequestRegister, PAGE_SelectDirs, PAGE_ConfigureBB, PAGE_Finished_Registered }; //El último debe ser uno final, que si no lo pillará como última página
    int nextId() const;
	//Ui::ConfigurationWindow ui;
	//void connections();
	//private slots:
};

#endif /*CONFIGURATIONWIZARD_H_*/
