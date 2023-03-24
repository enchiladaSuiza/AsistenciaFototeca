#include "login.h"
#include "ui_login.h"
#include "dbmanager.h"
#include "ventanaprincipal.h"

#include <QMessageBox>
#include <bitset>

Login::Login(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);

    setWindowTitle("Sistema de Control de Asistencia de la Fototeca Nacional");
    connect(this, &Login::usuarioRegistrado, VentanaPrincipal::conseguirInstancia(), &VentanaPrincipal::colocarNombre);
}

Login::~Login()
{
    delete ui;
}

int Login::registrarAdmin()
{
    if (!hayAdmins) hayAdmins = DbManager::hayAdministradores();
    if (hayAdmins) return QDialog::Accepted;
    tipoDialogo = 0;
    ui->infoLabel->setText("Se requiere crear un usuario para acceder a las pantallas de administración de este sistema.");
    ui->privilegiosLabel->setText("Este usuario deberá contar con todos los privilegios.");
    ui->personalCbox->setChecked(true);
    ui->personalCbox->setEnabled(false);
    ui->historialCbox->setChecked(true);
    ui->historialCbox->setEnabled(false);
    ui->opcionesCbox->setChecked(true);
    ui->opcionesCbox->setEnabled(false);
    ui->cancelarButton->setVisible(false);
    ui->ingresarButton->setText("Crear");
    return this->exec();
}

int Login::registrarUsuario()
{
    tipoDialogo = 0;
    ui->infoLabel->setText("Ingrese los datos del nuevo usuario.");
    ui->privilegiosLabel->setText("Privilegios.");
    ui->ingresarButton->setText("Crear");
    return this->exec();
}

int Login::iniciarSesion()
{
    if (usuarioId != -1) return QDialog::Accepted;
    tipoDialogo = 1;
    ui->infoLabel->setText("Por favor inicie sesión con algún usuario registrado.");
    ui->privilegiosLabel->setVisible(false);
    ui->personalCbox->setVisible(false);
    ui->historialCbox->setVisible(false);
    ui->opcionesCbox->setVisible(false);
    return this->exec();
}

int Login::editarUsuario(int id, QString nombre, int privilegios)
{
    tipoDialogo = 2;
    idUsuario = id;
    std::bitset<3> bs(privilegios);
    ui->infoLabel->setText("Actualice los datos de este usuario.");
    ui->privilegiosLabel->setText("Privilegios.");
    ui->usuarioEdit->setText(nombre);
    ui->ingresarButton->setText("Actualizar");
    ui->personalCbox->setChecked(!bs.test(2));
    ui->historialCbox->setChecked(!bs.test(1));
    ui->opcionesCbox->setChecked(!bs.test(0));
    return this->exec();
}

bool Login::accesoPersonal()
{
    return conseguirPrivilegio(2);
}

bool Login::accesoHistorial()
{
    return conseguirPrivilegio(1);
}

bool Login::accesoOpciones()
{
    return conseguirPrivilegio(0);
}

bool Login::conseguirPrivilegio(int posicion)
{
    Login login;
    int privs = DbManager::conseguirPrivilegios(usuarioId);
    std::bitset<3> bs(privs);
    return !bs.test(posicion);
}

void Login::cerrarSesion()
{
    usuarioId = -1;
}

void Login::on_cancelarButton_clicked()
{
    reject();
}

void Login::on_ingresarButton_clicked()
{
    QString usuario = ui->usuarioEdit->text();
    QString pass = ui->contraEdit->text();
    std::bitset<3> privilegios;
    privilegios.set(0, !ui->opcionesCbox->isChecked());
    privilegios.set(1, !ui->historialCbox->isChecked());
    privilegios.set(2, !ui->personalCbox->isChecked());
    int codigoPrivs = (int)privilegios.to_ulong();

    switch (tipoDialogo)
    {
    case 0:
        if (DbManager::agregarUsuario(usuario, pass, codigoPrivs))
        {
            usuarioId = DbManager::ultimoId();
            // QMessageBox::information(nullptr, VentanaPrincipal::titulo, "Usuario creado.");
            emit usuarioRegistrado(usuario);
            accept();
        }
        else reject();
        break;
    case 1:
        usuarioId = DbManager::verificarCredenciales(usuario, pass);
        if (usuarioId != -1)
        {
            emit usuarioRegistrado(usuario);
            accept();
        }
        else
        {
            QMessageBox::critical(nullptr, VentanaPrincipal::titulo,
                                     "El nombre de usuario o contraseña ingresados son incorrectos.");
            reject();
        }
        break;
    case 2:
        if (DbManager::actualizarUsuario(idUsuario, usuario, pass, codigoPrivs))
        {
            emit usuarioActualizado();
            accept();
        }
        else
        {
            QMessageBox::critical(nullptr, VentanaPrincipal::titulo,
                                     "El usuario no pudo ser actualizado.");
            reject();
        }
        break;
    default:
        break;
    }
}

void Login::on_mostrarButton_clicked()
{
    if (ui->contraEdit->echoMode() == QLineEdit::Normal)
    {
        ui->contraEdit->setEchoMode(QLineEdit::Password);
        ui->mostrarButton->setText("Ocultar");
    }
    else
    {
        ui->contraEdit->setEchoMode(QLineEdit::Normal);
        ui->mostrarButton->setText("Mostrar");
    }
}

