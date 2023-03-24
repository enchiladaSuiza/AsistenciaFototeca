#include "cameramanager.h"
#include "dbmanager.h"
#include "foto.h"
#include "ui_foto.h"

#include <QFileDialog>

Foto::Foto(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Foto)
{
    ui->setupUi(this);

    sesion.setVideoOutput(ui->viewfinder);
    ui->viewfinder->setAspectRatioMode(Qt::KeepAspectRatio);
    ui->viewfinder->setVisible(false);

    captura = new QImageCapture;
    sesion.setImageCapture(captura);
    connect(captura, &QImageCapture::imageCaptured, this, &Foto::guardarFoto);
}

Foto::~Foto()
{
    delete ui;
}

void Foto::activarCamara()
{
    if (camara == nullptr && ui->camaraCbox->isChecked())
    {
        ui->viewfinder->setVisible(true);
        camara = CameraManager::nuevaCamara();
        sesion.setCamera(camara);
        camara->start();
    }
}

void Foto::desactivarCamara()
{
    if (camara != nullptr)
    {
        ui->viewfinder->setVisible(false);
        camara->stop();
        sesion.setCamera(nullptr);
        camara = nullptr;
    }
}

void Foto::on_tomarButton_clicked()
{
    captura->capture();
}

void Foto::guardarFoto(int id, const QImage &imagen)
{
    Q_UNUSED(id);
    QString nombreEmpleado = DbManager::nombreCompleto(idEmpleado);
    QString nombreArchivo = QFileDialog::getSaveFileName(
                nullptr, "Elija la ubicación donde guardar la fotografía.",
                "Imagen-" + nombreEmpleado + ".jpg", "Imagen (*.jpg)");
    imagen.save(nombreArchivo);
    colocarImagen(nombreArchivo);
    DbManager::actualizarImagenEmpleado(idEmpleado, nombreArchivo);
}

void Foto::actualizarImagen()
{
    QString ruta = DbManager::conseguirImagenEmpleado(idEmpleado);
    colocarImagen(ruta);
}

void Foto::quitarImagen()
{
    ui->rutaLabel->setText("No hay imagen establecida.");
    ui->imagenLabel->clear();
}

bool Foto::colocarImagen(QString ruta)
{
    if (ruta.isEmpty())
    {
        quitarImagen();
        return false;
    }
    ui->rutaLabel->setText(ruta);
    QImage imagen(ruta);
    if (!imagen.isNull())
    {
        QImage escalada = imagen.scaled(ui->imagenLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
        ui->imagenLabel->setPixmap(QPixmap::fromImage(escalada));
    }
    return true;
}


void Foto::setId(int id)
{
    idEmpleado = id;
}

void Foto::on_seleccionarButton_clicked()
{
    QString ruta = QFileDialog::getOpenFileName(nullptr, "Seleccione una imagen", "./", "Imágenes (*.jpg, *.png)");
    if (colocarImagen(ruta)) DbManager::actualizarImagenEmpleado(idEmpleado, ruta);
}

void Foto::on_descartarButton_clicked()
{
    quitarImagen();
    DbManager::actualizarImagenEmpleado(idEmpleado, "");
}

void Foto::on_camaraCbox_stateChanged(int estado)
{
    if (estado == Qt::Checked)
    {
        activarCamara();
        ui->tomarButton->setEnabled(true);
        ui->imagenLabel->setVisible(false);
    }
    else
    {
        desactivarCamara();
        ui->tomarButton->setEnabled(false);
        ui->imagenLabel->setVisible(true);
    }
}

