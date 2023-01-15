#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <qsqldatabase.h>
#include <qstring.h>

class DbManager
{
public:
    static bool abrirConexion(const QString& nombre);
};

#endif // DBMANAGER_H
