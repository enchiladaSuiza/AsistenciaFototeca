#ifndef FOTO_H
#define FOTO_H

#include <QCamera>
#include <QImageCapture>
#include <QMediaCaptureSession>
#include <QWidget>

namespace Ui {
class Foto;
}

class Foto : public QWidget
{
    Q_OBJECT

public:
    explicit Foto(QWidget *parent = nullptr);
    ~Foto();

    void activarCamara();
    void desactivarCamara();

    void actualizarImagen();
public slots:
    void setId(int idEmpleado);

private slots:
    void on_tomarButton_clicked();
    void guardarFoto(int id, const QImage &imagen);

    void on_seleccionarButton_clicked();

    void on_descartarButton_clicked();

    void on_camaraCbox_stateChanged(int arg1);

private:
    Ui::Foto *ui;
    QCamera *camara = nullptr;
    QMediaCaptureSession sesion;
    QImageCapture *captura;

    int idEmpleado = -1;
    bool colocarImagen(QString ruta);
    void quitarImagen();
};

#endif // FOTO_H
