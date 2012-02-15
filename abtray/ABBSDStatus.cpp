/***************************************
 *            ABBSDStatus.cpp
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
//      Name:    ABBSDStatus.cpp
//      Purpose: Hace de interfaz con todos los elementos del programa que tienen estado de cara al usuario
//      Created: 27/6/08
//
// ------------------

#include "ABBSDStatus.h"

ABBSDStatus::ABBSDStatus(BBInterfaceStatus * mybbis)
{
    //sessionapiinterface = myauthapi;
    bbis = mybbis;
    //apppubli = myapppubli;
}

//AuthAPI::APISTATUSCODE ABBSDStatus::currentSessionStatus() const
//{
//    return sessionapiinterface->currentStatus();
//
//}
BBInstance::BBInstanceStatus ABBSDStatus::currentInstanceStatus() const
{
    return BBInstance::BBIS_UNKNOWN; //TODO: Sacar de ProcessControl el estado
}


