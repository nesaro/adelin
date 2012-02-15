/***************************************
 *            SessionAPIInterface.h
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
//      Name:    SessionAPIInterface.h
//      Purpose: Interfaz con la api de sesiones que genera eventos compatibles con el contrlador
//      Created: 4/7/08
//
// ------------------


#ifndef SESSIONAPIINTERFACE_H_
#define SESSIONAPIINTERFACE_H_

#include "Event.h"
//#include "sessionAPI/AuthAPI.h"

// --------------------------------------------------------------------------
//
// Class
//      Name:    SessionAPIInterface
//      Purpose: Interfaz con la api de sesiones que genera eventos compatibles con el contrlador
//      Created: 4/7/08
//
// ------------------

class SessionAPIInterface: public QObject
{
    Q_OBJECT
    public:
        SessionAPIInterface(int appid);
        AuthAPI::APISTATUSCODE currentStatus();
    signals:
        void new_event(Event);
    private slots:
        void OnAPISessionOk();
        void OnAPILoginErr(AuthAPI::ERRCODE);
        void OnAPISessionExpired();
        void OnAPIUserNameNeeded();
    private:
        AuthAPI * authapi;

};

#endif /*SESSIONAPIINTERFACE_H_*/
