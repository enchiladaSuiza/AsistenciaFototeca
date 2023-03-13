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
    static bool updateCapturaHoraEntrada(int idRegistro, QString horaEntrada = "");
    static bool insertarRegistro(int idEmpleado, QDate fecha, QString entrada, QString salida = "");
    static QString conseguirArchivo();
    static QString qrPorId(int idEmpleado);
    static QSqlRecord empleadoPorQR(QString qr);
    static QSqlRecord capturasDeEmpleadoEnFecha(int idEmpleado, QDate fecha);
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
    static bool insertarRegistrosVacios(QDate dia);
    static bool hayRegistros(QDate dia);
    static QSqlQuery historialCompletoEmpleado(int idEmpleado, QDate inicio, QDate fin);
    static QSqlQuery idNombresCompletoEmpleados();
    static QString horarioEntradaDia(int empleado, QDate dia);
    static QSqlQuery historialCompletoTodos(QDate inicio, QDate fin);
private:
    inline static QString sqlEmpleado =
            "SELECT id, nombre || ' ' || apellido_paterno || ' ' ||"
            " apellido_materno AS nombre FROM empleado WHERE qr = ?";
    inline static QString sqlDiferenciasEmpleado =
            "SELECT fecha, e_normal, hora_entrada, ((strftime('%H', hora_entrada) - strftime('%H', e_normal)) * 60) + "
            "(strftime('%M', hora_entrada) - strftime('%M', e_normal)) AS dif_e, s_normal, hora_salida, "
            "((strftime('%H', s_normal) - strftime('%H', hora_salida)) * 60) + (strftime('%M', s_normal) - "
            "strftime('%M', hora_salida)) AS dif_s FROM empleado, (SELECT horario.id, subquery1.id, subquery1.empleado, "
            "fecha, hora_entrada, hora_salida, dia, CASE dia WHEN '0' THEN domingo_e WHEN '1' THEN lunes_e "
            "WHEN '2' THEN martes_e WHEN '3' THEN miercoles_e WHEN '4' THEN jueves_e WHEN '5' THEN viernes_e "
            "WHEN '6' THEN sabado_e END AS e_normal, CASE dia WHEN '0' THEN domingo_s WHEN '1' THEN lunes_s "
            "WHEN '2' THEN martes_s WHEN '3' THEN miercoles_s WHEN '4' THEN jueves_s WHEN '5' THEN viernes_s "
            "WHEN '6' THEN sabado_s END AS s_normal FROM horario, (SELECT id, empleado, fecha, "
            "strftime('%w', fecha) AS dia, hora_entrada, hora_salida FROM  registro) AS subquery1 WHERE "
            "horario.id = subquery1.empleado) AS subquery2 WHERE empleado.id = subquery2.id AND empleado.id = ? AND "
            "fecha BETWEEN ? AND ?";
    inline static QString sqlDiferenciasTodos =
            "SELECT empleado.id AS empleado, fecha, e_normal, hora_entrada, ((strftime('%H', hora_entrada) - "
            "strftime('%H', e_normal)) * 60) + (strftime('%M', hora_entrada) - strftime('%M', e_normal)) AS dif_e, "
            "s_normal, hora_salida, ((strftime('%H', s_normal) - strftime('%H', hora_salida)) * 60) + "
            "(strftime('%M', s_normal) - strftime('%M', hora_salida)) AS dif_s FROM empleado, (SELECT horario.id, "
            "subquery1.id, subquery1.empleado, fecha, hora_entrada, hora_salida, dia, CASE dia WHEN '0' THEN domingo_e "
            "WHEN '1' THEN lunes_e WHEN '2' THEN martes_e WHEN '3' THEN miercoles_e WHEN '4' THEN jueves_e WHEN '5' "
            "THEN viernes_e WHEN '6' THEN sabado_e END AS e_normal, CASE dia WHEN '0' THEN domingo_s WHEN '1' THEN lunes_s "
            "WHEN '2' THEN martes_s WHEN '3' THEN miercoles_s WHEN '4' THEN jueves_s WHEN '5' THEN viernes_s "
            "WHEN '6' THEN sabado_s END AS s_normal FROM horario, (SELECT id, empleado, fecha, "
            "strftime('%w', fecha) AS dia, hora_entrada, hora_salida FROM  registro) AS subquery1 WHERE "
            "horario.id = subquery1.empleado) AS subquery2 WHERE empleado.id = subquery2.id AND "
            "fecha BETWEEN ? AND ? ORDER BY empleado.nombre";
};

#endif // DBMANAGER_H
