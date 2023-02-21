#ifndef CHECAR_H
#define CHECAR_H

#include <QCamera>
#include <QMediaCaptureSession>
#include <QWidget>
#include <QImageCapture>
#include <QImage>
#include <QVideoSink>
#include <QTimer>
#include <QZXing.h>
#include <QTableWidgetItem>
#include <QStatusBar>

namespace Ui {
class Checar;
}

class Checar : public QWidget
{
    Q_OBJECT

public:
    explicit Checar(QWidget *parent = nullptr);
    ~Checar();

    void activarCamara();
    void desactivarCamara();

    void limpiarInformacion();
    void llenarInformacion(QString info = "", QString nombre = "",
                           QString entradaNormal = "", QString salidaNormal = "",
                           QString entradaCaptura = "", QString salidaCaptura = "");

private slots:
    void procesarFrame(const QVideoFrame &frame);
    void restablecerPantalla();

private:
    Ui::Checar *ui;

    QCamera *camara = nullptr;
    QMediaCaptureSession sesion;
    QZXing *decoder;
    QTimer *timerInfo;

    QTableWidgetItem* entradaEstablecida;
    QTableWidgetItem* entradaCapturada;
    QTableWidgetItem* salidaEstablecida;
    QTableWidgetItem* salidaCapturada;

    const int tiempoInformacion = 5000;
};

#endif // CHECAR_H
