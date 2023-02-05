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

    void mostrarInformacion(bool mostrar);
    void llenarInformacion(QString info = "", QString nombre = "", QString entradaNormal = "", QString salidaNormal = "", QString entradaCaptura = "", QString salidaCaptura = "");

private slots:
    void procesarFrame(const QVideoFrame &frame);
    void restablecerPantalla();
    void resumirProcesamiento();

private:
    Ui::Checar *ui;

    QCamera *camara = nullptr;
    QMediaCaptureSession sesion;
    QZXing *decoder;
    QTimer *timerInfo;
    QTimer *timerCamara;

    QList<QPair<QString, QString>> diasSemanaColumnas = {
        qMakePair("", ""),
        qMakePair("lunes_e", "lunes_s"),
        qMakePair("martes_e", "martes_s"),
        qMakePair("miercoles_e", "miercoles_s"),
        qMakePair("jueves_e", "jueves_s"),
        qMakePair("viernes_e", "viernes_s"),
        qMakePair("sabado_e", "sabado_s"),
        qMakePair("domingo_e", "domingo_s"),
    };

    const int tiempoInformacion = 5000;
};

#endif // CHECAR_H
