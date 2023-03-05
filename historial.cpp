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
    ui->weekPicker->setValue(numeroDeSemana(hoy));
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

void Historial::on_fortnightSelect_clicked()
{
    on_monthSelect_clicked();
    ui->weekPicker->setMaximum(2);
    ui->weekPicker->setVisible(true);
}

void Historial::on_weekSelect_clicked()
{
    on_monthSelect_clicked();
    ui->weekPicker->setMaximum(6);
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

int Historial::numeroDeSemana(QDate dia)
{
    int cantidadLunes = 0;
    while (dia.day() < 1)
    {
        if (dia.dayOfWeek() == 1) cantidadLunes++;
        dia = dia.addDays(-1);
    }
    cantidadLunes++;
    return cantidadLunes;
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
        if (desfaseEntrada < 1) diferenciaEntrada = crearItemTabla("✓");
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
            if (desfaseSalida < 1) diferenciaSalida = crearItemTabla("✓");
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
    while (inicio <= fin)
    {
        consultarDia(inicio);
        inicio = inicio.addDays(1);
    }
}

void Historial::actualizarConsulta()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    QAbstractButton *seleccionado = ui->buttonGroup->checkedButton();

    nombreReporte = "Reporte";
    int year = ui->yearPicker->value();
    int month = ui->monthPicker->currentIndex() + 1;
    int semana = ui->weekPicker->value();
    int quincena = semana;
    QDate inicio, fin;

    if (seleccionado == ui->daySelect)
    {
        inicio = ui->rangoFin->date();
        consultarDia(inicio);
        nombreReporte += inicio.toString(Qt::ISODate);
        return;
    }
    if (seleccionado == ui->yearSelect)
    {
        inicio = QDate(year, 1, 1);
        fin = QDate(year, 12, 31);
        nombreReporte += QString::number(year);
    }
    else if (seleccionado == ui->monthSelect)
    {
        inicio = QDate(year, month, 1);
        fin = inicio.addMonths(1).addDays(-1);
        nombreReporte += ui->monthPicker->currentText() + QString::number(year);
    }
    else if (seleccionado == ui->weekSelect)
    {
        inicio = QDate(year, month, 1);
        while (inicio.dayOfWeek() != 1) inicio = inicio.addDays(-1);
        inicio = inicio.addDays(7 * (semana - 1));
        fin = inicio.addDays(6);
        nombreReporte += ui->monthPicker->currentText() + QString::number(year) + "-Semana" + QString::number(semana);
    }
    else if (seleccionado == ui->fortnightSelect)
    {
        if (quincena == 1)
        {
            inicio = QDate(year, month, 1);
            fin = QDate(year, month, 15);
        }
        else
        {
            inicio = QDate(year, month, 16);
            fin = QDate(year, month, 1).addMonths(1).addDays(-1);
        }
        nombreReporte += ui->monthPicker->currentText() + QString::number(year) + "-Quincena" + QString::number(semana);
    }
    else
    {
        inicio = ui->rangoInicio->date();
        fin = ui->rangoFin->date();
        nombreReporte += inicio.toString(Qt::ISODate) + "---" + fin.toString(Qt::ISODate);
    }
    consultarRango(inicio, fin);
}

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
    QString directorio = QFileDialog::getSaveFileName(this, "Elija un directorio.", "./" +
                                                      nombreReporte + ".csv", "Comma Separated Values (*.csv)");
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


