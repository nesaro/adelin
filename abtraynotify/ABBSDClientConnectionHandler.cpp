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
#include "ABBSDClientConnectionHandler.h"
#include <QtNetwork>
#include <QThread>
#include <QtDebug>


ABBSDClientConnectionHandler::ABBSDClientConnectionHandler(int mycode) { code=mycode; success = false;}
void ABBSDClientConnectionHandler::run()
{
    QByteArray block;
    QDataStream out(&block, QIODevice::WriteOnly);
    QTcpSocket *clientConnection = new QTcpSocket();
    connect(clientConnection, SIGNAL(disconnected()), clientConnection, SLOT(deleteLater()));
    out.setVersion(QDataStream::Qt_4_0);
    qDebug() << "Codigo enviado:" << (quint16)code;
    out << (quint16)code;
    clientConnection->connectToHost("127.0.0.1",4445); //TODO: Definir correctamente hasta que salga QLocalSocket
    if (!clientConnection->waitForConnected())
    {
        qDebug() << "ERROR Conectando!"; //TODO: Aqui va la Notificacion o whatever
    }
    qDebug() << "Tamaño" << block.size();
    clientConnection->write(block);
    clientConnection->disconnectFromHost();
    if (!clientConnection->waitForDisconnected())
    {
        qDebug() << "No desconecté!";
    }
    success = true;
}
