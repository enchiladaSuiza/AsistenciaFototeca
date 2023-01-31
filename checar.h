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

    static int getFrameCounter();
    void activarCamara();
    void desactivarCamara();

private slots:
    void procesarFrame(const QVideoFrame &frame);

private:
    Ui::Checar *ui;

    QCamera *camara;
    QMediaCaptureSession sesion;
    QZXing *decoder;

    bool capturando = false;
    int frameCounter = 0;

    void mostrarInformacion(bool mostrar);
};

#endif // CHECAR_H
