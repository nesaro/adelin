/***************************************
 *            MainWindow.cpp
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
//      Name:    MainWindow.cpp
//      Purpose: Ventana Principal
//      Created: 23/10/07
//
// ------------------

#include "MainWindow.h"
#include "AB.h"
//#include "publiAPI/AppPubliImage.h"
#include "ConfigurationWindow.h"

#include <QtGui>
#include <QString>
#include <QMessageBox>


// --------------------------------------------------------------------------
//
// File
//      Name:    MainWindow::MainWindow
//      Purpose: Constructor Ventana Principal
//      Created: 23/10/07
//
// ------------------

MainWindow::MainWindow(ABConfiguration * conf, BBBInterfaceSocket * bc, BBSInterface * bs)
{
	configwindow = new ConfigurationWindow(conf, bc, this);
	statuswidget = new StatusWidget(bc, conf, this);
	filemanagementwindow = new FileManagementWindow(conf, bc, this);
	filerestorewidget = new FileRestoreWidget(conf, bs, this);
	ui.setupUi(this);
	ui.tabWidget ->removeTab(0);
	ui.tabWidget ->removeTab(0);
	ui.tabWidget->addTab(statuswidget,tr("Status"));
	ui.tabWidget->addTab(filerestorewidget,tr("Restore"));
    non_writable_conf_show=false;
    non_readable_conf_show=false;
    if (conf->bbconf->status()==BBCInterface::CIS_NWRITE)
    {
        non_writable_conf_show=true;
    }
    if (conf->bbconf->status()==BBCInterface::CIS_NOK || conf->bbconf->status()==BBCInterface::CIS_UNKNOWN)
    {
        non_readable_conf_show=true;
    }
    QString version="2.7"; //TODO: Move to an special header file
    QString caption="Adelin Backup ";
    setWindowTitle(caption + QString(AB::ABVERSION) + QString(".") +  version);
    bbsipoolthread = new BBSInterfacePoolThread(bs);
	connections();

    // ToolBar

    configurePaths = new QAction(QIcon(":/images/folder-visiting.png"), tr("&Configure Paths"), this);
    configurePaths->setShortcut(tr("Ctrl+N"));
    configurePaths->setStatusTip(tr("Configure paths"));
    connect(configurePaths, SIGNAL(triggered()), filemanagementwindow, SLOT(show()));
    configureProgram = new QAction(QIcon(":/images/preferences-system.png"), tr("&Configure Program"), this);
    configureProgram->setShortcut(tr("Ctrl+N"));
    configureProgram->setStatusTip(tr("Configure program"));
    connect(configureProgram, SIGNAL(triggered()), configwindow, SLOT(show()));
    aboutProgram = new QAction(QIcon(":/images/help-browser.png"), tr("&About dialog"), this);
    aboutProgram->setShortcut(tr("Ctrl+N"));
    aboutProgram->setStatusTip(tr("About"));
    connect(aboutProgram, SIGNAL(triggered()), this, SLOT(show_about_dialog()));
    mainToolBar = addToolBar(tr("Main"));
    mainToolBar->addAction(configurePaths);
    mainToolBar->addAction(configureProgram);
    mainToolBar->addAction(aboutProgram);

    qDebug() << "MainWindow::MainWindow: Iniciando BBSInterfacePoolThread";
    bbsipoolthread->start();
	
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    void MainWindow::connections()
//      Purpose: Conecta SLOTS con SIGNALS
//      Created: 24/10/07
//
// ------------------

void MainWindow::connections()
{
	QObject::connect(ui.actionQuit, SIGNAL(triggered()), this, SLOT(close()));
	QObject::connect(ui.actionAbout, SIGNAL(triggered()), this, SLOT(show_about_dialog()));
	QObject::connect(ui.actionConfiguraci_n_del_Backup, SIGNAL(triggered()), configwindow, SLOT(show()));
	QObject::connect(ui.actionConfiguraci_n_de_las_rutas, SIGNAL(triggered()), filemanagementwindow, SLOT(show()));
    QObject::connect(bbsipoolthread, SIGNAL(server_usage_changed(int)), statuswidget, SLOT(update_progress_bar(int)));
    QObject::connect(bbsipoolthread, SIGNAL(server_status_changed(BBSInterface::BBSStatus)), statuswidget, SLOT(set_server_status(BBSInterface::BBSStatus)));
    QObject::connect(bbsipoolthread, SIGNAL(server_connected(bool)),filerestorewidget , SLOT(activate(bool)));
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void MainWindow::show_about_dialog()
//      Purpose: 
//      Created: 31/10/07
//
// ------------------

void MainWindow::show_about_dialog()
{
    QString str1,str2;
    str1 = tr("<html><center>Adelin Backup %1. Online Backup Software based on <a href=\"http://www.boxbackup.org\">BoxBackup</a>.</center><center><b>Copyright Adelin Software 2007-2008</b></center><br><center>Code written by <a href=\"mailto:nesaro@gmail.com\">Nestor Arocha Rodriguez</a>.</center>").arg(AB::ABVERSION);
    str2 = tr("<center>It includes source code from <a href =\"http://boxi.sf.net\">Boxi</a> developed by Chris Wilson</center><center>This product includes software developed by Ben Summers.</center></html>");
	QMessageBox::about(this,tr("About Adelin Backup..."),str1+str2);
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    void MainWindow::showEvent(QshowEvent & event)
//      Purpose: Redefinición ShowEvent de la clase Padre
//      Created: 30/1/08
//
// ------------------

void MainWindow::showEvent(QShowEvent * event)
{
    //QWidget::showEvent(event);
    show_warnings();
}

void MainWindow::show_warnings()
{
    if (non_writable_conf_show)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot write boxbackup configuration file.\n"
                    "Unable to modify configuration. Please run adelin backup with the same user than boxbackup"));
        non_writable_conf_show=false;
    }
    if (non_readable_conf_show)
    {
        QMessageBox::warning(this, tr("Warning"), tr("Cannot read boxbackup Configuration\n"
                   " Please check installation options and permissions."));
        non_readable_conf_show=false;
    }
}


/*
void MainWindow::initPubli()
{
    QTimer *timer = new QTimer(this);
    timer->start(30000);
    AppPubliImage * publi =  new AppPubliImage(6,Advert::ASC_SMALL);
    ui.stackedWidget->addWidget(publi);
    ui.stackedWidget->setCurrentWidget(publi);
    connect(timer, SIGNAL(timeout()), publi, SLOT(requestAd()));
    publi =  new AppPubliImage(6,Advert::ASC_SMALL);
    ui.stackedWidget_2->addWidget(publi);
    ui.stackedWidget_2->setCurrentWidget(publi);
    connect(configurePaths, SIGNAL(triggered()), publi, SLOT(requestAd()));
    publi =  new AppPubliImage(6,Advert::ASC_SMALL);
    ui.stackedWidget_3->addWidget(publi);
    ui.stackedWidget_3->setCurrentWidget(publi);
    connect(configureProgram, SIGNAL(triggered()), publi, SLOT(requestAd()));
    publi =  new AppPubliImage(6,Advert::ASC_SMALL);
    ui.stackedWidget_4->addWidget(publi);
    ui.stackedWidget_4->setCurrentWidget(publi);
    connect(aboutProgram, SIGNAL(triggered()), publi, SLOT(requestAd()));
}
*/
