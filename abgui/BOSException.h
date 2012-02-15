/***************************************
 *            BOSException.h
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
//      Name:    BOSException.h
//      Purpose: Excepciones 
//      Created: 2/1/08
//
// ------------------

#ifndef BOSEXCEPTION_H_
#define BOSEXCEPTION_H_

#include <exception>

// --------------------------------------------------------------------------
//
// Class
//      Name:    BOSException
//      Purpose: Excepcion base
//      Created: 2/1/08
//
// ------------------

class BOSException: public std::exception
{
    public:
        BOSException();
        ~BOSException() throw ();

};

enum FRWLocationModelExceptionCode
{
    FRWLMEC_UNKNOWNITEM=0,
};

// --------------------------------------------------------------------------
//
// Class
//      Name:    BOSException
//      Purpose: Excepciones relacionadas con FRWLocationModel
//      Created: 2/1/08
//
// ------------------

class FRWLocationModelException: public BOSException
{
};

#endif /*BOSEXCEPTION_H_*/
