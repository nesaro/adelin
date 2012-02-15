#include "BBInstance.h"
#include <QtDebug>


//bool BBInstance::operator==(const BBInstance &other) 
//{
//    if (this->pid == other.pid) return true;
//    if (this->uid == other.uid && this->confpath == other.confpath) return true;
//    return false;
//}
//BBInstance::BBInstance(QString configdirectory, QString myusername)
//{
//    qDebug() << "BBInstance::BBInstance: Begin";
//    qDebug() << "BBInstance::BBInstance: Directorio pasado:" << configdirectory;
//    qDebug() << "BBInstance::BBInstance: username pasado:" << myusername;
//    confpath = QFileInfo(configdirectory); //TODO: Chequear si fue posible la inicialización
//    username = myusername;
//}

BBInstance::BBInstance(QString configdirectory, QString myusername, QString mybinarypath)
{
    qDebug() << "BBInstance::BBInstance: Begin";
    qDebug() << "BBInstance::BBInstance: Directorio pasado:" << configdirectory;
    qDebug() << "BBInstance::BBInstance: username pasado:" << myusername;
    qDebug() << "BBInstance::BBInstance: binario pasado:" << mybinarypath;
    confpath = QFileInfo(configdirectory); //TODO: Chequear si fue posible la inicialización
    binarypath = QFileInfo(mybinarypath); //TODO: Chequear si fue posible la inicialización
    username = myusername;
}

