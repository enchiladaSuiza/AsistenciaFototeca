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
    connect(&datos, &Datos::registroSeleccionado, &escaner, &Escaner::setId);
    connect(&datos, &Datos::filaSeleccionada, &horario, &Horario::setFila);
    connect(&datos, &Datos::dbActualizada, &horario, &Horario::actualizarRegistros);
    connect(&datos, &Datos::nombreCompletoSeleccion, this, &Personal::actualizarRegistroSeleccionado);
}

Personal::~Personal() { delete ui; }

void Personal::actualizarRegistroSeleccionado(QString* nombre)
{
    if (nombre == nullptr)
    {
        ui->seleccionLabel->setStyleSheet("font-style: italic;");
        ui->seleccionLabel->setText("Registro válido después de guardar los cambios.");
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        return;
    }

    if (nombre->isEmpty())
    {
        ui->seleccionLabel->setStyleSheet("font-style: normal");
        ui->seleccionLabel->setText("Seleccione un registro.");
        ui->tabWidget->removeTab(2);
        ui->tabWidget->removeTab(1);
        return;
    }

    ui->seleccionLabel->setStyleSheet("font-weight: bold");
    ui->seleccionLabel->setText(*nombre);
    ui->tabWidget->addTab(&horario, "Horario");
    ui->tabWidget->addTab(&escaner, "Código QR");
}

void Personal::on_tabWidget_currentChanged(int index)
{
    if (index == 2) escaner.activarCamara();
    else escaner.desactivarCamara();
}

