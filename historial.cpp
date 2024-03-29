#include "historial.h"
#include "ui_historial.h"

#include <bitset>
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
    ui->tableWidget->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);

    QDate hoy = QDate::currentDate();
    ui->yearPicker->setValue(hoy.year());
    ui->monthPicker->setCurrentIndex(hoy.month() - 1);
    ui->weekPicker->setValue(numeroDeSemana(hoy));
    ui->fortnightPicker->setValue(numeroDeQuincena(hoy));
    ui->rangoInicio->setDate(hoy.addDays(-1));
    ui->rangoFin->setDate(hoy);

    connect(ui->cboxPeriodo, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->yearPicker, &QSpinBox::valueChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->monthPicker, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->weekPicker, &QSpinBox::valueChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->fortnightPicker, &QSpinBox::valueChanged, this, &Historial::actualizarConsultaSlotInt);
    connect(ui->rangoInicio, &QDateEdit::dateChanged, this, &Historial::actualizarConsultaSlotDate);
    connect(ui->rangoFin, &QDateEdit::dateChanged, this, &Historial::actualizarConsultaSlotDate);
    connect(ui->empleadosCbox, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    weekSeleccion();
}

Historial::~Historial() { delete ui; }

void Historial::actualizarComboBox()
{
    QSqlQuery nombres = DbManager::idNombresCompletosEmpleados();
    // int seleccion = ui->empleadosCbox->currentIndex();
    indicesIds.clear();
    indicesIds.append(-1);

    disconnect(ui->empleadosCbox, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
    ui->empleadosCbox->clear();
    ui->empleadosCbox->addItem("- Todos -");
    QStringList listaNombres;
    while (nombres.next())
    {
        int id = nombres.value("id").toInt();
        indicesIds.append(id);
        QString nombre = nombres.value("nombre_completo").toString();
        listaNombres << nombre;
    }
    ui->empleadosCbox->addItems(listaNombres);
    // ui->empleadosCbox->setCurrentIndex(seleccion);
    connect(ui->empleadosCbox, &QComboBox::currentIndexChanged, this, &Historial::actualizarConsultaSlotInt);
}

void Historial::colocarHeadersDias(QDate inicio, QDate fin, bool colocarDemora, bool colocarAntelacion, bool colocarFaltas)
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

    if (colocarDemora) {
        columnas++;
        labels << "Demora";
    }
    if (colocarAntelacion) {
        columnas++;
        labels << "Antelación";
    }
    if (colocarFaltas) {
        columnas++;
        labels << "Faltas";
    }
    ui->tableWidget->setColumnCount(columnas);
    ui->tableWidget->setHorizontalHeaderLabels(labels);

}

void Historial::consultaTodos(QDate inicio, QDate fin)
{
    ui->tableWidget->clear();
    QSettings config;
    int colocarColumnas = config.value("columnasHistorial", 1).toInt();
    std::bitset<3> bsColumnas(colocarColumnas);
    bool colocarDemora = bsColumnas.test(0);
    bool colocarAntelacion = bsColumnas.test(1);
    bool colocarFaltas = bsColumnas.test(2);
    colocarHeadersDias(inicio, fin, colocarDemora, colocarAntelacion, colocarFaltas);

    ui->tableWidget->setRowCount(0);
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->demoraLabel->setVisible(false);
    ui->antelacionLabel->setVisible(false);
    ui->faltasLabel->setVisible(false);

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
                    insertarItemTabla("", fila + 1, columna, 2, 1);
                }
                else
                {
                    if (DbManager::fechaRegistroEmpleado(indicesIds[i]) < d) {
                        faltas++;
                    }
                    insertarItemTabla("", fila + 1, columna, 2, 1);
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
        for (int i = 0; i < 3; i++) {
            if (bsColumnas.test(i)) {
                insertarItemTabla(totales.at(i), fila + 1, columna, 2, 1);
                columna++;
            }
        }
        fila += 3;
    }
}

void Historial::consultarRango(QDate inicio, QDate fin)
{
    ui->tableWidget->clear();
    colocarHeadersDias(inicio, fin);

    ui->tableWidget->setRowCount(2);
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
                insertarItemTabla("", 0, indiceColumna, 2);
            }
            else
            {
                if (DbManager::fechaRegistroEmpleado(idEmpleado) < inicio) {
                    faltas++;
                }
                insertarItemTabla("", 0, indiceColumna, 2);
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

    QSettings configuracion;
    int tolerancia = configuracion.value("tolerancia").toInt();

    if (diferenciaEntrada < 1)
    {
        difEntradaString = ":)";
    }
    else if (tolerancia > 0)
    {
        diferenciaEntrada -= tolerancia;
        difEntradaString += " - " + QString::number(tolerancia) + " = ";
        if (diferenciaEntrada < 1) difEntradaString += ":)";
        else difEntradaString += QString::number(diferenciaEntrada);
    }

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

    if (demora > 0) demoraString = QString::number(demora);
    else demoraString = ":)";

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
    QSettings config;
    std::bitset<3> columnas(config.value("columnasHistorial", 1).toInt());
    QVector<QLabel*> labels = {ui->demoraLabel, ui->antelacionLabel, ui->faltasLabel};
    QStringList textoLabels;
    textoLabels << "Minutos de demora totales:\t" << "Minutos de antelación totales:\t" << "Faltas:\t";
    for (int i = 0; i < 3; i++) {
        if (columnas.test(i)) {
            labels[i]->setVisible(true);
            labels[i]->setText(textoLabels[i] + totales.at(i));
        }
        else {
            labels[i]->setVisible(false);
        }
    }
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
    int periodo = ui->cboxPeriodo->currentIndex();
    int year = ui->yearPicker->value();
    int month = ui->monthPicker->currentIndex() + 1;
    int semana = ui->weekPicker->value();
    int quincena = ui->fortnightPicker->value();
    nombreReporte = "Reporte";
    QDate inicio, fin;

    switch (periodo)
    {
    case 0:
        inicio = QDate(year, 1, 1);
        fin = QDate(year, 12, 31);
        nombreReporte += QString::number(year);
        break;
    case 1:
        inicio = QDate(year, month, 1);
        fin = inicio.addMonths(1).addDays(-1);
        nombreReporte += ui->monthPicker->currentText() + QString::number(year);
        break;
    case 2:
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
        break;
    case 3:
        inicio = QDate(year, month, 1);
        while (inicio.dayOfWeek() != 1) inicio = inicio.addDays(-1);
        inicio = inicio.addDays(7 * (semana - 1));
        fin = inicio.addDays(6);
        nombreReporte += ui->monthPicker->currentText() + QString::number(year) + "-Semana" + QString::number(semana);
        break;
    case 4:
        inicio = ui->rangoFin->date();
        fin = inicio;
        nombreReporte += inicio.toString(Qt::ISODate);
        break;
    default:
        inicio = ui->rangoInicio->date();
        fin = ui->rangoFin->date();
        nombreReporte += inicio.toString(Qt::ISODate) + "---" + fin.toString(Qt::ISODate);
        break;
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

int Historial::numeroDeQuincena(QDate fecha)
{
    int dia = fecha.day();
    if (dia <= 15) return 1;
    else return 2;
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
    int indiceCbox = ui->empleadosCbox->currentIndex();
    if (indiceCbox != 0)
    {
        QString nombreEmpleado = ui->empleadosCbox->itemText(indiceCbox);
        nombreReporte += "-" + nombreEmpleado;
    }
    QString directorio = QFileDialog::getSaveFileName(this, "Elija un directorio.", "./" +
                                                      nombreReporte + ".csv", "Comma Separated Values (*.csv)");
    QFile archivo(directorio);

    if (!archivo.open(QFile::WriteOnly | QIODevice::NewOnly)) return;

    QTextStream salida(&archivo);
    int columnas = ui->tableWidget->columnCount();
    int filas = ui->tableWidget->rowCount();

    for (int i = 0; i < columnas; i++)
    {
        QString texto = ui->tableWidget->horizontalHeaderItem(i)->text();
        salida << reemplazarCaracteresEspeciales(texto) << ",";
    }
    salida << '\n';

    for (int i = 0; i < filas; i++)
    {
        for (int j = 0; j < columnas; j++)
        {
            QTableWidgetItem *item = ui->tableWidget->item(i, j);
            if (item == nullptr)
            {
                salida << " ,";
                continue;
            }
            salida << reemplazarCaracteresEspeciales(item->text()) << ",";
        }
        salida << '\n';
    }

    if (indiceCbox != 0)
    {
        QSettings config;
        int mostrar = config.value("columnasHistorial").toInt();
        std::bitset<3> cols(mostrar);
        qDebug() << mostrar;
        if (cols.test(0)) {
            QStringList demora = ui->demoraLabel->text().split('\t');
            salida << '\n' << demora[0] << ',' << demora[1];
        }
        if (cols.test(1)) {
            QStringList antelacion = ui->antelacionLabel->text().split('\t');
            salida << '\n' << antelacion[0] << ',' << antelacion[1];
        }
        if (cols.test(2)) {
            QStringList faltas = ui->faltasLabel->text().split('\t');
            salida << '\n' << faltas[0] << ',' << faltas[1];
        }
    }
    archivo.close();
}

QString Historial::reemplazarCaracteresEspeciales(QString texto)
{
    QString nuevoTexto = texto;
    for (QPair<QString, QString> reemplazo : reemplazos)
    {
        nuevoTexto = nuevoTexto.replace(reemplazo.first, reemplazo.second);
    }
    return nuevoTexto;
}

void Historial::yearSeleccion()
{
    ui->yearPicker->setVisible(true);
    ui->monthPicker->setVisible(false);
    ui->fortnightPicker->setVisible(false);
    ui->weekPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);
    ui->rangoFin->setVisible(false);
}

void Historial::monthSeleccion()
{
    yearSeleccion();
    ui->monthPicker->setVisible(true);
}

void Historial::fortnightSeleccion()
{
    monthSeleccion();
    ui->fortnightPicker->setVisible(true);
}

void Historial::weekSeleccion()
{
    monthSeleccion();
    ui->weekPicker->setVisible(true);
}

void Historial::daySeleccion()
{
    ui->rangoFin->setVisible(true);
    ui->yearPicker->setVisible(false);
    ui->monthPicker->setVisible(false);
    ui->weekPicker->setVisible(false);
    ui->fortnightPicker->setVisible(false);
    ui->rangoInicio->setVisible(false);
}

void Historial::rangeSeleccion()
{
    daySeleccion();
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

void Historial::on_cboxPeriodo_currentIndexChanged(int index)
{
    switch (index)
    {
    case 0:
        yearSeleccion();
        break;
    case 1:
        monthSeleccion();
        break;
    case 2:
        fortnightSeleccion();
        break;
    case 3:
        weekSeleccion();
        break;
    case 4:
        daySeleccion();
        break;
    case 5:
        rangeSeleccion();
    }
}

