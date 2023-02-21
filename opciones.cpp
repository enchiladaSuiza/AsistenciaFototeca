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
    ui->contratacionesPlainTextEdit->setPlainText(contrataciones);
}

Opciones::~Opciones()
{
    delete ui;
}

void Opciones::on_seleccionarArchivoButton_clicked()
{
    QString archivo = DbManager::conseguirArchivo();
    opciones.setValue("archivoBase", archivo);
    ui->nombreArchivoLabel->setText(archivo);
}

void Opciones::on_contratacionesPlainTextEdit_textChanged()
{
    ui->guardarContratacionesButton->setEnabled(true);
}

void Opciones::on_guardarContratacionesButton_clicked()
{
    QString contrataciones = ui->contratacionesPlainTextEdit->toPlainText();
    opciones.setValue("contrataciones", contrataciones);
    ui->guardarContratacionesButton->setEnabled(false);
}

