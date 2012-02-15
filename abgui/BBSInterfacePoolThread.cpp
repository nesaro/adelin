/***************************************
 *            BBSInterfacePoolThread.cpp
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
//      Name:    BBSInterfacePoolThread.cpp
//      Purpose: Hilo encargado de preguntar periodicamente a BBSI el estado de la conexión
//      Created: 31/1/08
//
// ------------------


#include "BBSInterfacePoolThread.h"

#include <QtDebug>

#include <assert.h>

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterfacePoolThread::BBSInterfacePoolThread() 
//      Purpose: Constructor
//      Created: 31/1/08
//
// ------------------

BBSInterfacePoolThread::BBSInterfacePoolThread(BBSInterface * bbsinterface)
{
    bbsi = bbsinterface;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterfacePoolThread::~BBSInterfacePoolThread()  
//      Purpose: Destructor
//      Created: 31/1/08
//
// ------------------
BBSInterfacePoolThread::~BBSInterfacePoolThread()
{
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBSInterfacePoolThread::run()
//      Purpose: Thread execution function
//      Created: 31/1/08
//
// ------------------
void BBSInterfacePoolThread::run()
{
    qDebug() << "BBSInterfacePoolThread::run: Begin";
    int oldvalue=-1,value;
    bool oldconnected=false,connected;
    BBSInterface::BBSStatus currentstatus=BBSInterface::BBSS_UNKNOWN,prevstatus=BBSInterface::BBSS_UNKNOWN;
    while(true)
    {
        try 
        {
            currentstatus = bbsi->check_status();
        }
        catch(BoxException & e)
        {
            qDebug() << "BBSInterfacePoolThread::run: Captured exception";
            currentstatus = BBSInterface::BBSS_UNKNOWN;
        }
        switch(currentstatus)
        {
            case BBSInterface::BBSS_AVAILABLE: 
                value=get_account_usage();
                connected = true;
                break;
            case BBSInterface::BBSS_UNAVAILABLE:
                value=-1;
                connected = false;
                break;
            case BBSInterface::BBSS_UNKNOWN:
                value=-1;
                connected = false;
                break;
        }
        if (value!=oldvalue){
            emit server_usage_changed(value);
            oldvalue=value;
        }
        if (currentstatus!=prevstatus){
            emit server_status_changed(currentstatus);
            prevstatus = currentstatus;
        }
        if (connected!=oldconnected){
            emit server_connected(connected);
            oldconnected=connected;
        }
        sleep(60);
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterfacePoolThread::get_account_usage()
//      Purpose: Devuelve el porcentaje de uso de la cuenta
//      Created: 31/1/08
//
// ------------------
int BBSInterfacePoolThread::get_account_usage()
{
    std::auto_ptr<BackupProtocolClientAccountUsage> mpUsage;
    mpUsage.reset(bbsi->GetAccountUsage());
    if (!mpUsage.get()) 
    {
        qDebug() << "BBSInterfacePoolThread::get_account_usage: Error creando mpUsage";
        return -1;
    }
    //int64_t size = mpUsage->GetBlockSize();
    float occupation=mpUsage->GetBlocksUsed();
    qDebug() << "BBSInterfacePoolThread::get_account_usage: bloques ocupados:" << occupation;
    float total=mpUsage->GetBlocksHardLimit();
    qDebug() << "BBSInterfacePoolThread::get_account_usage: size:" << total;
    qDebug() << "BBSInterfacePoolThread::get_account_usage: Valor devuelto:" << (int)((occupation/total)*100);
    return (int)((occupation/total)*100); 
}

