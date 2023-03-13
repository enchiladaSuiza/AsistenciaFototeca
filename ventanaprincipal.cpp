#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"
#include "dbmanager.h"

#include <QShortcut>
#include <QTime>
#include <QPixmap>
#include <QSvgWidget>
#include <QSvgRenderer>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    new QShortcut(Qt::CTRL | Qt::Key_Q, this, SLOT(close()));

    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Mexico));

    ahora = QDateTime::currentDateTime();
    startTimer(1000);
    actualizarTiempo();
//    if (!DbManager::hayRegistros(ahora.date()))
//    {
//        DbManager::insertarRegistrosVacios(ahora.date());
//    }

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
    QDateTime nuevoAhora = QDateTime::currentDateTime();
//    if (ahora.date().day() != nuevoAhora.date().day() && !DbManager::hayRegistros(nuevoAhora.date()))
//    {
//        DbManager::insertarRegistrosVacios(nuevoAhora.date());
//    }
    ahora = nuevoAhora;
    ui->horaLabel->setText(QLocale().toString(ahora, "d/MMM/yyyy (ddd)\nhh:mm:ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    actualizarTiempo();
}

void VentanaPrincipal::activarTodosLosBotones()
{
    ui->checarButton->setEnabled(true);
    ui->personalButton->setEnabled(true);
    ui->historialButton->setEnabled(true);
    ui->opcionesButton->setEnabled(true);
}

void VentanaPrincipal::on_checarButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(checar);
    activarTodosLosBotones();
    ui->checarButton->setEnabled(false);
}

void VentanaPrincipal::on_personalButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(personal);
    activarTodosLosBotones();
    ui->personalButton->setEnabled(false);
}

void VentanaPrincipal::on_historialButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(historial);
    activarTodosLosBotones();
    ui->historialButton->setEnabled(false);
}

void VentanaPrincipal::on_opcionesButton_clicked()
{
    ui->stackedWidget->setCurrentWidget(opciones);
    activarTodosLosBotones();
    ui->opcionesButton->setEnabled(false);
}

void VentanaPrincipal::on_stackedWidget_currentChanged(int actual)
{
    switch (actual)
    {
    case 0:
        checar->activarCamara();
        personal->desactivarCamaraQR();
        break;
    case 1:
        checar->desactivarCamara();
        personal->activarCamaraQR();
        break;
    case 2:
        historial->actualizarConsulta();
        checar->desactivarCamara();
        personal->desactivarCamaraQR();
        break;
    case 3:
        checar->desactivarCamara();
        personal->desactivarCamaraQR();
        break;
    }
}

