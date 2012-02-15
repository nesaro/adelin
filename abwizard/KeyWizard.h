/***************************************
 *            KeyWizard.h
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
//      Name:    KeyWizard.h
//      Purpose: Asistente de Registro
//      Created: 24/10/07
//
// ------------------

#ifndef KEYWIZARD_H_
#define KEYWIZARD_H_
#include "ABConfiguration.h"
#include <QtGui>
#include "ui_KWIntroPage.h"
#include "ui_KWInsertSignedKeysPage.h"
#include "ui_KWEndPage.h"
#include "FileUtils.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:   KWIntroPage
//      Purpose: Crea la página de introducción del KeyWizard
//      Created: 4/2/08
//
// ------------------

class KWIntroPage:public QWizardPage
{
    Q_OBJECT
    public:
        KWIntroPage(QWidget * parent);
        ~KWIntroPage(){};
        bool isComplete() const { return lastcompletestatus; }
        void initializePage();

    private:
        class KWIntroPageWidget:public QWidget
        {
            public:
                KWIntroPageWidget(QWidget * parent);
                Ui::KWIntroPage ui;
        };
        KWIntroPageWidget * mywidget;
        bool lastcompletestatus;
};


// --------------------------------------------------------------------------
//
// Class
//      Name:   KWInsertSignedKeysPage
//      Purpose: Crea la página para la inserción de claves
//      Created: 9/2/08
//
// ------------------

class KWInsertSignedKeysPage:public QWizardPage
{
    Q_OBJECT
    public:
        KWInsertSignedKeysPage(QWidget * parent);
        ~KWInsertSignedKeysPage(){};
        void initializePage();
        bool isComplete() const { return lastcompletestatus; }
        bool validatePage();

    private slots:
            void get_file_name_key1();
            void get_file_name_key2();
            void myUpdate();

    private:
        class KWInsertSignedKeysPageWidget:public QWidget
        {
            public:
                KWInsertSignedKeysPageWidget(QWidget * parent);
                Ui::KWInsertSignedKeysPage ui;
        };
        void connections();
        void init_lineEdits();
        void save_key_paths();
        KWInsertSignedKeysPageWidget * mywidget;
        bool lastcompletestatus;
};


// --------------------------------------------------------------------------
//
// Class
//      Name:   KWEndPage
//      Purpose: Crea la página de finalización del KeyWizard
//      Created: 9/2/08
//
// ------------------

class KWEndPage:public QWizardPage
{
    Q_OBJECT
    public:
        KWEndPage(QWidget * parent);
        ~KWEndPage(){};

    private:
        class KWEndPageWidget:public QWidget
        {
            public:
                KWEndPageWidget(QWidget * parent);
                Ui::KWEndPage ui;
        };
        void myUpdate();
        KWEndPageWidget * mywidget;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    KeyWizard
//      Purpose: Asistente de Registro
//      Created: 24/10/07
//
// ------------------

class KeyWizard : public QWizard
{
	Q_OBJECT
	public:
	KeyWizard(ABConfiguration * , QWidget * parent = 0, Qt::WindowFlags f = 0);
    ABConfiguration * conf;
	private:
	//Ui::ConfigurationWindow ui;
	//void connections();
	//private slots:
	
};

#endif /*KEYWIZARD_H_*/
