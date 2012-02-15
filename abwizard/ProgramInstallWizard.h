/***************************************
 *            ProgramInstallWizard.h
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
//      Name:    ProgramInstallWizard.h
//      Purpose: Asistente de Registro
//      Created: 8/11/07
//
// ------------------

#ifndef PROGRAMINSTALLWIZARD_H_
#define PROGRAMINSTALLWIZARD_H_
#include <QtGui>
#include "ui_PIWIntroPage.h"
#include "ABConfiguration.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:   PIWIntroPage
//      Purpose: Crea la página de introducción
//      Created: 4/2/08
//
// ------------------

class PIWIntroPage:public QWizardPage
{
    Q_OBJECT
    public:
        PIWIntroPage(QWidget * parent);
        ~PIWIntroPage(){};
        bool isComplete() const { return lastcompletestatus; }
        void showEvent(QShowEvent * event);

    private slots:
        void get_bbackupd_filename();
        void get_bbackupconf_filename();
        void get_openssl_filename();
    private:
        class PIWIntroPageWidget:public QWidget
        {
            public:
                PIWIntroPageWidget(QWidget * parent);
                Ui::PIWIntroPage ui;
        };
        void connections();
        void myUpdate();
        PIWIntroPageWidget * mywidget;
        bool lastcompletestatus;
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    ProgramInstallWizard
//      Purpose: Asistente de Registro
//      Created: 8/11/07
//
// ------------------

class ProgramInstallWizard : public QWizard
{
    Q_OBJECT
    public:
        ProgramInstallWizard(ABConfiguration * config, QWidget * parent = 0, Qt::WindowFlags f = 0);
        ABConfiguration * conf; //Some page needs access to ABConfiguration instance
    private slots:
        void on_accept(){conf->write_config(); conf->load_bb_config();} //TODO: Mostrar mensaje de error si algo sale mal
    private:
        void connections();

};

#endif /*PROGRAMINSTALLWIZARD_H_*/
