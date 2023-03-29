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
}

Foto::~Foto()
{
    delete ui;
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
    QString ruta = QFileDialog::getOpenFileName(nullptr, "Seleccione una imagen", "./", "Imágenes (*.jpg *.jpeg *.png)");
    if (colocarImagen(ruta)) DbManager::actualizarImagenEmpleado(idEmpleado, ruta);
}

void Foto::on_descartarButton_clicked()
{
    quitarImagen();
    DbManager::actualizarImagenEmpleado(idEmpleado, "");
}


