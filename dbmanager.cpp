    #include "dbmanager.h"
#include <QDate>
#include <QFileDialog>
#include <QSettings>
#include <QSqlError>
#include <QSqlQuery>
#include <QErrorMessage>
#include <QMessageBox>

bool DbManager::abrirConexion()
{
    QSettings configuracion;
    QString archivoBase = "./fototeca.db";
    if (!configuracion.contains("archivoBase")) configuracion.setValue("archivoBase", archivoBase);
    else archivoBase = configuracion.value("archivoBase").toString();

    if (!QFile::exists(archivoBase))
    {
        archivoBase = conseguirArchivo();
        configuracion.setValue("archivoBase", archivoBase);
    }

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(archivoBase);
    if (db.open()) return true;

    QMessageBox box;
    box.setIcon(QMessageBox::Critical);
    box.setWindowTitle("Sistema de Control de Asistencia de la Fototeca Nacional");
    box.setText("No es posible abrir la base de datos.");
    box.setInformativeText("Puede descargar el archivo de "
                           "https://github.com/enchiladaSuiza/AsistenciaFototeca/blob/master/fototeca.db");
    box.exec();
    return false;
}

QString DbManager::conseguirArchivo()
{
    QFileDialog dialogo;
    return dialogo.getOpenFileName(nullptr, "Indique la ubicación del archivo de base de datos.",
                            "./", "Archivo SQLite (*.db)");
}

bool DbManager::actualizarQREmpleado(int idEmpleado, QString qr)
{
    QSqlQuery query;
    query.prepare("UPDATE empleado SET qr = ? WHERE id = ?");
    query.addBindValue(qr);
    query.addBindValue(idEmpleado);
    return query.exec();
}

QSqlRecord DbManager::empleadoPorQR(QString qr)
{
    QSqlQuery query;
    query.prepare("SELECT id, nombre || ' ' || apellido_paterno || ' ' || apellido_materno AS nombre "
                  "FROM empleado WHERE qr = ?");
    query.addBindValue(qr);
    query.exec();
    query.next();
    return query.record();
}

QSqlRecord DbManager::capturasDeEmpleadoEnFecha(int idEmpleado, QString fecha)
{
    QSqlQuery query;
    if (fecha.isEmpty()) fecha = QDate::currentDate().toString(Qt::ISODate);
    query.prepare("SELECT id, hora_entrada, hora_salida FROM registro WHERE empleado = ? AND fecha = ?");
    query.addBindValue(idEmpleado);
    query.addBindValue(fecha);
    query.exec();
    query.next();
    return query.record();
}

bool DbManager::updateCapturaHoraSalida(int idRegistro, QString horaSalida)
{
    QSqlQuery query;
    if (horaSalida.isEmpty()) horaSalida = QTime::currentTime().toString("hh:mm");
    query.prepare("UPDATE registro SET hora_salida = ? WHERE id = ?");
    query.addBindValue(horaSalida);
    query.addBindValue(idRegistro);
    return query.exec();
}

QSqlRecord DbManager::horariosPorEmpleado(int idEmpleado)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM horario WHERE id = ?");
    query.addBindValue(idEmpleado);
    query.exec();
    query.next();
    return query.record();
}

bool DbManager::insertarRegistro(int idEmpleado, QString entrada, QString salida, QString fecha)
{
    QSqlQuery query;
    if (fecha.isEmpty()) fecha = QDate::currentDate().toString(Qt::ISODate);
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

QSqlQuery DbManager::capturasDeUnDia(QDate dia)
{
    int diaSemana = dia.dayOfWeek();
    QPair<QString, QString> tiempos = diasSemanaColumnas.at(diaSemana);

    QSqlQuery query;
    query.prepare("SELECT empleado.id, nombre || ' ' || apellido_paterno || ' ' || apellido_materno AS nombre, " +
                tiempos.first + ", hora_entrada, "
                "((strftime('%H', hora_entrada) - strftime('%H', " + tiempos.first + ")) * 60) +"
                "(strftime('%M', hora_entrada) - strftime('%M', " + tiempos.first + ")) AS diferencia_entrada, "
                + tiempos.second + ", hora_salida, "
                "((strftime('%H', " + tiempos.second + ") - strftime('%H', hora_salida)) * 60) +"
                "(strftime('%M', " + tiempos.second + ") - strftime('%M', hora_salida)) AS diferencia_salida "
                "FROM registro, empleado, horario "
                "WHERE registro.empleado = empleado.id AND horario.id = empleado.id AND fecha = date(?)");
    query.addBindValue(dia.toString(Qt::ISODate));
    query.exec();
    return query;
}
