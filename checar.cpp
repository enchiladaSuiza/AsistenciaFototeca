#include "checar.h"
#include "ui_checar.h"
#include "dbmanager.h"
#include "cameramanager.h"

#include <QDate>
#include <QVideoWidget>
#include <QCamera>
#include <QMediaDevices>
#include <QSqlRecord>
#include <QSqlField>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    entradaEstablecida = new QTableWidgetItem;
    entradaCapturada = new QTableWidgetItem;
    salidaEstablecida = new QTableWidgetItem;
    salidaCapturada = new QTableWidgetItem;
    entradaEstablecida->setTextAlignment(Qt::AlignCenter);
    entradaCapturada->setTextAlignment(Qt::AlignCenter);
    salidaEstablecida->setTextAlignment(Qt::AlignCenter);
    salidaCapturada->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(0, 0, entradaEstablecida);
    ui->tableWidget->setItem(0, 1, entradaCapturada);
    ui->tableWidget->setItem(1, 0, salidaEstablecida);
    ui->tableWidget->setItem(1, 1, salidaCapturada);

    timerInfo = new QTimer;
    connect(timerInfo, &QTimer::timeout, this, &Checar::restablecerPantalla);

    timerEscaneo = new QTimer;
    connect(timerEscaneo, &QTimer::timeout, this, &Checar::habilitarEscaneo);

    decoder = new QZXing;
    decoder->setDecoder(QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13);
    decoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    activarCamara();
    sesion.setVideoOutput(ui->viewfinder);
    ui->viewfinder->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);

    connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Checar::procesarFrame);
}

Checar::~Checar()
{
    desactivarCamara();
    delete ui;
}

void Checar::limpiarInformacion()
{
    ui->labelEmpleado->setText("");
    entradaEstablecida->setText("");
    entradaCapturada->setText("");
    salidaEstablecida->setText("");
    salidaCapturada->setText("");
}

void Checar::llenarInformacion(QString info, QString nombre,
                               QString entradaNormal, QString salidaNormal,
                               QString entradaCaptura, QString salidaCaptura)
{
    ui->labelEstado->setText(info);
    ui->labelEmpleado->setText(nombre);
    entradaEstablecida->setText(entradaNormal);
    entradaCapturada->setText(entradaCaptura);
    salidaEstablecida->setText(salidaNormal);
    salidaCapturada->setText(salidaCaptura);
}

void Checar::procesarFrame(const QVideoFrame &frame)
{
    if (!escaneando) return;

    QString codigo = decoder->decodeImage(frame.toImage());
    if (codigo.isEmpty()) return;

    QSqlRecord empleado = DbManager::empleadoPorQR(codigo);
    if (empleado.field(0).typeID() != 1)
    {
        ui->labelEstado->setText("- Código desconocido -");
        timerInfo->start(tiempoInformacion);
        return;
    }

    escaneando = false;
    timerEscaneo->start(tiempoNoEscaneo);

    int idEmpleado = empleado.value("id").toInt();
    QString nombre = empleado.value("nombre").toString();

    QDate hoy = QDate::currentDate();
    int diaSemana = hoy.dayOfWeek();
    QString ahora = QTime::currentTime().toString("hh:mm");

    QSqlRecord horarios = DbManager::horariosPorEmpleado(idEmpleado);
    QPair<QString, QString> tiempos = DbManager::diasSemanaColumnas.at(diaSemana);
    QString entradaNormal = horarios.value(tiempos.first).toString();
    QString salidaNormal = horarios.value(tiempos.second).toString();

    if (entradaNormal.isEmpty() || salidaNormal.isEmpty()) // ¿Hoy trabaja?
    {
        llenarInformacion("- Usted no trabaja el día de hoy -", nombre);
        timerInfo->start(tiempoInformacion);
        return;
    }

    QSqlRecord capturas = DbManager::capturasDeEmpleadoEnFecha(idEmpleado, hoy);
    int idRegistro = capturas.value("id").toInt();
    QString entrada = capturas.value("hora_entrada").toString();
    QString salida = capturas.value("hora_salida").toString();

    if (entrada.isEmpty())
    {
        // DbManager::updateCapturaHoraEntrada(idRegistro, ahora);
        DbManager::insertarRegistro(idEmpleado, hoy, ahora);
        llenarInformacion("- Entrada caputrada -", nombre, entradaNormal, salidaNormal, ahora);
        timerInfo->start(tiempoInformacion);
        return;
    }

    if (salida.isEmpty())
    {
        DbManager::updateCapturaHoraSalida(idRegistro, ahora);
        llenarInformacion("- Salida caputrada -", nombre, entradaNormal, salidaNormal, entrada, ahora);
        timerInfo->start(tiempoInformacion);
        return;
    }

    llenarInformacion("- Su salida ya fue capturada antes -", nombre, entradaNormal, salidaNormal, entrada, salida);
    timerInfo->start(tiempoInformacion);
}

void Checar::activarCamara()
{
    if (camara == nullptr)
    {
        camara = CameraManager::nuevaCamara();
        sesion.setCamera(camara);
        camara->start();
    }
}

void Checar::desactivarCamara()
{
    if (camara != nullptr)
    {
        camara->stop();
        sesion.setCamera(nullptr);
        camara = nullptr;
    }
}

void Checar::restablecerPantalla()
{
    ui->labelEstado->setText("- Buscando código QR -");
    ui->labelEmpleado->setText("");
    limpiarInformacion();
}

void Checar::habilitarEscaneo() {
    escaneando = true;
}
