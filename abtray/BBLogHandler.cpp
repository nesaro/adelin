/***************************************
 *            BBLogHandler.cpp
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdserver.
 *   abbsdserver is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdserver is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdserver.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    BBLogHandler.cpp
//      Purpose: Manejador del fichero de logs de BB
//      Created: 4/3/08
//
// ------------------


#include "BBLogHandler.h"
#include <QtDebug>

BBLogHandler::BBLogHandler()
{
    //Asignar QTextStream al fichero de log
    //Mover a la última posición del fichero
}

void BBLogHandler::run()
{
    while (true)
    {
        qDebug() << "BBLogHandler::run:" << tr("Beggining Iteration") ;
        //Ver si atEnd es verdadero o falso(o lineas pendientes >0)
        //Si hay linea:
            //QString linea = stream->readLine()
            //parseline(linea)
        sleep(1);
    }
}

bool BBLogHandler::parseline()
{
    //si hay fecha al principio, sacarla.
    //Pasar por los regexp a ver si es FATAL ERROR WARNING o NOTICE
    //Hacer comparaciones con regexp para el resto del mensaje (Normalmente es un texto fijo con un par de parametros variables)
    return true;
}
