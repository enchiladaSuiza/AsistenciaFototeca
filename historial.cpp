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

    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, &Historial::actualizarConsulta);
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
    on_yearSelect_clicked();
    ui->weekPicker->setVisible(true);
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

void Historial::consultarDia(QDate dia)
{
    QSqlQuery capturas = DbManager::capturasDeUnDia(dia);
    if (!capturas.next())
    {
        return;
    }

    int fila = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(fila);

    QTableWidgetItem *filaDia = new QTableWidgetItem(QLocale().toString(dia, "d/MMM/yyyy (ddd)"));
    filaDia->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(fila, 0, filaDia);
    ui->tableWidget->setSpan(fila, 0, 1, 8);

    do
    {
        QTableWidgetItem *id = new QTableWidgetItem(capturas.value(0).toString());
        id->setTextAlignment(Qt::AlignCenter);
        QTableWidgetItem *nombre = new QTableWidgetItem(capturas.value(1).toString());
        QTableWidgetItem *entradaNormal = new QTableWidgetItem(capturas.value(2).toTime().toString("h:mm"));
        QTableWidgetItem *entradaCaptura = new QTableWidgetItem(capturas.value(3).toTime().toString("h:mm"));
        QTableWidgetItem *diferenciaEntrada;
        int desfaseEntrada = capturas.value(4).toInt();
        if (desfaseEntrada < 1)
        {
            diferenciaEntrada = new QTableWidgetItem("✔️");
        }
        else
        {
            diferenciaEntrada = new QTableWidgetItem(QString::number(desfaseEntrada));
        }

        QTableWidgetItem *salidaNormal = new QTableWidgetItem(capturas.value(5).toTime().toString("h:mm"));
        QTableWidgetItem *salidaCaptura = nullptr;
        QTableWidgetItem *diferenciaSalida = nullptr;
        QString tiempoCapturaSalida = capturas.value(6).toTime().toString("h:mm");
        int desfaseSalida = capturas.value(7).toInt();
        if (tiempoCapturaSalida.isEmpty())
        {
            salidaCaptura = new QTableWidgetItem("-");
            diferenciaSalida = new QTableWidgetItem("-");
        }
        else
        {
            salidaCaptura = new QTableWidgetItem(tiempoCapturaSalida);
            if (desfaseSalida < 1)
            {
                diferenciaSalida = new QTableWidgetItem("✔️");
            }
            else
            {
                diferenciaSalida = new QTableWidgetItem(QString::number(desfaseSalida));
            }
        }

        fila++;
        ui->tableWidget->insertRow(fila);
        ui->tableWidget->setItem(fila, 0, id);
        ui->tableWidget->setItem(fila, 1, nombre);
        ui->tableWidget->setItem(fila, 2, entradaNormal);
        ui->tableWidget->setItem(fila, 3, entradaCaptura);
        ui->tableWidget->setItem(fila, 4, diferenciaEntrada);
        ui->tableWidget->setItem(fila, 5, salidaNormal);
        ui->tableWidget->setItem(fila, 6, salidaCaptura);
        ui->tableWidget->setItem(fila, 7, diferenciaSalida);
        ui->tableWidget->resizeColumnToContents(1);
    } while (capturas.next());
}

void Historial::actualizarConsulta(QAbstractButton* seleccionado)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    if (seleccionado == nullptr)
    {
        seleccionado = ui->buttonGroup->checkedButton();
    }

    if (seleccionado == ui->yearSelect)
    {
        int year = ui->yearPicker->value();
        QDate dia(year, 1, 1);
        QDate fin(year, 12, 31);
        while (dia <= fin)
        {
            consultarDia(dia);
            dia = dia.addDays(1);
        }
    }
    else if (seleccionado == ui->monthSelect)
    {
        int year = ui->yearPicker->value();
        int month = ui->monthPicker->currentIndex() + 1;
        QDate dia(year, month, 1);
        QDate fin = dia.addMonths(1);
        while (dia < fin)
        {
            consultarDia(dia);
            dia = dia.addDays(1);
        }
    }
    else if (seleccionado == ui->weekSelect)
    {
        int year = ui->yearPicker->value();
        int semana = ui->weekPicker->value() - 1;
        QDate dia(year, 1, 1);
        while (dia.dayOfWeek() != 1) // ¿Primer lunes del año?
        {
            dia = dia.addDays(1);
        }
        dia = dia.addDays(7 * semana);
        for (int i = 0; i < 7; i++)
        {
            consultarDia(dia);
            dia = dia.addDays(1);
        }
    }
    else if (seleccionado == ui->daySelect)
    {
        consultarDia(ui->rangoFin->date());
    }
    else
    {
        QDate dia = ui->rangoInicio->date();
        QDate fin = ui->rangoFin->date();
        while (dia <= fin)
        {
            consultarDia(dia);
            dia = dia.addDays(1);
        }
    }
}

void Historial::seleccionCambiada(QAbstractButton *boton)
{
    actualizarConsulta(boton);
}

void Historial::on_rangoFin_dateChanged(const QDate &date)
{
    Q_UNUSED(date);
    actualizarConsulta();
}

void Historial::on_yearPicker_valueChanged(int year)
{
    Q_UNUSED(year);
    actualizarConsulta();
}

void Historial::on_monthPicker_currentIndexChanged(int index)
{
    Q_UNUSED(index)
    actualizarConsulta();
}

void Historial::on_rangoInicio_dateChanged(const QDate &date)
{
    Q_UNUSED(date);
    actualizarConsulta();
}

void Historial::on_weekPicker_valueChanged(int week)
{
    Q_UNUSED(week);
    actualizarConsulta();
}

