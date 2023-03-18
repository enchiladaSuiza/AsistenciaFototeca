#ifndef CHECAR_H
#define CHECAR_H

#include <QCamera>
#include <QMediaCaptureSession>
#include <QWidget>
#include <QImageCapture>
#include <QImage>
#include <QVideoSink>
#include <QTimer>
#include <QTableWidgetItem>
#include <QStatusBar>

#include "QZXing/QZXing.h"

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
    void llenarInformacion(QString info = "", QString rutaImagen = "", QString nombre = "",
                           QString entradaNormal = "", QString salidaNormal = "",
                           QString entradaCaptura = "", QString salidaCaptura = "");

    void habilitarEscaneo();

private slots:
    void procesarFrame(const QVideoFrame &frame);
    void restablecerPantalla();

private:
    Ui::Checar *ui;

    QCamera *camara = nullptr;
    QMediaCaptureSession sesion;
    QZXing *decoder;
    QTimer *timerInfo;
    QTimer *timerEscaneo;

    QTableWidgetItem* entradaEstablecida;
    QTableWidgetItem* entradaCapturada;
    QTableWidgetItem* salidaEstablecida;
    QTableWidgetItem* salidaCapturada;

    const int tiempoInformacion = 5000;
    const int tiempoNoEscaneo = 1000;
    bool escaneando = true;
};

#endif // CHECAR_H
