/***************************************
 *            main.cpp
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
//      Name:    abbsdclient.cpp
//      Purpose: Programa principal
//      Created: 3/3/08
//
// ------------------

#include "abbsd.h"
#include <string>
#include <QCoreApplication>
#include <QtDebug>
#include "ABBSDClientConnectionHandler.h"

#define MAX_STR_LENGTH 15

// --------------------------------------------------------------------------
//
// Function
//      Name:    fillcodemap
//      Purpose: Rellena el map con los indices de códigos a enviar dada una entrada
//      Created: 3/3/08
//
// ------------------

//bool fillcodemap(std::map<std::string,int> & codemap)
//{
//    codemap["store-full"]=0;
//    codemap["read-error"]=1;
//    return true;
//}

bool sendcode(int code)
{
    char **tmpchar;
    int tmpint=0;
    QCoreApplication * app = new QCoreApplication(tmpint,tmpchar);
    ABBSDClientConnectionHandler * mythread = new ABBSDClientConnectionHandler(code);
    app->connect(mythread,SIGNAL(finished()),app,SLOT(quit()));
    app->connect(mythread,SIGNAL(terminated()),app,SLOT(quit()));
    mythread->start();
    return app->exec();
}

int main(int argc, char * argv[])
{
    char tmpstr[MAX_STR_LENGTH+1];
    //Parsear parámetros de entrada
    if (argc!=2) return -1;
    ////Recortar el tamaño máximo
    strncpy(tmpstr,argv[1],MAX_STR_LENGTH);
    tmpstr[MAX_STR_LENGTH]='\0';
    ////Buscar en el map codetosend
    int pos=-1;
    for (int i=0; i<(sizeof(abbsd::EventNames)/sizeof(std::string));i++)
        if (std::string(tmpstr)==abbsd::EventNames[i]) {pos=i;break;}
    if (pos==-1) return -1;
    //Si el parámetro es correcto
    ////Enviar un socket al servidor
    sendcode(pos);
    return 0;
    //Else
    ////Devolver -1 (+ parámetro de error)
}

