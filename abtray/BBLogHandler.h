/***************************************
 *            BBLogHandler.h
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
//      Name:    BBLogHandler.h
//      Purpose: Manejador del fichero de logs de BB
//      Created: 3/3/08
//
// ------------------


#ifndef BBLOGHANDLER_H_
#define BBLOGHANDLER_H_
#include <QThread>
#include "EventHandler.h"

class BBLogHandler: public QThread
{
    Q_OBJECT
    public:
        BBLogHandler();
        void run();
        bool scanfile();
    signals:
        void new_event(Event);

    private:
        QTextStream * stream;
        bool parseline();
};

#endif /*BBLOGHANDLER_H_*/
