#include "checar.h"
#include "ui_checar.h"

#include <QDate>

Checar::Checar(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Checar)
{
    ui->setupUi(this);
}

Checar::~Checar()
{
    delete ui;
}
