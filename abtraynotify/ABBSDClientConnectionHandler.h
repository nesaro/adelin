/***************************************
 *            ABBSDClientConnectionHandler.cpp
 *   Copyright 2008 Adelin ??
 * 
 ***************************************/
/*
 *    This file is part of abbsdclient.
 *   abbsdclient is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   abbsdclient is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with abbsdclient.  If not, see <http://www.gnu.org/licenses/>.
*/

// --------------------------------------------------------------------------
//
// File
//      Name:    ABBSDClientConnectionHandler.cpp
//      Purpose: Programa principal
//      Created: 3/3/08
//
// ------------------

#include <QThread>

// --------------------------------------------------------------------------
//
// Class
//      Name:    ABBSDClientConnectionHandler
//      Purpose: Programa principal
//      Created: 3/3/08
//
// ------------------


class ABBSDClientConnectionHandler: public QThread
{
    Q_OBJECT
    public:
        ABBSDClientConnectionHandler(int mycode);
        void run();
        bool success;
    private: 
        int code;
};
