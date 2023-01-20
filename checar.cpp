#include "checar.h"
#include "ui_checar.h"

#include <QDate>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);

    mostrarInformacion(false);
}

void Checar::mostrarInformacion(bool mostrar)
{
    ui->entradaNormalLabel->setVisible(mostrar);
    ui->entradaCapturaLabel->setVisible(mostrar);
    ui->salidaNormalLabel->setVisible(mostrar);
    ui->salidaCapturaLabel->setVisible(mostrar);
}

Checar::~Checar()
{
    delete ui;
}
