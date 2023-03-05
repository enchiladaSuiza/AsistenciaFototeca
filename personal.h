#ifndef PERSONAL_H
#define PERSONAL_H

#include <QSqlTableModel>
#include <QWidget>
#include <QVector>
#include <datos.h>
#include <horario.h>
#include <escaner.h>

namespace Ui {
class Personal;
}

class Personal : public QWidget
{
    Q_OBJECT

public:
    explicit Personal(QWidget *parent = nullptr);
    ~Personal();

    void activarCamaraQR();
    void desactivarCamaraQR();

private slots:
    void actualizarRegistroSeleccionado(QString *nombre);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::Personal *ui;
    Datos datos;
    Horario horario;
    Escaner escaner;
};

#endif // PERSONAL_H
