#include "ventanaprincipal.h"
#include "ui_ventanaprincipal.h"
#include "login.h"

#include <QShortcut>
#include <QTime>
#include <QPixmap>
#include <QSvgWidget>
#include <QSvgRenderer>
#include <QMessageBox>

VentanaPrincipal::VentanaPrincipal(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::VentanaPrincipal)
{
    ui->setupUi(this);

    new QShortcut(Qt::CTRL | Qt::Key_Q, this, SLOT(close()));
    QLocale::setDefault(QLocale(QLocale::Spanish, QLocale::Mexico));

    instancia = this;

    opciones = new Opciones(this);
    checar = new Checar(this);
    personal = new Personal(this);
    historial = new Historial(this);

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

    ui->stackedWidget->addWidget(checar);
    ui->stackedWidget->addWidget(personal);
    ui->stackedWidget->addWidget(historial);
    ui->stackedWidget->addWidget(opciones);
    ui->checarButton->setEnabled(false);
    ui->usuarioLabel->setVisible(false);
    ui->logoutButton->setVisible(false);
    ui->vline->setVisible(false);

    ahora = QDateTime::currentDateTime();
    startTimer(1000);
    actualizarTiempo();

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

const VentanaPrincipal* VentanaPrincipal::conseguirInstancia()
{
    return instancia;
}

void VentanaPrincipal::colocarNombre(QString usuario)
{
    ui->usuarioLabel->setVisible(true);
    ui->usuarioLabel->setText(usuario);
    ui->logoutButton->setVisible(true);
    ui->vline->setVisible(true);
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
    Login login;
    if (login.registrarAdmin() == QDialog::Rejected ||
            login.iniciarSesion() == QDialog::Rejected) return;
    if (Login::accesoPersonal())
    {
        ui->stackedWidget->setCurrentWidget(personal);
        activarTodosLosBotones();
        ui->personalButton->setEnabled(false);
    }
    else mostrarMensajeSinPermiso();
}

void VentanaPrincipal::on_historialButton_clicked()
{
    Login login;
    if (login.registrarAdmin() == QDialog::Rejected ||
            login.iniciarSesion() == QDialog::Rejected) return;
    if (Login::accesoHistorial())
    {
        historial->actualizarConsulta();
        ui->stackedWidget->setCurrentWidget(historial);
        activarTodosLosBotones();
        ui->historialButton->setEnabled(false);
    }
    else mostrarMensajeSinPermiso();
}

void VentanaPrincipal::on_opcionesButton_clicked()
{
    Login login;
    if (login.registrarAdmin() == QDialog::Rejected ||
            login.iniciarSesion() == QDialog::Rejected) return;
    if (Login::accesoOpciones())
    {
        ui->stackedWidget->setCurrentWidget(opciones);
        activarTodosLosBotones();
        ui->opcionesButton->setEnabled(false);
    }
    else mostrarMensajeSinPermiso();
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

void VentanaPrincipal::mostrarMensajeSinPermiso()
{
    QMessageBox::information(nullptr, titulo, "Su usuario no tiene permiso para acceder a esta pantalla.");
}


void VentanaPrincipal::on_logoutButton_clicked()
{
    Login::cerrarSesion();
    ui->usuarioLabel->setVisible(false);
    ui->logoutButton->setVisible(false);
    ui->vline->setVisible(false);
    on_checarButton_clicked();
}

