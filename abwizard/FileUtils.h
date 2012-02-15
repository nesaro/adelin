#ifndef FILEUTILS__H
#define FILEUTILS__H

#include <QString>
#include <QWidget>
#include <QFileInfo>

namespace FileUtils{
bool crearRutaPrevia(QFileInfo rutafichero);
bool copiarFichero(QWidget * parent, QString origen, QString destino);
QFileInfo preguntarPorFichero(QWidget * parent, QString nombrefichero, QString descripcionfichero);
bool preguntarycopiararutadefinitiva(QWidget * parent, QString nombrefichero, QString descripcion, QFileInfo destinofinal);
};

#endif // FILEUTILS__H
