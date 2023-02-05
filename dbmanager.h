#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <qsqldatabase.h>
#include <qstring.h>

class DbManager
{
public:
    static bool abrirConexion(const QString& nombre);
    static bool actualizarQREmpleado(int idEmpleado, QString qr);
    static QSqlQuery nombreCompletoPorQR(QString qr);
    static QSqlQuery capturasPorEmpleadoFecha(int idEmpleado, QString fecha = "");
    static bool updateCapturaHoraSalida(int idRegistro, QString horaSalida = "");
    static QSqlQuery horariosPorEmpleado(int idEmpleado);
    static bool insertarRegistro(int idEmpleado, QString entrada, QString salida = "", QString fecha = "");
    static QString qrPorId(int idEmpleado);
};

#endif // DBMANAGER_H
