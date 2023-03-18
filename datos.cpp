#include "datos.h"
#include "ui_datos.h"
#include <QMessageBox>
#include <QSqlError>
#include <comboboxdelegate.h>
#include <spinboxdelegate.h>

Datos::Datos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Datos)
{
    ui->setupUi(this);

    model = new QSqlTableModel;
    model->setTable("empleado");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, "No. de\nEmpleado");
    model->setHeaderData(1, Qt::Horizontal, "Nombre(s)");
    model->setHeaderData(2, Qt::Horizontal, "Apellido\nPaterno");
    model->setHeaderData(3, Qt::Horizontal, "Apellido\nMaterno");
    model->setHeaderData(4, Qt::Horizontal, "CURP");
    model->setHeaderData(5, Qt::Horizontal, "RFC");
    model->setHeaderData(6, Qt::Horizontal, "Tipo de\nContratación");

    connect(model, &QSqlTableModel::dataChanged, this, &Datos::cambiosDetectados);

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(7);
    ui->tableView->hideColumn(8);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tableView->horizontalHeader()->setMinimumHeight(36);
    ui->tableView->verticalHeader()->setDefaultAlignment(Qt::AlignCenter);
    ui->tableView->verticalHeader()->setMinimumWidth(15);

    ui->tableView->setItemDelegateForColumn(0, new SpinBoxDelegate(this));
    ui->tableView->setItemDelegateForColumn(6, new ComboBoxDelegate(this));
    ui->tableView->resizeColumnToContents(6);
}

Datos::~Datos()
{
    delete ui;
}

QItemSelectionModel* Datos::getSelectionModel() const
{
    return ui->tableView->selectionModel();
}

void Datos::on_guardarButton_clicked()
{
    if (!model->submitAll())
    {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setWindowTitle("Sistema de Control de Asistencia de la Fototeca Nacional");
        box.setText("Error guardando a la base de datos. Cancelando cambios.");
        box.setInformativeText(model->lastError().text());
        box.exec();
        model->revertAll();
        return;
    }
    ui->tableView->clearSelection();
    activarBotonesDeCambios(false);
    filasAgregadas.clear();
    emit nombreCompletoSeleccion(&emptyString);
    emit dbActualizada();
}

void Datos::on_revertirButton_clicked()
{
    model->revertAll();
    ui->tableView->clearSelection();
    activarBotonesDeCambios(false);
    filasAgregadas.clear();
    emit nombreCompletoSeleccion(&emptyString);
}

void Datos::on_eliminarButton_clicked()
{
    model->removeRow(filaActual);
    activarBotonesDeCambios(true);
}

void Datos::actualizarSeleccion(const QModelIndex &index)
{
    filaActual = index.row();
    QString *nombre = nullptr;
    int id = -1;

    if (filasAgregadas.contains(filaActual)) return;

    id = model->index(filaActual, 0).data().toInt();
    nombre = new QString(model->index(filaActual, 1).data().toString()
            + " " + model->index(filaActual, 2).data().toString()
            + " " + model->index(filaActual, 3).data().toString());

    emit registroSeleccionado(id);
    emit nombreCompletoSeleccion(nombre);
}

void Datos::cambiosDetectados()
{
    activarBotonesDeCambios(true);
}

void Datos::activarBotonesDeCambios(bool activos)
{
    ui->revertirButton->setEnabled(activos);
    ui->guardarButton->setEnabled(activos);
}

void Datos::on_agregarButton_clicked()
{
    int filaAgregada = model->rowCount();
    model->insertRow(filaAgregada);
    filasAgregadas.append(filaAgregada);
    activarBotonesDeCambios(true);
}

void Datos::on_tableView_activated(const QModelIndex &index)
{
    actualizarSeleccion(index);
}

void Datos::on_tableView_clicked(const QModelIndex &index)
{
    actualizarSeleccion(index);
}
