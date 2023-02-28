#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

#include <QShortcut>
#include <QTime>
#include <QPixmap>
#include <cameramanager.h>
#include <QSvgWidget>
#include <QSvgRenderer>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    new QShortcut(Qt::CTRL | Qt::Key_Q, this, SLOT(close()));

    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Mexico));

    startTimer(1000);
    actualizarTiempo();

//    QPixmap sc(":/imagenes/SC.png");
//    QPixmap sinafo(":/imagenes/SINAFO.png");
//    QPixmap inah(":/imagenes/INAH.png");

//    ui->logoSC->setPixmap(sc.scaled(200, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    ui->logoInah->setPixmap(inah.scaled(150, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
//    ui->logoSinafo->setPixmap(sinafo.scaled(150, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));

//    ui->logoSinafo->setPixmap(sinafo);
//    ui->logoInah->setPixmap(inah);
//    ui->logoSC->setPixmap(sc);

    QSvgWidget *cultura = new QSvgWidget(":/svg/CULTURA.svg");
    QSvgWidget *inah = new QSvgWidget(":/svg/INAH.svg");
    QSvgWidget *sinafo = new QSvgWidget(":/svg/SINAFO.svg");

    cultura->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
    inah->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
    sinafo->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);

    cultura->setFixedSize(150, 60);
    inah->setFixedSize(110, 40);
    sinafo->setFixedSize(110, 40);

    ui->headerLayout->insertWidget(0, sinafo);
    ui->headerLayout->insertWidget(0, inah);
    ui->headerLayout->insertWidget(0, cultura);

    ui->headerLayout->setAlignment(cultura, Qt::AlignLeft);
    ui->headerLayout->setAlignment(inah, Qt::AlignCenter);
    ui->headerLayout->setAlignment(sinafo, Qt::AlignCenter);

    checar = new Checar(this);
    personal = new Personal(this);
    historial = new Historial(this);
    opciones = new Opciones(this);

    ui->stackedWidget->addWidget(checar);
    ui->stackedWidget->addWidget(personal);
    ui->stackedWidget->addWidget(historial);
    ui->stackedWidget->addWidget(opciones);
    ui->checarButton->setEnabled(false);
}

VentanaPrincipal::~VentanaPrincipal() { delete ui; }

void VentanaPrincipal::actualizarTiempo()
{  
    ui->horaLabel->setText(QLocale().toString(QDateTime::currentDateTime(), "d/MMM/yyyy (ddd)\nhh:mm:ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    actualizarTiempo();
}

void VentanaPrincipal::on_checarButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(checar);
    ui->checarButton->setEnabled(false);
    ui->personalButton->setEnabled(true);
    ui->historialButton->setEnabled(true);
    ui->opcionesButton->setEnabled(true);
}

void VentanaPrincipal::on_personalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(personal);
    ui->checarButton->setEnabled(true);
    ui->personalButton->setEnabled(false);
    ui->historialButton->setEnabled(true);
    ui->opcionesButton->setEnabled(true);
}

void VentanaPrincipal::on_historialButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(historial);
    ui->checarButton->setEnabled(true);
    ui->personalButton->setEnabled(true);
    ui->historialButton->setEnabled(false);
    ui->opcionesButton->setEnabled(true);
}

void VentanaPrincipal::on_opcionesButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(opciones);
    ui->checarButton->setEnabled(true);
    ui->personalButton->setEnabled(true);
    ui->historialButton->setEnabled(true);
    ui->opcionesButton->setEnabled(false);

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

