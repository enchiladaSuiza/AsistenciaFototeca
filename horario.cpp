#include "horario.h"
#include "ui_horario.h"

#include <QSqlError>
#include <timeeditdelegate.h>

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
    mapper->setItemDelegate(new TimeEditDelegate(this));

    camposDias.insert(ui->lunesCBox, qMakePair(ui->lunesEntrada, ui->lunesSalida));
    camposDias.insert(ui->martesCBox, qMakePair(ui->martesEntrada, ui->martesSalida));
    camposDias.insert(ui->miercolesCBox, qMakePair(ui->miercolesEntrada, ui->miercolesSalida));
    camposDias.insert(ui->juevesCBox, qMakePair(ui->juevesEntrada, ui->juevesSalida));
    camposDias.insert(ui->viernesCBox, qMakePair(ui->viernesEntrada, ui->viernesSalida));
    camposDias.insert(ui->sabadoCBox, qMakePair(ui->sabadoEntrada, ui->sabadoSalida));
    camposDias.insert(ui->domingoCBox, qMakePair(ui->domingoEntrada, ui->domingoSalida));

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

    connect(model, &QSqlTableModel::dataChanged, this, &Horario::cambiosDetectados);
}

Horario::~Horario()
{
    delete ui;
}

QDataWidgetMapper* Horario::getMapper() const
{
    return mapper;
}

void Horario::actualizarRegistros()
{
    model->select();
}

void Horario::setFila(int fila)
{
    mapper->setCurrentIndex(fila);
    // qDebug() << "En fila " << fila;
    actualizarCheckBoxes();
}

void Horario::toggleTimeEdits(QCheckBox* dia)
{
    QTimeEdit *entrada = camposDias.value(dia).first;
    QTimeEdit *salida = camposDias.value(dia).second;
    if (dia->isChecked())
    {
        entrada->setEnabled(true);
        salida->setEnabled(true);

    }
    else
    {
        entrada->setEnabled(false);
        salida->setEnabled(false);
    }
    emit mapper->itemDelegate()->commitData(entrada);
    emit mapper->itemDelegate()->commitData(salida);
}

void Horario::actualizarCheckBoxes()
{
    QMapIterator<QCheckBox*, QPair<QTimeEdit*, QTimeEdit*>> i(camposDias);
    while (i.hasNext())
    {
        i.next();
        i.key()->setChecked(i.value().first->isEnabled());
    }
}

void Horario::cambiosDetectados()
{
    activarBotonesDeCambios(true);
}

void Horario::activarBotonesDeCambios(bool activos)
{
    ui->descartarButton->setEnabled(activos);
    ui->guardarButton->setEnabled(activos);
}

void Horario::on_guardarButton_clicked()
{
    int indice = mapper->currentIndex();
    if (!model->submitAll())
    {
        qDebug() << "Error guardando los datos a la DB. Cancelando cambios.";
        qDebug() << model->lastError();
        model->revertAll();
    }
    mapper->setCurrentIndex(indice);
    activarBotonesDeCambios(false);
}

void Horario::on_descartarButton_clicked()
{
    model->revertAll();
    mapper->revert();
    actualizarCheckBoxes();
    activarBotonesDeCambios(false);
}


void Horario::on_lunesCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->lunesCBox);
}

void Horario::on_martesCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->martesCBox);
}

void Horario::on_miercolesCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->miercolesCBox);
}

void Horario::on_juevesCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->juevesCBox);
}

void Horario::on_viernesCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->viernesCBox);
}

void Horario::on_sabadoCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->sabadoCBox);
}

void Horario::on_domingoCBox_stateChanged(int arg1)
{
    (void)arg1;
    toggleTimeEdits(ui->domingoCBox);
}
