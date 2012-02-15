/***************************************
 *            ProcessControl.cpp
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
//      Name:    ProcessControl.cpp
//      Purpose: Control de procesos
//      Created: 23/4/08
//
// ------------------

#include "ProcessControl.h"
#include <QProcess>
#include <QTextStream>
#include <QtDebug>
#include <QFile>
#include <QFileInfo>
#include <QRegExp>
#include "AB.h"
#include <sys/types.h>

#ifndef WIN32

#include <pwd.h>

#else

#include <windows.h>
#include <psapi.h>
#include <stdio.h>

//Alternativa1 : http://msdn.microsoft.com/en-us/library/aa390418.aspx

#endif


#ifdef WIN32

std::wstring PrintProcessNameAndID( DWORD processID )
{
    TCHAR szProcessName[MAX_PATH] = TEXT("<unknown>");

    // Get a handle to the process.

    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, processID );

    // Get the process name.

    if (NULL != hProcess )
    {
        HMODULE hMod;
        DWORD cbNeeded;

        if ( EnumProcessModules( hProcess, &hMod, sizeof(hMod), 
             &cbNeeded) )
        {
            GetModuleBaseName( hProcess, hMod, szProcessName, 
                               sizeof(szProcessName)/sizeof(TCHAR) );
        }
    }

    // Print the process name and identifier.
    CloseHandle( hProcess );
    return std::wstring(szProcessName); //FIXME: PELIGRO!
}
#endif

// --------------------------------------------------------------------------
//
// Function
//      Name:    ProcessControl::ProcessControl(ABConfiguration *)
//      Purpose: Constructor para las instancias
//      Created: 25/6/08
//
// ------------------

ProcessControl::ProcessControl(ABConfiguration * myconf)
{
    conf = myconf;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBInstance::BBInstanceStatus ProcessControl::instanceStatus(BBInstance)
//      Purpose: Devuelve el estado de la instancia en el sistema
//      Created: 25/6/08
//
// ------------------
BBInstance::BBInstanceStatus ProcessControl::instanceStatus(BBInstance instance) const
{
    qDebug() << "ProcessControl::instanceStatus: Begin";
    qDebug() << "ProcessControl::instanceStatus: Directorio de configuraicon de lainstanca:" << instance.getConfigPath().absoluteFilePath();
    if (instance.getConfigPath() != QFileInfo(QString(conf->get_bb_config_filename().c_str())))
        return BBInstance::BBIS_BADCONFIG;
    if (!isRunningInstance(instance))
        return BBInstance::BBIS_OFF;
    if (instance.getUserName()!=ProcessControl::currentUserName())
    {
        return BBInstance::BBIS_BADUSER;
    }
    return BBInstance::BBIS_OK;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBInstanceStatus ProcessControl::allInstances()
//      Purpose: Devuelve una lista con todas las instancias de bbackupd que corren en el equipo
//      Created: 25/6/08
//
// ------------------

std::list<BBInstance> ProcessControl::allInstances()
{
    qDebug() << "ProcessControl::allInstances: Begin:";
    std::list<BBInstance> tmp;
    //Listar todos los procesos que se llamen bbackupd
    //Para cada proceso
    //  Generar BBInstance
    //  A帽adir a la lista tmp
    /* http://bytes.com/forum/thread225657.html
     *
     * Here's an example of how to do it in Delphi. The code
     * should help you see the steps you need to take to do
     * the PInvoke calls in C#.
     * http://tinyurl.com/k1ki [Experts-Exchange.com]
     *
     * First, you have to get the Process ID somehow.
     *
     * EnumProcesses in Psapi.dll will return you an array of
     * Process ID's.
     *
     * You can then call OpenProcess from Kernel32.dll and
     * pass in a parameter that tells Win32 you're going to
     * inspect the Process's user token.
     *
     * Then you can call OpenProcessToken() to get the process
     * security token.
     *
     * From there, you can call GetTokenInformation() to
     * get the SID owner of the token.
     *
     * Finally, you can call LookupAccountSid() to get the
     * name of the user with that SID.
     *
     * If you need help getting started, let us know, becase
     * it's too lengthy of a process to post here in one
     * post.
     *
     */
    //Listar todos los procesos que se llamen bbackupd
    std::list<int> listapids = getPidProcessByName("bbackupd");
    //Para cada pid
    //  Generar BBInstance
    for (std::list<int>::const_iterator i = listapids.begin(); i!=listapids.end(); i++)
    {
        qDebug() << "ProcessControl::allInstances: Found bbackupd instance";
        BBInstance myinstance = generateBBInstanceFromPID(*i);
        tmp.push_back(myinstance);
    }
    //  A帽adir a la lista tmp
    return tmp;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    std::list<BBInstance> ProcessControl::allValidInstances()
//      Purpose: Devuelve una lista con todas las instancias de bbackupd que se ejecutan con la configuraci贸n vigente y el usuario correcto(No deberia haber m谩s de una)
//      Created: 25/6/08
//
// ------------------

std::list<BBInstance> ProcessControl::allValidInstances()
{
    qDebug() << "ProcessControl::allValidInstances: Begin:";
    std::list<BBInstance> tmp = allInstances();
    std::list<BBInstance> result;
#ifndef WIN32
    for (std::list<BBInstance>::const_iterator i=tmp.begin(); i!=tmp.end(); i++)
    {
        if ((*i).getConfigPath() == QFileInfo(conf->get_bb_config_filename().c_str())) 
        {
            if ((*i).getUserName() == ProcessControl::currentUserName())
            {
                result.push_back(*i);
            }
        }
        //Comparar configuraci贸n y usuario
    }
#else
    for (std::list<BBInstance>::const_iterator i=tmp.begin(); i!=tmp.end(); i++)
    {
	 if ((*i).getBinaryPath() == QString(AB::system_bb_binary_location_1))
	    if ((*i).getUserName() == ProcessControl::currentUserName())
            {
                result.push_back(*i);
            }
    }
#endif
    //TODO: Lanzar excepci贸n en caso de que la lista sea > 1
    return result;

}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ProcessControl::haltInstance(BBInstance)
//      Purpose: Para (Mata) la instancia pasada
//      Created: 25/6/08
//
// ------------------

bool ProcessControl::haltInstance(BBInstance myinstance)
{
    qDebug() << "ProcessControl::haltInstance: Begin:";
    int mypid = getInstancePID(myinstance);
    if (mypid == 0) return false;
#ifdef WIN32
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, mypid );
    return (0 != TerminateProcess(hProcess,1));
#else
    QString tmppid;
    tmppid.setNum(mypid);
    QString comando = QString("kill ") + tmppid;
    qDebug() << "ProcessControl::haltInstance: comando:" << comando;
    bool resultado = QProcess::startDetached(comando);
    return resultado;
#endif
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ProcessControl::isRunningInstance(BBInstance)
//      Purpose: Determina si la instancia esta en ejecuci贸n
//      Created: 25/6/08
//
// ------------------

bool ProcessControl::isRunningInstance(BBInstance myinstance)
{
    qDebug() << "ProcessControl::isRunningInstance: Begin:";
    return getInstancePID(myinstance) != 0;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    bool ProcessControl::launchInstance(BBInstance)
//      Purpose: Lanza la instancia pasada
//      Created: 25/6/08
//
// ------------------

bool ProcessControl::launchInstance(BBInstance myinstance)
{
    qDebug() << "ProcessControl::launchInstance: Begin:";
    QString comando = QString(AB::system_bb_binary_location_1);
    QStringList argumentos;
    argumentos << myinstance.getConfigPath().absoluteFilePath();
    qDebug() << "ProcessControl::launchInstance: comando:" << comando;
    bool resultado = QProcess::startDetached(comando, argumentos);
    return resultado;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    unsigned int ProcessControl::getInstancePID(BBInstance)
//      Purpose: Obtiene el pid del proceso asociado a la instancia
//      Created: 25/6/08
//
// ------------------
unsigned int ProcessControl::getInstancePID(BBInstance myinstance)
{
	qDebug() << "ProcessControl::getInstancePID: begin:";
#ifndef WIN32
    std::string command = std::string("ps -C bbackupd -o pid,uid,command  --no-heading");
    qDebug() << "ProcessControl::getInstancePID: comando solicitado:" << command.c_str();
    QProcess myprocess;
    myprocess.start(command.c_str());
    if (!myprocess.waitForFinished()) 
    {
        myprocess.close();
        return 0;
    }
    while (!myprocess.atEnd())
    {
        char buf[1024];
        qint64 length = myprocess.readLine(buf,sizeof(buf));
        if (length != -1)
        {
            QString mystring(buf);
            QRegExp regex("^\\s*(\\d*)\\s*(\\d*)\\s*((\\w|/)*)\\s((\\w|/|\\.)*).*$");
            qDebug() << "ProcessControl::getInstancePID: Parsing line:" << mystring;
            if (regex.exactMatch(mystring))
            {
                //Comparar, y si es igual devolver
                QStringList strlist = regex.capturedTexts();
                qDebug() << "ProcessControl::getInstancePID: lista:" << strlist;
                if ((myinstance.getConfigPath().absoluteFilePath() == QFileInfo(strlist.at(5)).absoluteFilePath()) && 
                        (ProcessControl::userNamefromUID(strlist.at(2).toUInt()) == myinstance.getUserName()) ) 
                {
                    qDebug() << "Encontrado!";
                    myprocess.close();
                    return strlist.at(1).toUInt();
                }
                else
                {
                    qDebug() << "linea incorrecta" << strlist.at(5) << myinstance.getConfigPath().absoluteFilePath() << strlist.at(2).toUInt() << myinstance.getUserName();
                }
            } 
            else
            {
                qDebug() << "ProcessControl::getInstancePID: Error en la expresion regular";
            }
        }
        else break;
    }
    qDebug() << "No se encontro el proceso";
    myprocess.close();
    return 0;
#else
    std::list<int> listapids = getPidProcessByName("bbackupd.exe");
    //Para cada pid:
    for (std::list<int>::const_iterator i = listapids.begin();
		    i!= listapids.end();
		    i++)
    {
	    //	Comprobar si el uid es el usuario actual
	    //	Comprobar que el parametro es el fichero de configuraci髇 que nos interesa
	    qDebug() << "ProcessControl::getInstancePID: Iterando sobre un proceso";
	    //if(getConfigPath(*i) == QString(myinstance.getConfigPath().absolutePath()) 
	    if(QFileInfo(QFileInfo(getBinaryPath(*i))) == myinstance.getBinaryPath()
			    && getPIDUserName(*i) == myinstance.getUserName()) 
	    {
		    return *i;
	    }
    }
    return 0;
#endif
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QString ProcessControl::userNamefromUID(unsigned int)
//      Purpose: Obtiene el nombre de usuario asociado a un UID
//      Created: 25/6/08
//
// ------------------
QString ProcessControl::userNamefromUID(unsigned int uid)
{
    //Pasar la string a uid
#ifdef WIN32 
	assert(false);
    return QString();
#else
    struct passwd * mypasswd;
    mypasswd = getpwuid(uid); //TODO Check if delete is required
    if (mypasswd == NULL) return QString();
    return QString(mypasswd->pw_name);
#endif
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QString ProcessControl::currentUserName()
//      Purpose: Obtiene el nombre de usuario actual
//      Created: 25/6/08
//
// ------------------
QString ProcessControl::currentUserName()
{
#ifdef WIN32
  TCHAR  infoBuf[32767];
  DWORD  bufCharCount = 32767;
  if (!GetUserName(infoBuf, &bufCharCount)) return QString();
  std::wstring username(infoBuf);
  std::string temp;
  std::copy(username.begin(), username.end(), std::back_inserter(temp));
  qDebug() << "ProcessControl::currentUserName: Nombre obtenido: " << temp.c_str();
  return QString(temp.c_str());

#else
    return userNamefromUID(getuid());
#endif
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    unsigned int ProcessControl::getUID(int)
//      Purpose: Obtiene el UID del usuario que ejecuta el PID dado
//      Created: 25/6/08
//
// ------------------
unsigned int ProcessControl::getUID(int pid)
{
    qDebug() << "ProcessControl::getUID: Begin:";
#ifndef WIN32
    int processuid=0;
    QString strpid, statusfilename;
    strpid.setNum(pid);
    statusfilename = QString("/proc/") + strpid + QString("/status");
    if (!QFile::exists(statusfilename)) return 0;
    QFile statusfile(statusfilename);
    if (!statusfile.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;
    QTextStream in(&statusfile);
    QString line = in.readLine();
    QRegExp regex("^Uid:\\t(\\d*)\\t(\\d*)\\t(\\d*)\\t(\\d*)$");
    while(!line.isNull()){
        qDebug() << "ProcessControl::getUID: Parsing line:" << line;
        if (regex.exactMatch(line))
        {
            QStringList strlist = regex.capturedTexts();
            qDebug() << "ProcessControl::getUID: UID:" << strlist.at(2);
            processuid = strlist.at(2).toInt();
            break;
        } 
        line = in.readLine();
    }
    return processuid;
#else
	assert(false);
#endif
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    ProcessControl::getPidProcessByName(std::string name)
//      Purpose: Obtiene los pid de los procesos cuyo nombre es name
//      Created: 23/4/08
//
// ------------------

std::list<int> ProcessControl::getPidProcessByName(std::string name)
{
    std::list<int> result;
    qDebug() << "ProcessControl::getPidProcessByName: Begin:";
#ifndef WIN32
    QProcess myprocess;
    std::string command = std::string("ps -C ")+name+std::string(" -o pid --no-heading");
    qDebug() << "ProcessControl::getPidProcessByName: comando solicitado:" << command.c_str();
    myprocess.start(command.c_str());
    if (!myprocess.waitForFinished()) return result;
    while (!myprocess.atEnd())
    {
        char buf[16];
        qint64 length = myprocess.readLine(buf,sizeof(buf));
        if (length != -1)
        {
            QString mystring(buf);
            result.push_back(mystring.toInt());
        }
        else break;
    }
#else
      DWORD aProcesses[1024], cbNeeded, cProcesses;
      if ( !EnumProcesses( aProcesses, sizeof(aProcesses), &cbNeeded ) )
	      return result;
      // Calculate how many process identifiers were returned.

      cProcesses = cbNeeded / sizeof(DWORD);
      for ( int i = 0 ; i < cProcesses; i++)
      {
	      if (aProcesses[i] != 0)
	      {
		      std::wstring nombre = PrintProcessNameAndID( aProcesses[i] );
		      std::string temp;
		      std::copy(nombre.begin(), nombre.end(), std::back_inserter(temp));
		      qDebug() << "ProcessControl::getPidProcessByName: New pid name" << aProcesses[i] <<  temp.c_str();
		      if (QString(temp.c_str()).toLower()==QString(name.c_str()).toLower())
		      {
			      qDebug() << "ProcessControl::getPidProcessByName: found proccess";
			      result.push_back((int)aProcesses[i]);
		      }
	      }
      }
      
      


#endif
    return result;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    ProcessControl::checkProcessUser(int pid, int uid)
//      Purpose: Verifica si el uid del proceso se corresponde con el pasado
//      Created: 23/4/08
//
// ------------------

bool ProcessControl::checkProcessUser(int pid, int uid)
{
    int processuid=getUID(pid);
    return processuid == uid;
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    ProcessControl::checkProcessUser(int pid, std::string user)
//      Purpose: Verifica si el proceso fue lanzado por el usuario 
//      Created: 23/4/08
//
// ------------------

bool ProcessControl::checkProcessUser(int pid, std::string user)
{
#ifdef WIN32
	assert(false);
    return false;
#else
    //Pasar la string a uid
    struct passwd * mypasswd;
    mypasswd = getpwnam(user.c_str()); //TODO Check if delete is required
    if (mypasswd == NULL) return false;
    return checkProcessUser(pid,mypasswd->pw_name);
#endif
}


// --------------------------------------------------------------------------
//
// Function
//      Name:    QString ProcessControl::getConfigPath(int pid)
//      Purpose: Devuelve la ruta de configuraci贸n que se le paso al ejecutable
//      Created: 23/4/08
//
// ------------------

QString ProcessControl::getConfigPath(int pid)
{
    qDebug() << "ProcessControl::getConfigPath: Begin:";
#ifndef WIN32
    char data[256]; 
    //char arg[256];
    QString strpid, cmdlinefilename;
    strpid.setNum(pid);
    cmdlinefilename = QString("/proc/") + strpid + QString("/cmdline");
    if (!QFile::exists(cmdlinefilename)) return QString();
    QFile cmdlinefile(cmdlinefilename);
    if (!cmdlinefile.open(QIODevice::ReadOnly | QIODevice::Text)) return QString();
    qint64 num = cmdlinefile.read(data,sizeof(data));
    if (num <= 0) return QString();
    data[255] = '\0';
    //qDebug() << "ProcessControl::getConfigPath: Binary:" << num;
    QFileInfo fileinfo(data);
    if (fileinfo.baseName()!="bbackupd") return QString();
    const char * pos = (char *)memchr(data,'\0',sizeof(data));
    if (pos==NULL) return QString();
    //strcpy(arg,pos+1);
    //arg[1023] = '\0';
    qDebug() << "ProcessControl::getConfigPath: Arg:" << pos+1;
    const QString mystring((const char*)(pos+1));
    return QString(pos+1);
#else
	
    /*
    //Typedef the hello function
    typedef void (*pfunc)(HANDLE, int, void *, int, int *);

    //A pointer to a function
    pfunc hello;

    WCHAR dllname[10];
    dllname[0]='n';
    dllname[1]='t';
    dllname[2]='d';
    dllname[3]='l';
    dllname[4]='l';
    dllname[5]='.';
    dllname[6]='d';
    dllname[7]='l';
    dllname[8]='l';
    dllname[9]=0;
    
    //GetProcAddress
    hello = (pfunc)GetProcAddress(LoadLibrary(dllname), "ZwQueryInformationProcess");
    
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, pid );
    if (hProcess == NULL) return QString();

    LPTSTR lpExeName[256];
    DWORD dwSize = 256;

    //if(QueryFullProcessImageName(hProcess,dwFlags,(LPTSTR)lpExeName,&dwSize)) //FIXME: Solo es para vista
    //if (0!=GetModuleFileNameEx(hProcess,NULL,(LPTSTR)lpExeName,dwSize)) //Solo pilla la ruta del ejecutable, sin los argumentos
    //if (0!=GetProcessImageFileName(hProcess,(LPTSTR)lpExeName,dwSize)) //No lo encuentra en la libreria

    char wstr[1024];
    int cbRet = 0 ;

    hello( hProcess, 27, &wstr, sizeof(wstr), &cbRet); // 27 = ProcessImageFileName, ver documentacion de la funcion ZwQueryInformationProcess. Solo devuelve la ruta
    if (cbRet > 0)
    { 
	    //strcpy( PathImage, "Error in getting path!\n" );
	    //DbgPrint(Error in getting path -- code: %d", ntStatus);
	    qDebug() << "AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA";
	    for (int i=0 ; i < cbRet; i++)
	    {
		    qDebug() << wstr[i];
	    }
	    std::wstring exename((WCHAR*)wstr);
	    std::string temp;
	    std::copy(exename.begin(), exename.end(), std::back_inserter(temp));
	    qDebug() << "AAAAAAAAAAAAAAA" << temp.c_str();
	    return QString();

    }
    return QString();
    */
    assert(false); //No veo manera sencilla de hacer esto, plan B

#endif
}

QString ProcessControl::getBinaryPath(int pid)
{
	qDebug() << "ProcessControl::getBinaryPath: Begin";
#ifndef WIN32
#else
	
    HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
                                   PROCESS_VM_READ,
                                   FALSE, pid );
    if (hProcess == NULL) return QString();
    LPTSTR lpExeName[256];
    DWORD dwSize = 256;
    if (0!=GetModuleFileNameEx(hProcess,NULL,(LPTSTR)lpExeName,dwSize)) //Solo pilla la ruta del ejecutable, sin los argumentos
    {
	    std::wstring exename((WCHAR*)lpExeName);
	    std::string temp;
	    std::copy(exename.begin(), exename.end(), std::back_inserter(temp));
	    qDebug() << "AAAAAAAAAAAAAAA" << temp.c_str();
	    return QString(temp.c_str());
    }
#endif
    return QString();
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    BBInstance ProcessControl::generateBBInstanceFromPID(int)
//      Purpose: Obtiene todos los parametros y genera una instancia a partir de un PID
//      Created: 25/6/08
//
// ------------------
BBInstance ProcessControl::generateBBInstanceFromPID(int pid)
{
	QString configpath = getConfigPath(pid);
	unsigned int uid = getUID(pid);
	return BBInstance(configpath, userNamefromUID(uid),getBinaryPath(pid));
}

// --------------------------------------------------------------------------
//
// Function
//      Name:    QString ProcessControl::getPIDUsername(unsigned int)
//      Purpose: Obtiene el usuario poseedor de la instancia
//      Created: 25/6/08
//
// ------------------
QString ProcessControl::getPIDUserName(unsigned int pid)
{
#ifndef WIN32
	return QString();
#else
	QString sname;
	HANDLE tok = 0;
	TOKEN_USER *ptu;
	DWORD nlen, dlen;
	WCHAR name[300], dom[300], tubuf[300];
	int iUse;

	HANDLE hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
			PROCESS_VM_READ,
			FALSE, pid );

	//open the processes token
	if (!OpenProcessToken(hProcess,TOKEN_QUERY,&tok)) 
	{
		qDebug() << "No se pudo abrir el processToken";
		return QString();
	}

	//get the SID of the token
	ptu = (TOKEN_USER*)tubuf;
	if (!GetTokenInformation(tok,(TOKEN_INFORMATION_CLASS)1,ptu,300,&nlen)) 
	{
		if (tok) CloseHandle(tok);
		return QString();
	}

	//get the account/domain name of the SID
	dlen = 300;
	nlen = 300;
	if (!LookupAccountSid(0, ptu->User.Sid, name, &nlen, dom, &dlen, (PSID_NAME_USE)&iUse))
	{
		if (tok) CloseHandle(tok);
		return QString();
	}

	//copy info to our static buffer
	std::wstring original;
	std::string temp;
	if (dlen && false) { //Incluir dominio?
		original = dom;
		std::copy(original.begin(), original.end(), std::back_inserter(temp));
		sname += QString(temp.c_str());
		sname += QString("");
		original = name;
		std::copy(original.begin(), original.end(), std::back_inserter(temp));
		sname += QString(temp.c_str());
	} else {
		original = name;
		std::copy(original.begin(), original.end(), std::back_inserter(temp));
		sname += QString(temp.c_str());
	}
	//set our return variable
	qDebug() << "ProcessControl::getPIDUsername: username for pid" << pid << " : " << sname;
	return sname;

#endif

}
