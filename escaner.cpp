#include "escaner.h"
#include "ui_escaner.h"

#include <QMediaDevices>
#include <QSqlQuery>
#include <QVideoSink>

Escaner::Escaner(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Escaner)
{
    ui->setupUi(this);

    decoder = new QZXing;
    decoder->setDecoder( QZXing::DecoderFormat_QR_CODE | QZXing::DecoderFormat_EAN_13 );
    decoder->setSourceFilterType(QZXing::SourceFilter_ImageNormal);
    decoder->setTryHarderBehaviour(QZXing::TryHarderBehaviour_ThoroughScanning | QZXing::TryHarderBehaviour_Rotate);

    sesion.setVideoOutput(ui->viewfinder);

    connect(ui->viewfinder->videoSink(), &QVideoSink::videoFrameChanged, this, &Escaner::procesarFrame);
}

Escaner::~Escaner()
{
    delete ui;
}

void Escaner::procesarFrame(const QVideoFrame &frame)
{
    // qDebug() << "Procesando " << ++frameCounter;
    QImage imagen = frame.toImage();
    QString resultado = decoder->decodeImage(imagen);
    if (!resultado.isEmpty())
    {
        ui->codigoEdit->setText(resultado);
    }
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
    QSqlQuery query;
    query.prepare("UPDATE empleado SET qr = ? WHERE id = ?");
    query.addBindValue(qr);
    query.addBindValue(idRegistro);
    if (!query.exec())
    {
        qDebug() << "Query fallida";
        return;
    }
    ui->guardarButton->setEnabled(false);
}

void Escaner::setId(int id)
{
    idRegistro = id;
    QSqlQuery query;
    query.prepare("SELECT qr FROM empleado WHERE id = ?");
    query.addBindValue(id);
    query.exec();
    while (query.next())
    {
        QString codigo = query.value(0).toString();
        ui->codigoEdit->setText(codigo);
        ui->guardarButton->setEnabled(false);
    }
}

void Escaner::on_codigoEdit_textChanged(const QString &texto)
{
    Q_UNUSED(texto);
    ui->guardarButton->setEnabled(true);
}

