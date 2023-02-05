#include "dbmanager.h"
#include "escaner.h"
#include "ui_escaner.h"

#include <QMediaDevices>
#include <QSqlQuery>
#include <QTimer>
#include <QVideoSink>

Escaner::Escaner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Escaner)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    // connect(timer, &QTimer::timeout, this, &Escaner::activarProcesamiento);

    decoder = new QZXing;
    decoder->setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    decoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    sesion.setVideoOutput(ui->viewfinder);

    habilitarProcesamiento(true);
}

Escaner::~Escaner()
{
    delete ui;
}

void Escaner::procesarFrame(const QVideoFrame &frame)
{
    QString resultado = decoder->decodeImage(frame.toImage());
    if (!resultado.isEmpty())
    {
        // habilitarProcesamiento(false);
        // timer->start(cooldownProcesamiento);
        ui->codigoEdit->setText(resultado);
    }
}

void Escaner::habilitarProcesamiento(bool habilitar)
{
    if (habilitar)
    {
        connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Escaner::procesarFrame);
        return;
    }
    disconnect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Escaner::procesarFrame);
}

void Escaner::activarProcesamiento()
{
    habilitarProcesamiento(true);
}

void Escaner::activarCamara()
{
    if (camara == nullptr)
    {
        camara = new QCamera(QMediaDevices::defaultVideoInput());
        sesion.setCamera(camara);
        camara->start();
    }
}

void Escaner::desactivarCamara()
{
    if (camara == nullptr)
    {
        return;
    }
    sesion.setCamera(nullptr);
    camara->stop();
    camara = nullptr;
}


void Escaner::on_guardarButton_clicked()
{
    QString qr = ui->codigoEdit->text();
    ui->guardarButton->setEnabled(false);
//    habilitarProcesamiento(false);
//    timer->start(cooldownProcesamiento);

    DbManager::actualizarQREmpleado(idRegistro, qr);   
}

void Escaner::setId(int id)
{
    idRegistro = id;

    QString codigo = DbManager::qrPorId(id);
    ui->codigoEdit->setText(codigo);
}

void Escaner::on_codigoEdit_textChanged(const QString &texto)
{
    Q_UNUSED(texto);
    ui->guardarButton->setEnabled(true);
}



