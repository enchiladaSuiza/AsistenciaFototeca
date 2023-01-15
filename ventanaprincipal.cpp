#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

#include <QTime>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    startTimer(1000);
    actualizarTiempo();

    ui->stackedWidget->addWidget(&checar);
    ui->stackedWidget->addWidget(&personal);
}

VentanaPrincipal::~VentanaPrincipal()
{
    delete ui;
}

void VentanaPrincipal::actualizarTiempo()
{
    ui->fechaLabel->setText(QDate::currentDate().toString("d/MMM/yyyy (ddd)"));
    ui->horaLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    ui->horaLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}


void VentanaPrincipal::on_checarButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(&checar);
}

void VentanaPrincipal::on_personalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(&personal);
}

