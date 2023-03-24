#include "opciones.h"
#include "ui_opciones.h"
#include "QDebug"
#include "login.h"
#include "ventanaprincipal.h"

#include <QFile>
#include <QMessageBox>
#include <dbmanager.h>

Opciones::Opciones(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Opciones)
{
    ui->setupUi(this);

    QString archivoBase = opciones.value("archivoBase").toString();
    ui->nombreArchivoLabel->setText(archivoBase);

    QString contrataciones = opciones.value("contrataciones").toString();
    ui->contratacionesEdit->setPlainText(contrataciones);

    int minutosTolerancia = opciones.value("tolerancia").toInt();
    ui->toleranciaSbox->setValue(minutosTolerancia);

    int tema = opciones.value("tema").toInt();
    if (tema == 0) ui->claroButton->setChecked(true);
    else ui->oscuroButton->setChecked(true);

    actualizarTema();
    connect(ui->claroButton, &QRadioButton::clicked, this, &Opciones::establecerTemaClaro);
    connect(ui->oscuroButton, &QRadioButton::clicked, this, &Opciones::establecerTemaOscuro);
}

Opciones::~Opciones()
{
    delete ui;
}

void Opciones::on_seleccionarArchivoButton_clicked()
{
    QString archivo = DbManager::conseguirArchivo();
    if (archivo.isEmpty()) return;
    opciones.setValue("archivoBase", archivo);
    ui->nombreArchivoLabel->setText(archivo);
}

void Opciones::on_contratacionesEdit_textChanged()
{
    ui->guardarButton->setEnabled(true);
}

void Opciones::on_guardarButton_clicked()
{
    QString contrataciones = ui->contratacionesEdit->toPlainText();
    opciones.setValue("contrataciones", contrataciones);

    int minutosTolerancia = ui->toleranciaSbox->value();
    opciones.setValue("tolerancia", minutosTolerancia);
    ui->guardarButton->setEnabled(false);
}


void Opciones::on_toleranciaSbox_valueChanged(int valor)
{
    Q_UNUSED(valor);
    ui->guardarButton->setEnabled(true);
}

void Opciones::establecerTemaClaro()
{
    opciones.setValue("tema", 0);
    actualizarTema();
}

void Opciones::establecerTemaOscuro()
{
    opciones.setValue("tema", 1);
    actualizarTema();
}

void Opciones::actualizarTema()
{
    int tema = opciones.value("tema").toInt();
    QByteArray qss;
    QFile base(":/stylesheets/stylesheet.qss");
    base.open(QFile::ReadOnly);
    if (tema == 0)
    {
        qss += base.readAll();
    }
    else
    {
        QFile dark(":/stylesheets/dark.qss");
        dark.open(QFile::ReadOnly);
        qss += dark.readAll() + base.readAll();
    }
    qApp->setStyleSheet(qss);
    emit temaActualizado(tema);
}

void Opciones::on_pantallaButton_clicked()
{
    int indice = ui->stackedWidget->currentIndex();
    if (indice == 0)
    {
        ui->stackedWidget->setCurrentIndex(1);
        ui->pantallaButton->setText("Regresar");
        ui->guardarButton->setVisible(false);
        ui->listView->setModel(DbManager::conseguirUsuarios());
    }
    else
    {
        ui->stackedWidget->setCurrentIndex(0);
        ui->pantallaButton->setText("Administrar Usuarios");
        ui->guardarButton->setVisible(true);
    }
}

void Opciones::on_listView_clicked(const QModelIndex &index)
{
    idUsuario = ui->listView->model()->index(index.row(), 1).data().toInt();
    privilegiosUsuario = ui->listView->model()->index(index.row(), 2).data().toInt();
    QString nombre = index.data().toString();
    ui->usuarioLabel->setText(nombre);
    ui->editarUsuarioButton->setEnabled(true);
    ui->eliminarUsuarioButton->setEnabled(true);
}

void Opciones::on_editarUsuarioButton_clicked()
{
    QString nombre = ui->usuarioLabel->text();
    Login login;
    connect(&login, &Login::usuarioActualizado, this, &Opciones::cambiosUsuarios);
    int resultado = login.editarUsuario(idUsuario, nombre, privilegiosUsuario);
    if (resultado == Login::Accepted)
    {
        cambiosUsuarios();
    }
}

void Opciones::cambiosUsuarios()
{
    ui->listView->setModel(DbManager::conseguirUsuarios());
    ui->usuarioLabel->setText("");
}

void Opciones::on_eliminarUsuarioButton_clicked()
{
    int eliminar = QMessageBox::question(nullptr, VentanaPrincipal::titulo,
                                         "¿Eliminar permanentemente a este usuario?");
    if (eliminar == QMessageBox::Yes)
    {
        DbManager::eliminarUsuario(idUsuario);
        cambiosUsuarios();
    }
}

void Opciones::on_nuevoUsuarioButton_clicked()
{
    Login login;
    int resultado = login.registrarUsuario();
    if (resultado == Login::Accepted)
    {
        cambiosUsuarios();
    }
}

