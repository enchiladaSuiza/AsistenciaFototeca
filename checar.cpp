#include "checar.h"
#include "qcamera.h"
#include "ui_checar.h"
#include "QZXing.h"

#include <QDate>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QDir>
#include <QRandomGenerator>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);
    mostrarInformacion(false);

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
    ui->entradaNormalLabel->setVisible(mostrar);
    ui->entradaCapturaLabel->setVisible(mostrar);
    ui->salidaNormalLabel->setVisible(mostrar);
    ui->salidaCapturaLabel->setVisible(mostrar);
}

void Checar::procesarFrame(const QVideoFrame &frame)
{
    // qDebug() << "Procesando " << ++frameCounter;
    QImage imagen = frame.toImage();
    QString resultado = decoder->decodeImage(imagen);
    if (!resultado.isEmpty())
    {
        qDebug() << resultado;
    }
}

void Checar::activarCamara()
{
    if (capturando)
    {
        return;
    }
    camara = new QCamera(QMediaDevices::defaultVideoInput());
    sesion.setCamera(camara);
    camara->start();
    capturando = true;
}

void Checar::desactivarCamara()
{
    if (capturando)
    {
        sesion.setCamera(nullptr);
        camara->stop();
        delete camara;
        capturando = false;
    }
}

