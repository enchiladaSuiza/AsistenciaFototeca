#ifndef PERSONAL_H
#define PERSONAL_H

#include <QSqlTableModel>
#include <QWidget>
#include <QVector>
#include <datos.h>
#include <horario.h>
#include <escaner.h>
#include <foto.h>

namespace Ui {
class Personal;
}

class Personal : public QWidget
{
    Q_OBJECT

public:
    explicit Personal(QWidget *parent = nullptr);
    ~Personal();

    void activarCamaras();
    void desactivarCamaras();

private slots:
    void actualizarRegistroSeleccionado(QString *nombre);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::Personal *ui;
    Datos datos;
    Horario horario;
    Escaner escaner;
    Foto foto;
};

#endif // PERSONAL_H
