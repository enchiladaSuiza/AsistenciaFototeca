#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"

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

    QSettings configuracion;
    int tema = configuracion.value("tema").toInt();

    cultura = new QSvgWidget(":/svg/CULTURA.svg");
    inah = new QSvgWidget(":/svg/INAH.svg");
    sinafo = new QSvgWidget(":/svg/SINAFO.svg");

    if (tema == 0)
    {
        inah = new QSvgWidget(":/svg/INAH.svg");
        sinafo = new QSvgWidget(":/svg/SINAFO.svg");
    }
    else
    {
        inah = new QSvgWidget(":/svg/INAH_b.svg");
        sinafo = new QSvgWidget(":/svg/SINAFO_b.svg");
    }

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

    connect(opciones, &Opciones::temaActualizado, this, &VentanaPrincipal::cambiarLogos);
}

VentanaPrincipal::~VentanaPrincipal() { delete ui; }

void VentanaPrincipal::actualizarTiempo()
{  
    QDateTime nuevoAhora = QDateTime::currentDateTime();
    ahora = nuevoAhora;
    ui->horaLabel->setText(QLocale().toString(ahora, "d/MMM/yyyy (ddd)\nhh:mm:ss"));
}

void VentanaPrincipal::timerEvent(QTimerEvent *event)
{
    Q_UNUSED(event);
    actualizarTiempo();
}

void VentanaPrincipal::cambiarLogos(int tema)
{
    QString svgInah;
    QString svgSinafo;
    if (tema == 0)
    {
        svgInah = ":/svg/INAH.svg";
        svgSinafo = ":/svg/SINAFO.svg";
    }
    else
    {
        svgInah = ":/svg/INAH_b.svg";
        svgSinafo = ":/svg/SINAFO_b.svg";
    }
    inah->load(svgInah);
    sinafo->load(svgSinafo);
    inah->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
    sinafo->renderer()->setAspectRatioMode(Qt::KeepAspectRatio);
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
    historial->actualizarConsulta();
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
        personal->desactivarCamaras();
        break;
    case 1:
        checar->desactivarCamara();
        personal->activarCamaras();
        break;
    default:
        checar->desactivarCamara();
        personal->desactivarCamaras();
        break;
    }
}

