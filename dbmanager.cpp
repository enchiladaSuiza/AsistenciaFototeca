#include "dbmanager.h"
#include <QDate>
#include <QFileDialog>
#include <QSettings>
#include <QSqlError>
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

QSqlQuery DbManager::idNombresCompletosEmpleados()
{
    QSqlQuery query;
    query.prepare("SELECT id, nombre || ' ' || apellido_paterno || ' ' || "
                  "apellido_materno AS nombre_completo FROM empleado ORDER BY nombre_completo");
    query.exec();
    return query;
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

bool DbManager::actualizarImagenEmpleado(int idEmpleado, QString rutaImagen)
{
    QSqlQuery query;
    query.prepare("UPDATE empleado SET imagen = ? WHERE id = ?");
    query.addBindValue(rutaImagen);
    query.addBindValue(idEmpleado);
    return query.exec();
}

QString DbManager::conseguirImagenEmpleado(int idEmpleado)
{
    QSqlQuery query;
    query.prepare("SELECT imagen FROM empleado WHERE id = ?");
    query.addBindValue(idEmpleado);
    query.exec();
    query.next();
    return query.value("imagen").toString();
}

QSqlRecord DbManager::empleadoPorQR(QString qr)
{
    QSqlQuery query;
    query.prepare(sqlEmpleado);
    query.addBindValue(qr);
    query.exec();
    query.next();
    return query.record();
}

QSqlRecord DbManager::capturasDeEmpleadoEnFecha(int idEmpleado, QDate fecha)
{
    QSqlQuery query;
    query.prepare("SELECT id, hora_entrada, hora_salida FROM registro WHERE empleado = ? AND fecha = ?");
    query.addBindValue(idEmpleado);
    query.addBindValue(fecha.toString(Qt::ISODate));
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

bool DbManager::updateCapturaHoraEntrada(int idRegistro, QString horaEntrada)
{
    QSqlQuery query;
    if (horaEntrada.isEmpty()) horaEntrada = QTime::currentTime().toString("hh:mm");
    query.prepare("UPDATE registro SET hora_entrada = ? WHERE id = ?");
    query.addBindValue(horaEntrada);
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

QString DbManager::nombreCompleto(int idEmpleado)
{
    QSqlQuery query;
    query.prepare("SELECT id, nombre || ' ' || apellido_paterno || ' ' || "
                  "apellido_materno AS nombre_completo FROM empleado WHERE id = ?");
    query.addBindValue(idEmpleado);
    query.exec();
    query.next();
    return query.value("nombre_completo").toString();
}

bool DbManager::insertarRegistro(int idEmpleado, QDate fecha, QString entrada, QString salida)
{
    QSqlQuery query;
    QString fechaString = fecha.toString(Qt::ISODate);
    if (salida.isEmpty())
    {
        query.prepare("INSERT INTO registro VALUES (NULL, ?, ?, ?, NULL)");
        query.addBindValue(idEmpleado);
        query.addBindValue(fechaString);
        query.addBindValue(entrada);
    }
    else
    {
        query.prepare("INSERT INTO registro VALUES (NULL, ?, ?, ?, ?)");
        query.addBindValue(idEmpleado);
        query.addBindValue(fechaString);
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
    QSqlQuery query;
    QPair<QString, QString> tiempos = diasSemanaColumnas.at(dia.dayOfWeek());
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

bool DbManager::insertarRegistrosVacios(QDate dia)
{
    QSqlQuery query;
    QPair<QString, QString> tiempos = diasSemanaColumnas.at(dia.dayOfWeek());
    QString diaString = dia.toString(Qt::ISODate);
    query.prepare("INSERT INTO registro SELECT NULL, empleado.id, '" + diaString + "', NULL, NULL FROM empleado, horario "
                  "ON empleado.id = horario.id WHERE " + tiempos.first + " <> '' AND " + tiempos.second + " <> ''");
    return query.exec();
}

bool DbManager::hayRegistros(QDate dia)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) FROM registro WHERE fecha = ?");
    query.addBindValue(dia.toString(Qt::ISODate));
    query.exec();
    query.next();
    int registros = query.value(0).toInt();
    return registros > 0;
}

QSqlQuery DbManager::historialCompletoEmpleado(int idEmpleado, QDate inicio, QDate fin)
{
    QSqlQuery query;
    query.prepare(sqlDiferenciasEmpleado);
    query.addBindValue(idEmpleado);
    query.addBindValue(inicio.toString(Qt::ISODate));
    query.addBindValue(fin.toString(Qt::ISODate));
    query.exec();
    return query;
}

QSqlQuery DbManager::historialCompletoTodos(QDate inicio, QDate fin)
{
    QSqlQuery query;
    query.prepare(sqlDiferenciasTodos);
    query.addBindValue(inicio.toString(Qt::ISODate));
    query.addBindValue(fin.toString(Qt::ISODate));
    query.exec();
    return query;
}

QDate DbManager::fechaRegistroEmpleado(int empleado) {
    QSqlQuery query;
    query.prepare("SELECT fecha_registro FROM empleado WHERE id = ?");
    query.addBindValue(empleado);
    query.exec();
    query.next();
    QString iso = query.value("fecha_registro").toString();
    return QDate::fromString(iso, Qt::ISODate);
}

QString DbManager::horarioEntradaDia(int empleado, QDate dia)
{
    QSqlQuery query;
    QString diaString = dia.toString(Qt::ISODate);
    query.prepare("SELECT CASE strftime('%w', '" + diaString + "') WHEN '0' THEN domingo_e "
                "WHEN '1' THEN lunes_e WHEN '2' THEN martes_e WHEN '3' THEN miercoles_e "
                "WHEN '4' THEN jueves_e WHEN '5' THEN viernes_e WHEN '6' THEN sabado_e END "
                "AS e_normal FROM horario WHERE horario.id = ?");
    query.addBindValue(empleado);
    query.exec();
    query.next();
    return query.value("e_normal").toString();
}

bool DbManager::hayAdministradores()
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(id) FROM usuario WHERE privilegios = 0");
    query.exec();
    query.next();
    int admins = query.value(0).toInt();
    return admins > 0;
}

bool DbManager::agregarUsuario(QString nombre, QString pass, int privs)
{
    QSqlQuery query;
    query.prepare("INSERT INTO usuario VALUES (NULL, ?, ?, ?)");
    query.addBindValue(nombre);
    query.addBindValue(pass);
    query.addBindValue(privs);
    bool funciono = query.exec();
    if (funciono) ultimoIdInsertado = query.lastInsertId().toInt();
    return funciono;
}

int DbManager::ultimoId()
{
    return ultimoIdInsertado;
}

int DbManager::verificarCredenciales(QString usuario, QString pass)
{
    QSqlQuery query;
    query.prepare("SELECT id FROM usuario WHERE nombre = ? AND password = ?");
    query.addBindValue(usuario);
    query.addBindValue(pass);
    query.exec();
    query.next();
    if (!query.isValid()) return -1;
    return query.value(0).toInt();
}

int DbManager::conseguirPrivilegios(int idUsuario)
{
    QSqlQuery query;
    query.prepare("SELECT privilegios FROM usuario WHERE id = ?");
    query.addBindValue(idUsuario);
    query.exec();
    query.next();
    return query.value(0).toInt();
}

QSqlQueryModel* DbManager::conseguirUsuarios()
{
    QSqlQueryModel *modelo = new QSqlQueryModel;
    modelo->setQuery("SELECT nombre, id, privilegios FROM usuario");
    return modelo;
}

bool DbManager::actualizarUsuario(int id, QString nombreNuevo, QString pass, int privs)
{
    QSqlQuery query;
    query.prepare("UPDATE usuario SET nombre = ?, password = ?, privilegios = ? WHERE id = ?");
    query.addBindValue(nombreNuevo);
    query.addBindValue(pass);
    query.addBindValue(privs);
    query.addBindValue(id);
    return query.exec();
}

bool DbManager::eliminarUsuario(int id)
{
    QSqlQuery query;
    query.prepare("DELETE FROM usuario WHERE id = ?");
    query.addBindValue(id);
    return query.exec();
}

