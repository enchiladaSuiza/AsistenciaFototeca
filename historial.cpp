#include "historial.h"
#include "ui_historial.h"

#include <dbmanager.h>
#include <QSqlQuery>
#include <QFileDialog>
#include <QMimeData>
#include <QSettings>

Historial::Historial(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Historial)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setMinimumWidth(80);
    ui->tablaTotales->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    ui->tablaTotales->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablaTotales->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tablaTotales->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

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

    actualizarComboBox();
    connect(ui->buttonGroup, &QButtonGroup::buttonClicked, this, &Historial::actualizarConsulta);
    connect(ui->yearPicker, &QSpinBox::valueChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->monthPicker, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->weekPicker, &QSpinBox::valueChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->empleadosCbox, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->rangoInicio, &QDateEdit::dateChanged, this, &Historial::actualizarConsultaSlotDate);
    connect(ui->rangoInicio, &QDateEdit::dateChanged, this, &Historial::actualizarConsultaSlotDate);
}

Historial::~Historial() { delete ui; }

void Historial::actualizarComboBox()
{
    QSqlQuery nombres = DbManager::idNombresCompletoEmpleados();
    indicesIds.clear();
    indicesIds.append(-1);
    ui->empleadosCbox->clear();
    ui->empleadosCbox->addItem("- Todos -");
    while (nombres.next())
    {
        int id = nombres.value("id").toInt();
        indicesIds.append(id);
        QString nombre = nombres.value("nombre_completo").toString();
        ui->empleadosCbox->addItem(nombre);
    }
}

void Historial::colocarHeadersDias(QDate inicio, QDate fin, bool colocarTotales)
{
    int columnas = 0;
    QStringList labels;
    while (inicio <= fin)
    {
        QString fechaString = QLocale().toString(inicio, "d/MMM/yyyy\n(ddd)");
        labels << fechaString;
        inicio = inicio.addDays(1);
        columnas++;
    }

    if (colocarTotales)
    {
        columnas += 3;
        labels << "Demora" << "Anticipación" << "Faltas";
    }
    ui->tableWidget->setColumnCount(columnas);
    ui->tableWidget->setHorizontalHeaderLabels(labels);

}

void Historial::consultaTodos(QDate inicio, QDate fin)
{
    ui->tableWidget->clear();
    colocarHeadersDias(inicio, fin, true);

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tablaTotales->setVisible(false);

    QDate d;
    int demora, anticipacion, faltas, columna, fila = 0;
    for (int i = 1; i < indicesIds.size(); i++)
    {
        d = inicio;
        demora = anticipacion = faltas = columna = 0;

        ui->tableWidget->setRowCount(fila + 3);
        QString nombre = ui->empleadosCbox->itemText(i);
        insertarItemTabla(nombre, fila, 0, 1, ui->tableWidget->columnCount());

        QSqlQuery historial = DbManager::historialCompletoEmpleado(indicesIds[i], inicio, fin);
        historial.next();
        while (d <= fin)
        {
            if (!historial.isValid() ||
                    historial.value("fecha").toString() != d.toString(Qt::ISODate))
            {
                if (DbManager::horarioEntradaDia(indicesIds[i], d).isEmpty())
                {
                    insertarItemTabla("N/A", fila + 1, columna, 2, 1);
                }
                else
                {
                    faltas++;
                    insertarItemTabla(":(", fila + 1, columna, 2, 1);
                }
            }
            else
            {
                QSqlRecord registro = historial.record();
                QPair<int, int> diferencias = colocarStringsCaptura(registro, fila + 1, columna);
                demora += diferencias.first;
                anticipacion += diferencias.second;
                historial.next();
            }
            columna++;
            d = d.addDays(1);
        }
        QStringList totales = conseguirTotales(demora, anticipacion, faltas);
        insertarItemTabla(totales.at(0), fila + 1, columna, 2, 1);
        insertarItemTabla(totales.at(1), fila + 1, columna + 1, 2, 1);
        insertarItemTabla(totales.at(2), fila + 1, columna + 2, 2, 1);
        fila += 3;
    }
}

void Historial::consultarRango(QDate inicio, QDate fin)
{
    ui->tableWidget->clear();
    colocarHeadersDias(inicio, fin);

    ui->tableWidget->setRowCount(2);
    ui->tablaTotales->setVisible(true);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    int indice = ui->empleadosCbox->currentIndex();
    int idEmpleado = indicesIds.at(indice);

    int totalDemora = 0;
    int totalAnticipacion = 0;
    int faltas = 0;
    int indiceColumna = 0;

    QSqlQuery historial = DbManager::historialCompletoEmpleado(idEmpleado, inicio, fin);
    historial.next();
    while (inicio <= fin)
    {
        if (!historial.isValid() ||
                historial.value("fecha").toString() != inicio.toString(Qt::ISODate))
        {
            if (DbManager::horarioEntradaDia(idEmpleado, inicio).isEmpty())
            {
                insertarItemTabla("N/A", 0, indiceColumna, 2);
            }
            else
            {
                faltas++;
                insertarItemTabla(":(", 0, indiceColumna, 2);
            }
        }
        else
        {
            QSqlRecord registro = historial.record();
            QPair<int, int> diferencias = colocarStringsCaptura(registro, 0, indiceColumna);
            totalDemora += diferencias.first;
            totalAnticipacion += diferencias.second;
            historial.next();
        }
        indiceColumna++;
        inicio = inicio.addDays(1);
    }
    colocarTotales(totalDemora, totalAnticipacion, faltas);
}

QPair<int, int> Historial::colocarStringsCaptura(QSqlRecord registro, int fila, int columna)
{
    QString entradaNormal = registro.value("e_normal").toString();
    QString horaEntrada = registro.value("hora_entrada").toString();
    int diferenciaEntrada = registro.value("dif_e").toInt();
    QString difEntradaString = QString::number(diferenciaEntrada);
    QString salidaNormal = registro.value("s_normal").toString();
    QString horaSalida = registro.value("hora_salida").toString();
    int diferenciaSalida = registro.value("dif_s").toInt();
    QString difSalidaString = QString::number(diferenciaSalida);

    if (diferenciaEntrada < 1) difEntradaString = ":)";

    if (horaSalida.isEmpty())
    {
        horaSalida = "-";
        difSalidaString = "-";
    }
    else
    {
        if (diferenciaSalida < 1) difSalidaString = ":)";
    }

    insertarItemTabla(entradaNormal + "\n" + horaEntrada + "\n" + difEntradaString, fila, columna);
    insertarItemTabla(salidaNormal + "\n" + horaSalida + "\n" + difSalidaString, fila + 1, columna);

    return qMakePair(diferenciaEntrada, diferenciaSalida);
}

QStringList Historial::conseguirTotales(int demora, int anticipacion, int faltas)
{
    QStringList totales;
    QString demoraString, anticipacionString, faltasString;

    if (ui->buttonGroup->checkedButton() == ui->weekSelect)
    {
        QSettings configuracion;
        int tolerancia = configuracion.value("tolerancia").toInt();
        demoraString = QString::number(demora) + " - " + QString::number(tolerancia) + " = ";

        int diferencia = demora - tolerancia;
        if (diferencia > 0) demoraString += QString::number(diferencia);
        else demoraString += ":)";
    }
    else
    {
        if (demora > 0) demoraString = QString::number(demora);
        else demoraString = ":)";
    }

    if (anticipacion > 0) anticipacionString = QString::number(anticipacion);
    else anticipacionString = ":)";

    if (faltas > 0) faltasString = QString::number(faltas);
    else faltasString = ":)";

    totales << demoraString << anticipacionString << faltasString;
    return totales;
}

void Historial::colocarTotales(int demora, int anticipacion, int faltas)
{
    QStringList totales = conseguirTotales(demora, anticipacion, faltas);
    ui->tablaTotales->setItem(0, 0, crearItemTabla(totales.at(0)));
    ui->tablaTotales->setItem(0, 1, crearItemTabla(totales.at(1)));
    ui->tablaTotales->setItem(0, 2, crearItemTabla(totales.at(2)));
}

void Historial::actualizarConsultaSlotDate(const QDate &dia)
{
    Q_UNUSED(dia);
    actualizarConsulta();
}

void Historial::actualizarConsultaSlotInt(int slotInt)
{
    Q_UNUSED(slotInt);
    actualizarConsulta();
}

void Historial::actualizarConsulta()
{
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
        fin = inicio;
        nombreReporte += inicio.toString(Qt::ISODate);
    }
    else if (seleccionado == ui->yearSelect)
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

    if (fin > QDate::currentDate()) fin = QDate::currentDate();

    if (ui->empleadosCbox->currentIndex() == 0) consultaTodos(inicio, fin);
    else consultarRango(inicio, fin);
}

int Historial::numeroDeSemana(QDate dia)
{
    int cantidadLunes = 0;
    while (dia.day() > 1)
    {
        if (dia.dayOfWeek() == 1) cantidadLunes++;
        dia = dia.addDays(-1);
    }
    cantidadLunes++;
    return cantidadLunes;
}

QTableWidgetItem* Historial::crearItemTabla(QString texto)
{
    QTableWidgetItem* item = new QTableWidgetItem(texto);
    item->setTextAlignment(Qt::AlignCenter);
    return item;
}

void Historial::insertarItemTabla(QString texto, int fila, int columna, int filaSpan, int columnaSpan)
{
    QTableWidgetItem* item = crearItemTabla(texto);
    ui->tableWidget->setItem(fila, columna, item);
    if (ui->tableWidget->rowSpan(fila, columna) == filaSpan
            && ui->tableWidget->columnSpan(fila, columna) == columnaSpan) return;
    ui->tableWidget->setSpan(fila, columna, filaSpan, columnaSpan);
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

void Historial::on_cboxAtrasButton_clicked()
{
    int indice = ui->empleadosCbox->currentIndex();
    if (indice > 0) ui->empleadosCbox->setCurrentIndex(indice - 1);
}

void Historial::on_cboxAdelanteButton_clicked()
{
    int indice = ui->empleadosCbox->currentIndex();
    if (indice < ui->empleadosCbox->count() - 1) ui->empleadosCbox->setCurrentIndex(indice + 1);
}
