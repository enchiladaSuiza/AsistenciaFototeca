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

    void actualizarImagen();
public slots:
    void setId(int idEmpleado);

private slots:
    void guardarFoto(int id, const QImage &imagen);
    void on_seleccionarButton_clicked();
    void on_descartarButton_clicked();

private:
    Ui::Foto *ui;

    int idEmpleado = -1;
    bool colocarImagen(QString ruta);
    void quitarImagen();
};

#endif // FOTO_H
