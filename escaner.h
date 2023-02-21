#ifndef ESCANER_H
#define ESCANER_H

#include <QCamera>
#include <QMediaCaptureSession>
#include <QWidget>
#include <QZXing.h>

namespace Ui {
class Escaner;
}

class Escaner : public QWidget
{
    Q_OBJECT

public:
    explicit Escaner(QWidget *parent = nullptr);
    ~Escaner();

    void activarCamara();
    void desactivarCamara();

public slots:
    void setId(int id);

private slots:
    void procesarFrame(const QVideoFrame &frame);

    void on_guardarButton_clicked();
    void on_codigoEdit_textChanged(const QString &arg1);

private:
    Ui::Escaner *ui;

    QCamera *camara = nullptr;
    QMediaCaptureSession sesion;
    QZXing *decoder;
    QTimer *timer;

    int idRegistro = -1;
};

#endif // ESCANER_H
