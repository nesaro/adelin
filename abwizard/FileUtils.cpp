#include "FileUtils.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QtDebug>

bool FileUtils::crearRutaPrevia(QFileInfo rutafichero)
{
    //Equivale a mkdir -p
    QDir directorio = rutafichero.dir();
    QDir raiz("/");
    return raiz.mkpath(directorio.absolutePath());
}

bool FileUtils::copiarFichero(QWidget * parentdialog, QString origen, QString destino) //Genera excepciones en caso de error
{
    //Falta excepciones y todo eso
    if (QFileInfo(origen)==QFileInfo(destino)) return true;
    if (!QFileInfo(destino).exists())
    {
        if (!QFile::copy(origen,destino))
        {
            QMessageBox::warning(parentdialog, "Warning", QString("Unable to copy file ")+origen) ;
            qDebug() << "FileUtils::copiarFichero: origen" << origen << " , destino:" << destino; 
            return false ;
        }
    }
    else //Existe el fichero de destino
    {
        int rc = QMessageBox::warning(parentdialog, 
                "Warning","File already exists. Overwrite? (I will make a backup)",
                QMessageBox::Ok|QMessageBox::Cancel,
                QMessageBox::Ok);
        if (rc==QMessageBox::Cancel) return false;
        if (!QFile::copy(destino,destino+QString(".bk")))
        {
            QMessageBox::warning(parentdialog, "Warning", "Unable to copy file");
            return false;
        }
        if (!QFile::copy(origen,destino)) {
            QMessageBox::warning(parentdialog, "Warning", "Unable to copy file. (while overwriting previous version)");
            return false;
        }
    }
    return true;
}

QFileInfo FileUtils::preguntarPorFichero(QWidget * parent, QString nombrefichero, QString descripcionfichero)
{
    QString filter = QString(descripcionfichero + QString("(") + nombrefichero + QString(")"));
    QFileDialog dialog(parent);
    QString filename=dialog.getOpenFileName(parent, QString("Buscar Fichero"), "", filter);
    if (filename==QString()) return QFileInfo();
    return QFileInfo(filename);
}

bool FileUtils::preguntarycopiararutadefinitiva(QWidget * parent, QString nombrefichero, QString descripcion, QFileInfo destinofinal)
{
    QFileInfo rutatotal = preguntarPorFichero(parent, nombrefichero, descripcion);
    if (rutatotal==QFileInfo()) return false;
    if (!crearRutaPrevia(rutatotal)) return false;
    return copiarFichero(parent, rutatotal.filePath(),destinofinal.filePath());
}

