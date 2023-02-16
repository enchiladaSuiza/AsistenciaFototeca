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

    checar = new Checar(this);
    personal = new Personal(this);
    historial = new Historial(this);

    ui->stackedWidget->addWidget(checar);
    ui->stackedWidget->addWidget(personal);
    ui->stackedWidget->addWidget(historial);
}

VentanaPrincipal::~VentanaPrincipal() { delete ui; }

void VentanaPrincipal::actualizarTiempo()
{
    ui->fechaLabel->setText(QLocale().toString(QDate::currentDate(), "d/MMM/yyyy (ddd)"));
    ui->horaLabel->setText(QLocale().toString(QTime::currentTime(), "hh::mm::ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    ui->horaLabel->setText(QTime::currentTime().toString("hh:mm:ss"));
}

void VentanaPrincipal::on_checarButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(checar);
}

void VentanaPrincipal::on_personalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(personal);
}

void VentanaPrincipal::on_pushButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(historial);
}

void VentanaPrincipal::on_stackedWidget_currentChanged(int actual)
{
    if (actual >= 1)
    {
        if (actual == 2) historial->actualizarConsulta();
        checar->desactivarCamara();
        return;
    }
    checar->activarCamara();
}
