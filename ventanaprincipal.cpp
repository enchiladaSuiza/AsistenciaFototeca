#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

#include <QShortcut>
#include <QTime>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    new QShortcut(Qt::CTRL | Qt::Key_Q, this, SLOT(close()));

    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Mexico));

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
//    ui->fechaLabel->setText(QDate::currentDate().toString("d/MMM/yyyy (ddd)"));
//    ui->horaLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
    ui->fechaLabel->setText(QLocale().toString(QDate::currentDate(), "d/MMM/yyyy (ddd)"));
    ui->horaLabel->setText(QLocale().toString(QTime::currentTime(), "hh::mm::ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    (void)event;
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


void VentanaPrincipal::on_stackedWidget_currentChanged(int actual)
{
    if (actual == 1)
    {
        checar.desactivarCamara();
    }
    else
    {
        checar.activarCamara();
    }
}

