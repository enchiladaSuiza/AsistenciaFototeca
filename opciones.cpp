#include "opciones.h"
#include "ui_opciones.h"
#include "QDebug"

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

