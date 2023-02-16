#include "historial.h"
#include "ui_historial.h"

#include <dbmanager.h>
#include <QSqlQuery>
#include <QFileDialog>
#include <QMimeData>

Historial::Historial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Historial)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tableWidget->horizontalHeader()->setMinimumHeight(36);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Interactive);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

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
    if (!capturas.next()) return;

    int fila = ui->tableWidget->rowCount();
    ui->tableWidget->insertRow(fila);

    QTableWidgetItem *filaDia = new QTableWidgetItem(QLocale().toString(dia, "d/MMM/yyyy (ddd)"));
    filaDia->setTextAlignment(Qt::AlignCenter);
    ui->tableWidget->setItem(fila, 0, filaDia);
    ui->tableWidget->setSpan(fila, 0, 1, 8);

    do
    {  
        QTableWidgetItem *id = crearItemTabla(capturas.value(0).toString());
        QTableWidgetItem *nombre = crearItemTabla(capturas.value(1).toString());
        QTableWidgetItem *entradaNormal = crearItemTabla(capturas.value(2).toTime().toString("h:mm"));
        QTableWidgetItem *entradaCaptura = crearItemTabla(capturas.value(3).toTime().toString("h:mm"));

        int desfaseEntrada = capturas.value(4).toInt();
        QTableWidgetItem *diferenciaEntrada;
        if (desfaseEntrada < 1) diferenciaEntrada = crearItemTabla("✔️");
        else diferenciaEntrada = crearItemTabla(QString::number(desfaseEntrada));

        QTableWidgetItem *salidaNormal = crearItemTabla(capturas.value(5).toTime().toString("h:mm"));
        QTableWidgetItem *salidaCaptura = nullptr;
        QTableWidgetItem *diferenciaSalida = nullptr;

        QString tiempoCapturaSalida = capturas.value(6).toTime().toString("h:mm");
        int desfaseSalida = capturas.value(7).toInt();
        if (tiempoCapturaSalida.isEmpty())
        {
            salidaCaptura = crearItemTabla("-");
            diferenciaSalida = crearItemTabla("-");
        }
        else
        {
            salidaCaptura = crearItemTabla(tiempoCapturaSalida);
            if (desfaseSalida < 1) diferenciaSalida = crearItemTabla("✔️");
            else diferenciaSalida = crearItemTabla(QString::number(desfaseSalida));
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

void Historial::consultarRango(QDate inicio, QDate fin)
{
    rangoInicio = inicio.toString(Qt::ISODate);
    rangoFin = fin.toString(Qt::ISODate);
    while (inicio <= fin)
    {
        consultarDia(inicio);
        inicio = inicio.addDays(1);
    }
}

void Historial::actualizarConsulta(QAbstractButton* seleccionado)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    if (seleccionado == nullptr) seleccionado = ui->buttonGroup->checkedButton();

    if (seleccionado == ui->yearSelect)
    {
        int year = ui->yearPicker->value();
        consultarRango(QDate(year, 1, 1), QDate(year, 12, 31));
    }
    else if (seleccionado == ui->monthSelect)
    {
        int year = ui->yearPicker->value();
        int month = ui->monthPicker->currentIndex() + 1;
        QDate inicio(year, month, 1);
        consultarRango(inicio, inicio.addMonths(1));
    }
    else if (seleccionado == ui->weekSelect)
    {
        int year = ui->yearPicker->value();
        int semana = ui->weekPicker->value() - 1;
        QDate inicio(year, 1, 1);
        while (inicio.dayOfWeek() != 1) // ¿Primer lunes del año?
        {
            inicio = inicio.addDays(1);
        }
        consultarRango(inicio, inicio.addDays(7 * semana));
    }
    else if (seleccionado == ui->daySelect)
    {
        consultarRango(ui->rangoFin->date(), ui->rangoFin->date());
    }
    else
    {
        consultarRango(ui->rangoInicio->date(), ui->rangoFin->date());
    }
}

void Historial::seleccionCambiada(QAbstractButton *boton) { actualizarConsulta(boton); }

QTableWidgetItem* Historial::crearItemTabla(QString texto)
{
    QTableWidgetItem* item = new QTableWidgetItem(texto);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
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

void Historial::on_exportarButton_clicked()
{
    QFileDialog dialogo;
    QString nombreArchivo = "Reporte" + rangoInicio + "---" + rangoFin + ".csv";
    QString directorio = dialogo.getSaveFileName(this, "Elija un directorio.", "./" + nombreArchivo);
    QFile archivo(directorio);
    if (archivo.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream salida(&archivo);
        salida << "No. Empleado,Nombre,Entrada,Captura de Entrada,Diferencia de Entrada,"
               << "Salida,Captura de Salida, Diferencia de Salida\n";
        int filas = ui->tableWidget->rowCount();
        for (int i = 0; i < filas; i++)
        {
            for (int j = 0; j < 8; j++)
            {
                QTableWidgetItem *item = ui->tableWidget->item(i, j);
                if (item == nullptr) continue;
                salida << item->text() << ",";
            }
            salida << '\n';
        }
        archivo.close();
    }
}

