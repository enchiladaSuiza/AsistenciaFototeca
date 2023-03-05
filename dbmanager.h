#ifndef DBMANAGER_H
#define DBMANAGER_H

#include <QDate>
#include <QSqlDatabase>
#include <QSqlRecord>

class DbManager
{
public:
    static bool abrirConexion();
    static bool actualizarQREmpleado(int idEmpleado, QString qr);
    static bool updateCapturaHoraSalida(int idRegistro, QString horaSalida = "");
    static bool insertarRegistro(int idEmpleado, QString entrada, QString salida = "", QString fecha = "");
    static QString conseguirArchivo();
    static QString qrPorId(int idEmpleado);
    static QSqlRecord empleadoPorQR(QString qr);
    static QSqlRecord capturasDeEmpleadoEnFecha(int idEmpleado, QString fecha = "");
    static QSqlRecord horariosPorEmpleado(int idEmpleado);
    static QSqlQuery capturasDeUnDia(QDate dia);

    inline static const QList<QPair<QString, QString>> diasSemanaColumnas = {
        qMakePair("", ""),
        qMakePair("lunes_e", "lunes_s"),
        qMakePair("martes_e", "martes_s"),
        qMakePair("miercoles_e", "miercoles_s"),
        qMakePair("jueves_e", "jueves_s"),
        qMakePair("viernes_e", "viernes_s"),
        qMakePair("sabado_e", "sabado_s"),
        qMakePair("domingo_e", "domingo_s"),
    };
};

#endif // DBMANAGER_H
