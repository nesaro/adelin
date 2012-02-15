/***************************************
 *            ABBSDClientListener.cpp
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
//      Name:    ABBSDClientListener.cpp
//      Purpose: Escucha las peticiones
//      Created: 3/3/08
//
// ------------------


#include "ABBSDClientListener.h"
#include <QTcpSocket>

ABBSDClientListener::ABBSDClientListener(EventHandler * myeh){ 
    listen(QHostAddress("127.0.0.1"),4445);
    eh =  myeh;
}

void ABBSDClientListener::incomingConnection(int socketDesctriptor){
	qDebug() << "ABBSDClientListener::incomingConnection: begin";
    SocketHandler * handler = new SocketHandler(socketDesctriptor);
    connect(handler, SIGNAL(finished()),handler, SLOT(deleteLater()));
    connect(handler, SIGNAL(new_event(Event)), eh, SLOT(handle_new_event(Event)));
	qDebug() << "ABBSDClientListener::incomingConnection: about to start handler";
    handler->start();
	qDebug() << "ABBSDClientListener::incomingConnection: handler started";
}

SocketHandler::SocketHandler(int mysocketDesctriptor)
{
    socketDescriptor = mysocketDesctriptor;
}

void SocketHandler::run()
{
    qDebug("Comienza el handler!");
    qint16 code=15;
    QTcpSocket tcpSocket;
    if (!tcpSocket.setSocketDescriptor(socketDescriptor)) {
        //emit error(tcpSocket.error());
        qDebug() << "Error de socket";
        return;
    }

    QDataStream in(&tcpSocket);
    in.setVersion(QDataStream::Qt_4_0);
    if(!tcpSocket.waitForConnected())
    {
        qDebug() << "No conecto!";
        return;
    }
    if (tcpSocket.state() != QAbstractSocket::ConnectedState)
    {
        qDebug() << "Desconectado!";
        return;
    }
    if (!tcpSocket.waitForReadyRead())
    {
        qDebug() << "No hay datos pendientes!";
        return;
    }
    if(tcpSocket.bytesAvailable() != (int)sizeof(quint16))
    {
        qDebug() << "Bytes disponibles:" << tcpSocket.bytesAvailable();
        return;
    }
    in >> code;
    qDebug() << "Resultado:" << code;
    //emit new_value(code);
    Event myevent(Event::codetotype(code));
    emit new_event(myevent);
    tcpSocket.waitForDisconnected(); 
    tcpSocket.disconnectFromHost();
}

