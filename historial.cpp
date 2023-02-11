#include "historial.h"
#include "ui_historial.h"

#include <dbmanager.h>
#include <QSqlQuery>

Historial::Historial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Historial)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(36);
//    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableWidget->verticalHeader()->setMinimumWidth(15);

    QDate hoy = QDate::currentDate();
    ui->yearPicker->setValue(hoy.year());
    ui->monthPicker->setCurrentIndex(hoy.month() - 1);
    ui->weekPicker->setValue(hoy.weekNumber());
    ui->rangoInicio->setDate(hoy.addDays(-1));
    ui->rangoFin->setDate(hoy);

    ui->yearPicker->setVisible(false);
    ui->monthPicker->setVisible(false);
    ui->weekPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);

    consultarDia();
}

Historial::~Historial()
{
    delete ui;
}

void Historial::on_yearSelect_clicked()
{
    ui->yearPicker->setVisible(true);

    ui->monthPicker->setVisible(false);
    ui->weekPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);
    ui->rangoFin->setVisible(false);
}

void Historial::on_monthSelect_clicked()
{
    on_yearSelect_clicked();
    ui->monthPicker->setVisible(true);
}

void Historial::on_weekSelect_clicked()
{
    ui->weekPicker->setVisible(true);

    ui->yearPicker->setVisible(false);
    ui->monthPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);
    ui->rangoFin->setVisible(false);
}

void Historial::on_daySelect_clicked()
{
    ui->rangoFin->setVisible(true);

    ui->yearPicker->setVisible(false);
    ui->monthPicker->setVisible(false);
    ui->weekPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);
}

void Historial::on_rangeSelect_clicked()
{
    on_daySelect_clicked();
    ui->rangoInicio->setVisible(true);
}

void Historial::consultarDia()
{
    QSqlQuery capturas = DbManager::capturasDeUnDia(QDate::currentDate());
    while (capturas.next())
    {
        QTableWidgetItem *id = new QTableWidgetItem(capturas.value(0).toString());
        QTableWidgetItem *nombre = new QTableWidgetItem(capturas.value(1).toString());
        QTableWidgetItem *entradaNormal = new QTableWidgetItem(capturas.value(2).toString());
        QTableWidgetItem *entradaCaptura = new QTableWidgetItem(capturas.value(3).toString());
        // Diferencia
        QTableWidgetItem *salidaNormal = new QTableWidgetItem(capturas.value(4).toString());
        QTableWidgetItem *salidaCaptura = new QTableWidgetItem(capturas.value(5).toString());
        // Diferencia
        int fila = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila, 0, id);
        ui->tableWidget->setItem(fila, 1, nombre);
        ui->tableWidget->setItem(fila, 2, entradaNormal);
        ui->tableWidget->setItem(fila, 3, entradaCaptura);
        ui->tableWidget->setItem(fila, 5, salidaNormal);
        ui->tableWidget->setItem(fila, 6, salidaCaptura);
    }
}
