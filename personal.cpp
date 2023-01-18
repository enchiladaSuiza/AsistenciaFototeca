#include "personal.h"
#include "ui_personal.h"
#include <QSqlError>
#include <QSqlQuery>

Personal::Personal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Personal)
{
    ui->setupUi(this);

    ui->tabWidget->addTab(&datos, "Datos");
    connect(&datos, &Datos::registroSeleccionado, this, &Personal::setRegistro);
    connect(&datos, &Datos::filaSeleccionada, &horario, &Horario::setFila);
    connect(&datos, &Datos::dbActualizada, &horario, &Horario::actualizarRegistros);
}

Personal::~Personal()
{
    delete ui;
}

void Personal::setRegistro(int id)
{
    QSqlQuery query;
    query.prepare("SELECT nombre, apellido_paterno, apellido_materno FROM empleado WHERE id = :id");
    query.bindValue(":id", id);
    query.exec();
    if (query.next())
    {
        QString nombre = query.value(0).toString() + " " + query.value(1).toString() + " " + query.value(2).toString();
        ui->seleccionLabel->setText(nombre);
        ui->tabWidget->addTab(&horario, "Horario");
        registro = id;
    }
    else
    {
        ui->seleccionLabel->setText("Registro inválido");
        ui->tabWidget->removeTab(1);
        registro = -1;
    }
    // qDebug() << "En registro " << registro;
}

void Personal::on_tabWidget_currentChanged(int index)
{
    if (index == 1)
    {

    }
}

