#include "horario.h"
#include "ui_horario.h"

#include <QSqlError>

Horario::Horario(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Horario)
{
    ui->setupUi(this);

    model = new QSqlTableModel;
    model->setTable("horario");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    mapper = new QDataWidgetMapper;
    mapper->setModel(model);
    mapper->addMapping(ui->lunesEntrada, model->fieldIndex("lunes_e"));
    mapper->addMapping(ui->lunesSalida, model->fieldIndex("lunes_s"));
    mapper->addMapping(ui->martesEntrada, model->fieldIndex("martes_e"));
    mapper->addMapping(ui->martesSalida, model->fieldIndex("martes_s"));
    mapper->addMapping(ui->miercolesEntrada, model->fieldIndex("miercoles_e"));
    mapper->addMapping(ui->miercolesSalida, model->fieldIndex("miercoles_s"));
    mapper->addMapping(ui->juevesEntrada, model->fieldIndex("jueves_e"));
    mapper->addMapping(ui->juevesSalida, model->fieldIndex("jueves_s"));
    mapper->addMapping(ui->viernesEntrada, model->fieldIndex("viernes_e"));
    mapper->addMapping(ui->viernesSalida, model->fieldIndex("viernes_s"));
    mapper->addMapping(ui->sabadoEntrada, model->fieldIndex("sabado_e"));
    mapper->addMapping(ui->sabadoSalida, model->fieldIndex("sabado_s"));
    mapper->addMapping(ui->domingoEntrada, model->fieldIndex("domingo_e"));
    mapper->addMapping(ui->domingoSalida, model->fieldIndex("domingo_s"));
}

Horario::~Horario()
{
    delete ui;
}

QDataWidgetMapper* Horario::getMapper() const
{
    return mapper;
}

void Horario::setFila(int fila)
{
    mapper->setCurrentIndex(fila);
}

void toggleTimeEdits(QTimeEdit *entrada, QTimeEdit *salida, int habilitados)
{
    if (habilitados == Qt::Checked)
    {
        entrada->setEnabled(true);
        salida->setEnabled(true);
    }
    else
    {
        entrada->setEnabled(false);
        salida->setEnabled(false);
    }
}

void Horario::on_lunesCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->lunesEntrada, ui->lunesSalida, arg1);
}

void Horario::on_martesCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->martesEntrada, ui->martesSalida, arg1);
}

void Horario::on_miercolesCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->miercolesEntrada, ui->miercolesSalida, arg1);
}

void Horario::on_juevesCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->juevesEntrada, ui->juevesSalida, arg1);
}

void Horario::on_viernesCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->viernesEntrada, ui->viernesSalida, arg1);
}

void Horario::on_sabadoCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->sabadoEntrada, ui->sabadoSalida, arg1);
}

void Horario::on_domingoCBox_stateChanged(int arg1)
{
    toggleTimeEdits(ui->domingoEntrada, ui->domingoSalida, arg1);
}

void Horario::on_guardarButton_clicked()
{
    if (!model->submitAll())
    {
        qDebug() << "Error guardando los datos a la DB. Cancelando cambios.";
        qDebug() << model->lastError();
        model->revertAll();
    }
}

void Horario::on_descartarButton_clicked()
{
    model->revertAll();
}
