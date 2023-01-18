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
    void setRegistro(int id);

    void on_tabWidget_currentChanged(int index);

private:
    Ui::Personal *ui;
    Datos datos;
    Horario horario;
    int registro = -1;
};

#endif // PERSONAL_H
