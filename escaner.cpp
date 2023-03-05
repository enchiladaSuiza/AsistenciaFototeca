#include "dbmanager.h"
#include "escaner.h"
#include "ui_escaner.h"

#include <QFileDialog>
#include <QMediaDevices>
#include <QSqlQuery>
#include <QTimer>
#include <QVideoSink>
#include <cameramanager.h>

Escaner::Escaner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Escaner)
{
    ui->setupUi(this);

    timer = new QTimer(this);

    decoder = new QZXing;
    decoder->setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    decoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    sesion.setVideoOutput(ui->viewfinder);
    ui->viewfinder->setAspectRatioMode(Qt::KeepAspectRatioByExpanding);

    connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Escaner::procesarFrame);
}

Escaner::~Escaner()
{
    delete ui;
}

void Escaner::procesarFrame(const QVideoFrame &frame)
{
    QString resultado = decoder->decodeImage(frame.toImage());
    if (!resultado.isEmpty()) ui->codigoEdit->setText(resultado);
}

void Escaner::activarCamara()
{
    if (camara == nullptr)
    {
        camara = CameraManager::nuevaCamara();
        sesion.setCamera(camara);
        camara->start();
    }
}

void Escaner::desactivarCamara()
{
    if (camara != nullptr)
    {
        camara->stop();
        sesion.setCamera(nullptr);
        camara = nullptr;
    }
}

void Escaner::on_guardarButton_clicked()
{
    QString qr = ui->codigoEdit->text();
    ui->guardarButton->setEnabled(false);
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

void Escaner::on_exportarButton_clicked()
{
    QString texto = ui->codigoEdit->text();
    QString nombreArchivo = QFileDialog::getSaveFileName(
                nullptr, "Elija la ubicación donde exportar el ćodigo QR.",
                texto + "-QR.png", "Imagen (*.png)");
    QImage imagen = QZXing::encodeData(
                texto, QZXing::EncoderFormat_QR_CODE,
                QSize(240, 240), QZXing::EncodeErrorCorrectionLevel_H, true);
}

