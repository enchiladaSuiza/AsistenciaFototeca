#include "personal.h"
#include "ui_personal.h"
#include <QSqlError>

Personal::Personal(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Personal)
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
    // ui->tableView->resizeColumnsToContents();
    ui->tableView->setWordWrap(true);
    // ui->tableView->verticalHeader()->hide();
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter | (Qt::Alignment)Qt::TextWordWrap);
    ui->tableView->horizontalHeader()->setMinimumHeight(35);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
}

Personal::~Personal()
{
    delete ui;
}

void Personal::on_revertirButton_clicked()
{
    for (int fila : filasEliminadas)
    {
        ui->tableView->showRow(fila);
    }
    filasEliminadas.clear();
    model->revertAll();
}

void Personal::on_guardarButton_clicked()
{
    if (!model->submitAll())
    {
        qDebug() << "Error guardando los datos a la DB. Cancelando cambios.";
        qDebug() << model->lastError();
        model->revertAll();
    }
}

void Personal::on_agregarButton_clicked()
{
    model->insertRow(model->rowCount());
}

void Personal::on_eliminarButton_clicked()
{
    ui->tableView->hideRow(filaSeleccionada);
    filasEliminadas.append(filaSeleccionada);
    model->removeRow(filaSeleccionada);
}

void Personal::actualizarSeleccion(const QModelIndex &index)
{
    filaSeleccionada = index.row();
    // qDebug() << filaSeleccionada;
    QString nombre = model->index(filaSeleccionada, 1).data().toString();
    nombre.append(" " + model->index(filaSeleccionada, 2).data().toString());
    nombre.append(" " + model->index(filaSeleccionada, 3).data().toString());
    ui->seleccionLabel->setText(nombre);
}

void Personal::on_tableView_activated(const QModelIndex &index)
{
    actualizarSeleccion(index);
}

void Personal::on_tableView_clicked(const QModelIndex &index)
{
    actualizarSeleccion(index);
}

