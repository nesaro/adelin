/***************************************
 *            BBSInterface.cpp
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
//      Name:    BBSInterface.cpp
//      Purpose: Interfaz con el servidor BBStored
//      Created: 21/11/07
//
// ------------------


#include "Box.h"
#include "BackupClientCryptoKeys.h"
#include "BackupStoreConstants.h"
#include "BackupStoreFile.h"
#include "BoxException.h"
#include "BoxPortsAndFiles.h"

#include "BBSInterface.h"

#include <QtDebug>

#include <iostream>
#include <map>
#include <assert.h>




// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterface::BBSInterface(BosConfiguration * config)  
//      Purpose: Constructor
//      Created: 12/11/07
//
// ------------------

BBSInterface::BBSInterface(ABConfiguration * configuration)
{
    conf = configuration;
    connected=false;
    mutex = new QMutex();
    connected_protect = new QMutex();
    writable_protect = new QMutex();
    writable_connection = false;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterface::~BBSInterface()  
//      Purpose: Destructor
//      Created: 12/11/07
//
// ------------------
BBSInterface::~BBSInterface()
{
    bool tmpconnected;
    {
        QMutexLocker lock(connected_protect); 
        tmpconnected = connected;
    }
    if (tmpconnected) disconnect();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterface::connect
//      Purpose: Realiza la conexión al servidor, llamando a connect2 y gestionando excepciones
//      Created: 12/11/07
//
// ------------------
bool BBSInterface::connect(bool writable){
    qDebug() << "BBSInterface::connect: Iniciando";
    mutex->lock();
    bool tmpconnected ;
    {
        QMutexLocker lock(writable_protect);
        QMutexLocker lock2(connected_protect);  //FIXME... poner un timeout
        if (connected && (writable_connection == writable)) return true; //Ya estamos conectados
    }
    qDebug() << "BBSInterface::connect: Realizando nueva conexión";
    {
        QMutexLocker lock(connected_protect); 
        connected = connect2(writable); 
    }
    qDebug() << "BBSInterface::connect: resultado de la llamada a connect2:" << connected << "\n";
    {
        QMutexLocker lock(connected_protect); 
        if (!connected) mutex->unlock();
        tmpconnected = connected;
    }
    {
        QMutexLocker lock(writable_protect);
        QMutexLocker lock2(connected_protect);  //FIXME... poner un timeout
        writable_connection = connected && writable;
    }
    return tmpconnected;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSInterface::connect2
//      Purpose: Realiza la verdadera conexión al servidor
//      Created: 12/11/07
//
// ------------------

bool BBSInterface::connect2(bool writable){
    std::string storeHost;
    if (conf->bbconf->stringprops.find("StoreHostname") != conf->bbconf->stringprops.end())
    {
        conf->bbconf->stringprops["StoreHostname"]->GetInto(storeHost);
    }

    if (storeHost.length() == 0) {
        qDebug() << "BBSInterface::connect2(): Nombre de servidor incorrecto:" << storeHost.c_str() << "\n";
        return false;
    }

    std::string keysFile;
    conf->bbconf->pathprops["KeysFile"]->GetInto(keysFile);

    if (keysFile.length() == 0) {
        qDebug() << "BBSInterface::connect2(): Fichero de claves incorrecto:" << keysFile.c_str() << "\n";
        return false;
    }

    //TLSContext: Contexto? para conexiones
    TLSContext tlsContext;
    if (!InitTlsContext(tlsContext))//, mErrorMessage))
    {
        qDebug() << "BBSInterface::connect2(): Error inicializando contexto\n";
        return false;
    }

    // Initialise keys
    // Establece la clave para todos los elementos necesarios??
    BackupClientCryptoKeys_Setup(keysFile.c_str());

    // 2. Connect to server
    mpSocket = new SocketStreamTLS();
    int storePort = BOX_PORT_BBSTORED;
    // Abrir el socket
    try
    {
        mpSocket->Open(tlsContext, Socket::TypeINET, storeHost.c_str(), storePort);
    }
    catch (BoxException& e)  
    {
        qDebug() << "BBSInterface::connect2: Excepcion capturada al hacer el mpSocket->Open. Código: " << e.what();
        return false;
    }

    // 3. Make a protocol, and handshake
    // BackupProtocolClient es una clase autogenerada
    mpConnection = new BackupProtocolClient(*mpSocket);
    mpConnection->Handshake();

    // Check the version of the server
    {
        std::auto_ptr<BackupProtocolClientVersion> serverVersion(
            mpConnection->QueryVersion(BACKUP_STORE_SERVER_VERSION));

        if (serverVersion->GetVersion() != BACKUP_STORE_SERVER_VERSION)
        {
            qDebug() << "BBSInterface::connect2: Versión del servidor incorrecta";
        //    mErrorMessage.Printf(wxT(
        //        "Wrong server version: "
        //        "expected %d but found %d"),
        //        BACKUP_STORE_SERVER_VERSION,
        //        serverVersion->GetVersion());
            return false;
        }
    }

    // Login -- if this fails, the Protocol will exception
    int acctNo;
    if (!conf->bbconf->intprops["AccountNumber"]->GetInto(acctNo)) 
    {
        qDebug() << "BBSInterface::connect2: Error en el login con el servidor";
        return false;
    }

    mpConnection->QueryLogin(acctNo, writable ? 0 : BackupProtocolClientLogin::Flags_ReadOnly);

    return true;

}



// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBSInterface::disconnect()  
//      Purpose: Desconecta del servidor
//      Created: 12/11/07
//
// ------------------


void BBSInterface::disconnect()
{
    qDebug() << "BBSInterface::disconnect()";
    if (mpConnection != NULL) {
        try {
            mpConnection->QueryFinished();
        } catch (BoxException &e) {
            // ignore exceptions - may be disconnected from server
        }
        delete mpConnection;
        mpConnection = NULL;
    }
    
    if (mpSocket != NULL) {
        try {
            mpSocket->Close();
        } catch (BoxException &e) {
            // ignore exceptions - may be disconnected from server
        }
        delete mpSocket;
        mpSocket = NULL;
    }
    {
        QMutexLocker lock(connected_protect); 
        connected = false;
    }
    mutex->unlock();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBSInterface::InitTlsContext()  
//      Purpose: Inicializa el contexto TLS (Sacada de boxi)
//      Created: 12/11/07
//
// ------------------


bool BBSInterface::InitTlsContext(TLSContext& target)
{
    std::string certFile;
    conf->bbconf->pathprops["CertificateFile"]->GetInto(certFile);

    if (certFile.length() == 0) {
        qDebug() << "BBSInterface::InitTlsContext(): Error: El fichero de certificados no esta configurado\n";
        return false;
    }

    std::string privKeyFile;
    conf->bbconf->pathprops["PrivateKeyFile"]->GetInto(privKeyFile);

    if (privKeyFile.length() == 0) {
        qDebug() << "BBSInterface::InitTlsContext(): Error: El fichero de clave privada no esta configurado\n";
        return false;
    }

    std::string caFile;
    conf->bbconf->pathprops["TrustedCAsFile"]->GetInto(caFile);

    if (caFile.length() == 0) {
        qDebug() << "BBSInterface::InitTlsContext(): Error: El fichero de TrustedCA no esta configurado\n";
        return false;
    }

    try {   
        target.Initialise(false /* as client */, certFile.c_str(),
            privKeyFile.c_str(), caFile.c_str());
    } catch (BoxException &e) {
        qDebug() << "BBSInterface::InitTlsContext(): Error: ";
        qDebug() <<   "There is something wrong with your Certificate ";
        qDebug() <<   "File, Private Key File, or Trusted CAs File. (" << e.what() << ")\n";
        qDebug() << certFile.c_str() << privKeyFile.c_str() << caFile.c_str();
        return false;
    }
    
    return true;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSStatus BBSInterface::check_status()  
//      Purpose: Comprueba si el servidor está levantado o no
//      Created: 27/11/07
//
// ------------------


BBSInterface::BBSStatus BBSInterface::check_status()
{
    //Comprobar servidor responde ping
    //Comprobar servidor tiene puerto abierto
    //¿Comprobar versión?
    qDebug() << "BBSInterface::check_status() begin";
    {
        QMutexLocker lock(connected_protect); 
        if(connected) return BBSS_AVAILABLE;
    }
    if(!connect()) return BBSS_UNAVAILABLE;
    disconnect();
    return BBSS_AVAILABLE; //Consiguio conectar
}


// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBSInterface::list_directory(int64_t id, BackupStoreDirectory& dir) const  
//      Purpose: Devuelve el contenido del directorio id en el objeto dir. Copiada de Boxi
//      Created: 27/11/07
//
// ------------------


bool BBSInterface::list_directory(int64_t id, int16_t excludeFlags, BackupStoreDirectory & dir) 
{
    if(!connect()) return false;
    try {
        mpConnection->QueryListDirectory(
            id,
            // both files and directories:
            BackupProtocolClientListDirectory::Flags_INCLUDE_EVERYTHING,
            excludeFlags,
            // want attributes:
            true);

        // Retrieve the directory from the stream following
        std::auto_ptr<IOStream> dirstream(mpConnection->ReceiveStream());

        dir.ReadFromStream(*dirstream, mpConnection->GetTimeout());
        disconnect();
        return true;
    }
    catch (BoxException& e)
    {
//        HandleException("Error listing directory on server", e);
        qDebug() << "BBSInterface::list_directory: Error al solicitar el contenido del directorio";
        disconnect();
        return false;
    }

}


// --------------------------------------------------------------------------
//
// Function
//      Name:  BackupProtocolClientAccountUsage * BBSInterface::GetAccountUsage() const  
//      Purpose: Devuelve la quota de uso del espacio del servidor
//      Created: 30/11/07
//
// ------------------


BackupProtocolClientAccountUsage* BBSInterface::GetAccountUsage() {
    if (!connect()) return false;

    try {
        std::auto_ptr<BackupProtocolClientAccountUsage> Usage =
            mpConnection->QueryGetAccountUsage();
        disconnect();
        return new BackupProtocolClientAccountUsage(*Usage); //FIXME.. Memory leak
    } catch (BoxException &e) {
        //HandleException("Error getting account information from server", e);
        qDebug() << "BBSInterface::GetAccountUsage: Error";
        disconnect();
        return NULL;
    }
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  BBSNode BBSInterface::get_bbsnode(int64_t,int64_t) 
//      Purpose: Devuelve el objeto de clase Entry asociado al id en el servidor
//      Created: 13/12/07
//
// ------------------


BBSNode BBSInterface::get_bbsnode(int64_t parent, int64_t id) {
    BackupStoreDirectory::Entry * entry=0;
    BackupStoreDirectory dir;
    if(!(list_directory(parent,BackupProtocolClientListDirectory::Flags_EXCLUDE_NOTHING, dir))) assert(false);
    BackupStoreDirectory::Iterator i(dir);
    while ((entry = i.Next()) != 0)
    {
        if (entry->GetObjectID()==id)
        {
           
            BackupStoreFilenameClear clear(entry->GetName());
            BBSNode tmp(*entry,parent);
            return tmp;
        }
    }
    //FIXME Excepción
    //return BBSNode()
    assert(false);
}

// --------------------------------------------------------------------------
//
// Function
//      Name:  void BBSInterface::restore_file(int64_t,int64_t, std::string) 
//      Purpose: Restaura el fichero del servidor en el sistema
//      Created: 19/12/07
//
// ------------------


bool BBSInterface::restore_file(int64_t parentid, int64_t id, std::string filename)
{
    if (!connect()) return false; 
    try  
    { 
        qDebug() << "BBSInterface::restore_file: llamando a QueryGetFile";
        mpConnection->QueryGetFile(parentid, id); 
     
        // Stream containing encoded file 
        qDebug() << "BBSInterface::restore_file:creando IOStream";
        std::auto_ptr<IOStream> objectStream(mpConnection->ReceiveStream()); 
         
        // Decode it 
        qDebug() << "BBSInterface::restore_file:Llamando a DecodeFile";
        BackupStoreFile::DecodeFile(*objectStream, filename.c_str(),  mpConnection->GetTimeout()); 
        disconnect(); 
        return true; 
    } 
    catch (BoxException& e)  
    { 
        //HandleException("Error retrieving file from server", e);  //TODO
        qDebug() << "BBSInterface::restore_file: Capturada excepcion:" << e.what();
        disconnect(); 
        return false; 
    }   



}

// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBSInterface::delete_file(int64_t, std::string) 
//      Purpose: Borra el fichero del servidor en el sistema
//      Created: 17/3/08
//
// ------------------


bool BBSInterface::delete_file(int64_t parentid, std::string filename)
{
    if (!connect()) return false; 
    qDebug() << "BBSInterface::delete_file:" << "Begin";
    qDebug() << "BBSInterface::delete_file:" << "Filename: " << filename.c_str();
    BackupStoreFilenameClear clear(filename.c_str());
    try
    {
    mpConnection->QueryDeleteFile(parentid, clear); 
    }
    catch (BoxException& e)  
    { 
        qDebug() << "BBSInterface::delete_file: Capturada excepcion:" << e.what();
        disconnect(); 
        return false; 
    }   
    disconnect();
    return true;

}

// --------------------------------------------------------------------------
//
// Function
//      Name:  bool BBSInterface::delete_directory(int64_t) 
//      Purpose: Borra el directorio del servidor
//      Created: 17/3/08
//
// ------------------


bool BBSInterface::delete_directory(int64_t dirid)
{
    if (!connect(true)) return false; 
    try
    {
        mpConnection->QueryDeleteDirectory(dirid);
    }
    catch (BoxException& e)  
    { 
        qDebug() << "BBSInterface::delete_file: Capturada excepcion:" << e.what();
        disconnect(); 
        return false; 
    }   
    disconnect(); 
    return true;

}
