#include "dbmanager.h"
#include "qdebug.h"

bool DbManager::abrirConexion(const QString& nombre)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);
    if (db.open())
    {
        qDebug() << "Conexión con la base de datos exitosa.";
        return true;
    }
    qDebug() << "Conexión con la base de datos fallida.";
    return false;
}
