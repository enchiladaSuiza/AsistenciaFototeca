#include "checar.h"
#include "qcamera.h"
#include "ui_checar.h"

#include <QDate>
#include <QMediaCaptureSession>
#include <QVideoWidget>
#include <QMediaDevices>
#include <QGraphicsVideoItem>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);
    mostrarInformacion(false);

    QVideoWidget* viewFinder = new QVideoWidget;

    camera = new QCamera;

    QMediaCaptureSession session;
    session.setCamera(camera);
    session.setVideoOutput(viewFinder);
    camera->start();
    viewFinder->show();
}

void Checar::mostrarInformacion(bool mostrar)
{
    ui->entradaNormalLabel->setVisible(mostrar);
    ui->entradaCapturaLabel->setVisible(mostrar);
    ui->salidaNormalLabel->setVisible(mostrar);
    ui->salidaCapturaLabel->setVisible(mostrar);
}

Checar::~Checar()
{
    delete ui;
}

void Checar::on_toggleCamera_clicked()
{
    camera->setActive(!camera->isActive());
}

