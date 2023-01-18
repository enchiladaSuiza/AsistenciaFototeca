#include "datos.h"
#include "ui_datos.h"
#include <QSqlError>

Datos::Datos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Datos)
{
    ui->setupUi(this);

    model = new QSqlTableModel;
    model->setTable("empleado");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    model->setHeaderData(0, Qt::Horizontal, tr("No. de Empleado"));
    model->setHeaderData(1, Qt::Horizontal, tr("Nombre(s)"));
    model->setHeaderData(2, Qt::Horizontal, tr("Apellido Paterno"));
    model->setHeaderData(3, Qt::Horizontal, tr("Apellido Materno"));
    model->setHeaderData(4, Qt::Horizontal, tr("CURP"));
    model->setHeaderData(5, Qt::Horizontal, tr("RFC"));
    model->setHeaderData(6, Qt::Horizontal, tr("Tipo de Contratación"));

    ui->tableView->setModel(model);
    ui->tableView->hideColumn(7);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tableView->horizontalHeader()->setMinimumHeight(35);

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
        qDebug() << "Error guardando los datos a la DB. Cancelando cambios.";
        qDebug() << model->lastError();
        model->revertAll();
    }
}

void Datos::on_revertirButton_clicked()
{
    model->revertAll();
}

void Datos::on_eliminarButton_clicked()
{
    model->removeRow(filaActual);
}

void Datos::actualizarSeleccion(const QModelIndex &index)
{
    filaActual = index.row();
    int id = model->index(filaActual, 0).data().toInt();
    emit registroSeleccionado(id);
    emit filaSeleccionada(filaActual);
}

void Datos::on_agregarButton_clicked()
{
    model->insertRow(model->rowCount());
}

void Datos::on_tableView_activated(const QModelIndex &index)
{
    actualizarSeleccion(index);
}

void Datos::on_tableView_clicked(const QModelIndex &index)
{
    actualizarSeleccion(index);
}
