#include "opciones.h"
#include "ui_opciones.h"
#include "QDebug"

#include <QFile>
#include <dbmanager.h>

Opciones::Opciones(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Opciones)
{
    ui->setupUi(this);

    QString archivoBase = opciones.value("archivoBase").toString();
    ui->nombreArchivoLabel->setText(archivoBase);

    QString contrataciones = opciones.value("contrataciones").toString();
    ui->contratacionesEdit->setPlainText(contrataciones);

    int minutosTolerancia = opciones.value("tolerancia").toInt();
    ui->toleranciaSbox->setValue(minutosTolerancia);

    int tema = opciones.value("tema").toInt();
    if (tema == 0) ui->claroButton->setChecked(true);
    else ui->oscuroButton->setChecked(true);

    connect(ui->claroButton, &QRadioButton::clicked, this, &Opciones::establecerTemaClaro);
    connect(ui->oscuroButton, &QRadioButton::clicked, this, &Opciones::establecerTemaOscuro);
}

Opciones::~Opciones()
{
    delete ui;
}

void Opciones::on_seleccionarArchivoButton_clicked()
{
    QString archivo = DbManager::conseguirArchivo();
    if (archivo.isEmpty()) return;
    opciones.setValue("archivoBase", archivo);
    ui->nombreArchivoLabel->setText(archivo);
}

void Opciones::on_contratacionesEdit_textChanged()
{
    ui->guardarButton->setEnabled(true);
}

void Opciones::on_guardarButton_clicked()
{
    QString contrataciones = ui->contratacionesEdit->toPlainText();
    opciones.setValue("contrataciones", contrataciones);

    int minutosTolerancia = ui->toleranciaSbox->value();
    opciones.setValue("tolerancia", minutosTolerancia);
    ui->guardarButton->setEnabled(false);
}


void Opciones::on_toleranciaSbox_valueChanged(int valor)
{
    Q_UNUSED(valor);
    ui->guardarButton->setEnabled(true);
}

void Opciones::establecerTemaClaro()
{
    opciones.setValue("tema", 0);
    actualizarTema();
}

void Opciones::establecerTemaOscuro()
{
    opciones.setValue("tema", 1);
    actualizarTema();
}

void Opciones::actualizarTema()
{
    QSettings opciones;
    int tema = opciones.value("tema").toInt();
    QFile qss(":/stylesheets/stylesheet.qss");
    qss.open(QFile::ReadOnly);
    if (tema == 0)
    {
        qApp->setStyleSheet(qss.readAll());
        return;
    }
    QFile dark(":/stylesheets/dark.qss");
    dark.open(QFile::ReadOnly);
    qApp->setStyleSheet(dark.readAll() + qss.readAll());
}
