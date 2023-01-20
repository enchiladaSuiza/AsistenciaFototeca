#ifndef PERSONAL_H
#define PERSONAL_H

#include <QSqlTableModel>
#include <QWidget>
#include <QVector>
#include <datos.h>
#include <horario.h>

namespace Ui {
class Personal;
}

class Personal : public QWidget
{
    Q_OBJECT

public:
    explicit Personal(QWidget *parent = nullptr);
    ~Personal();

private slots:
    void actualizarRegistroSeleccionado(QString *nombre);
private:
    Ui::Personal *ui;
    Datos datos;
    Horario horario;
};

#endif // PERSONAL_H
