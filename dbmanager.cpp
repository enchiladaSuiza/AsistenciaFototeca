#include "dbmanager.h"
#include "qdebug.h"
#include <QSqlError>

bool DbManager::abrirConexion(const QString& nombre)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(nombre);
    if (db.open())
    {
        return true;
    }
    qDebug() << "Conexión con la base de datos fallida.";
    qDebug() << db.lastError();
    return false;
}
