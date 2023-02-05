#include "checar.h"
#include "qcamera.h"
#include "ui_checar.h"
#include "QZXing.h"

#include <QDate>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QDir>
#include <QRandomGenerator>
#include <QSqlQuery>
#include <QSqlQuery>
#include <dbmanager.h>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);
    mostrarInformacion(false);

    timerInfo = new QTimer(this);
    connect(timerInfo, &QTimer::timeout, this, &Checar::restablecerPantalla);

    timerCamara = new QTimer(this);
    connect(timerCamara, &QTimer::timeout, this, &Checar::resumirProcesamiento);

    decoder = new QZXing;
    decoder->setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    decoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    activarCamara();
    sesion.setVideoOutput(ui->viewfinder);

    connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Checar::procesarFrame);
}

Checar::~Checar()
{
    delete ui;
}

void Checar::mostrarInformacion(bool mostrar)
{
    ui->nombreLabel->setVisible(mostrar);
    ui->entradaNormalLabel->setVisible(mostrar);
    ui->entradaCapturaLabel->setVisible(mostrar);
    ui->salidaNormalLabel->setVisible(mostrar);
    ui->salidaCapturaLabel->setVisible(mostrar);
}

void Checar::llenarInformacion(QString info, QString nombre,
                               QString entradaNormal, QString salidaNormal,
                               QString entradaCaptura, QString salidaCaptura)
{
    mostrarInformacion(true);
    ui->label->setText(info);
    ui->nombreLabel->setText(nombre);
    ui->entradaNormalLabel->setText("Entrada\n" + entradaNormal);
    ui->entradaCapturaLabel->setText("Captura\n" + entradaCaptura);
    ui->salidaNormalLabel->setText("Salida\n" + salidaNormal);
    ui->salidaCapturaLabel->setText("Captura\n" + salidaCaptura);
}

void Checar::procesarFrame(const QVideoFrame &frame)
{
    QString codigo = decoder->decodeImage(frame.toImage());

    if (codigo.isEmpty()) return;

    QSqlQuery empleado = DbManager::nombreCompletoPorQR(codigo);
    if (!empleado.isValid())
    {
        ui->label->setText("Código desconocido.");
        timerInfo->start(tiempoInformacion);
        return;
    }

    //desactivarCamara();
    //timerCamara->start(1000);
    //disconnect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Checar::procesarFrame);

    QString ahora = QTime::currentTime().toString("h:mm");
    int diaSemana = QDate::currentDate().dayOfWeek();

    int idEmpleado = empleado.value("id").toInt();
    QString nombre = empleado.value("nombre").toString();
    QString apPaterno = empleado.value("apellido_paterno").toString();
    QString apMaterno = empleado.value("apellido_materno").toString();
    QString nombreCompleto = nombre + " " + apPaterno + " " + apMaterno;

    QSqlQuery horarios = DbManager::horariosPorEmpleado(idEmpleado);
    QPair<QString, QString> tiempos = diasSemanaColumnas.at(diaSemana);
    QString entradaNormal = horarios.value(tiempos.first).toString();
    QString salidaNormal = horarios.value(tiempos.second).toString();

    QSqlQuery capturas = DbManager::capturasPorEmpleadoFecha(idEmpleado);
    if (capturas.isValid()) // ¿Ya escaneó hoy?
    {
        QString entrada = capturas.value("hora_entrada").toString();

        if (entrada == ahora) return; // ¿Apenas escaneó su entrada?

        QString salida = capturas.value("hora_salida").toString();

        if (salida.isEmpty())
        {
            int idRegistro = capturas.value("id").toInt();
            if (DbManager::updateCapturaHoraSalida(idRegistro))
            {
                llenarInformacion("Salida caputrada.", nombreCompleto, entradaNormal, salidaNormal, entrada, ahora);
                timerInfo->start(tiempoInformacion);
            }
            return;
        }
        if (salida != ahora)
        {
            ui->label->setText("Su salida ya fue capturada.");
            timerInfo->start(tiempoInformacion);
        }
        return;
    }

    if (DbManager::insertarRegistro(idEmpleado, ahora))
    {
        QSqlQuery horarios = DbManager::horariosPorEmpleado(idEmpleado);

        int diaSemana = QDate::currentDate().dayOfWeek();
        QPair<QString, QString> tiempos = diasSemanaColumnas.at(diaSemana);
        QString entradaNormal = horarios.value(tiempos.first).toString();
        QString salidaNormal = horarios.value(tiempos.second).toString();

        llenarInformacion("Entrada caputrada.", nombreCompleto, entradaNormal, salidaNormal, ahora, "-");
        timerInfo->start(tiempoInformacion);
    }
}

void Checar::activarCamara()
{
    if (camara == nullptr)
    {
        camara = new QCamera(QMediaDevices::defaultVideoInput());
        sesion.setCamera(camara);
        camara->start();
    }
}

void Checar::desactivarCamara()
{
    if (camara == nullptr)
    {
        return;
    }
    sesion.setCamera(nullptr);
    camara->stop();
    camara = nullptr;
}

void Checar::restablecerPantalla()
{
    ui->label->setText("Buscando código QR...");
    mostrarInformacion(false);
}

void Checar::resumirProcesamiento()
{
    qDebug() << "Eh hello izza me";
    connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Checar::procesarFrame);
    activarCamara();
}
