#include "dbmanager.h"
#include "qdebug.h"
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlQuery>

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

bool DbManager::actualizarQREmpleado(int idEmpleado, QString qr)
{
    QSqlQuery query;
    query.prepare("UPDATE empleado SET qr = ? WHERE id = ?");
    query.addBindValue(qr);
    query.addBindValue(idEmpleado);
    return query.exec();
}

QSqlQuery DbManager::nombreCompletoPorQR(QString qr)
{
    QSqlQuery query;
    query.prepare("SELECT id, nombre, apellido_paterno, apellido_materno FROM empleado WHERE qr = ?");
    query.addBindValue(qr);
    query.exec();
    query.next();
    return query;
}

QSqlQuery DbManager::capturasPorEmpleadoFecha(int idEmpleado, QString fecha)
{
    QSqlQuery query;
    if (fecha.isEmpty())
    {
        fecha = QDate::currentDate().toString();
    }
    query.prepare("SELECT id, hora_entrada, hora_salida FROM registro WHERE empleado = ? AND fecha = ?");
    query.addBindValue(idEmpleado);
    query.addBindValue(fecha);
    query.exec();
    query.next();
    return query;
}

bool DbManager::updateCapturaHoraSalida(int idRegistro, QString horaSalida)
{
    QSqlQuery query;
    if (horaSalida.isEmpty())
    {
        horaSalida = QTime::currentTime().toString("h:mm");
    }
    query.prepare("UPDATE registro SET hora_salida = ? WHERE id = ?");
    query.addBindValue(horaSalida);
    query.addBindValue(idRegistro);
    return query.exec();
}

QSqlQuery DbManager::horariosPorEmpleado(int idEmpleado)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM horario WHERE id = ?");
    query.addBindValue(idEmpleado);
    query.exec();
    query.next();
    return query;
}

bool DbManager::insertarRegistro(int idEmpleado, QString entrada, QString salida, QString fecha)
{
    QSqlQuery query;
    if (fecha.isEmpty())
    {
        fecha = QDate::currentDate().toString();
    }
    if (salida.isEmpty())
    {
        query.prepare("INSERT INTO registro VALUES (NULL, ?, ?, ?, NULL)");
        query.addBindValue(idEmpleado);
        query.addBindValue(fecha);
        query.addBindValue(entrada);
    }
    else
    {
        query.prepare("INSERT INTO registro VALUES (NULL, ?, ?, ?, ?)");
        query.addBindValue(idEmpleado);
        query.addBindValue(fecha);
        query.addBindValue(entrada);
        query.addBindValue(salida);
    }
    return query.exec();
}

QString DbManager::qrPorId(int idEmpleado)
{
    QSqlQuery query;
    query.prepare("SELECT qr FROM empleado WHERE id = ?");
    query.addBindValue(idEmpleado);
    query.exec();
    query.next();
    return query.value(0).toString();
}
