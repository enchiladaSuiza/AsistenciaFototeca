#ifndef HORARIO_H
#define HORARIO_H

#include <QWidget>
#include <QSqlTableModel>
#include <qdatawidgetmapper.h>

namespace Ui {
class Horario;
}

class Horario : public QWidget
{
    Q_OBJECT

public:
    explicit Horario(QWidget *parent = nullptr);
    ~Horario();
    QDataWidgetMapper *getMapper() const;

    void seleccionarRegistro(int id);
    void setFila(int fila);
private slots:
    void on_lunesCBox_stateChanged(int arg1);

    void on_martesCBox_stateChanged(int arg1);

    void on_miercolesCBox_stateChanged(int arg1);

    void on_juevesCBox_stateChanged(int arg1);

    void on_viernesCBox_stateChanged(int arg1);

    void on_sabadoCBox_stateChanged(int arg1);

    void on_domingoCBox_stateChanged(int arg1);

    void on_guardarButton_clicked();

    void on_descartarButton_clicked();

private:
    Ui::Horario *ui;
    QSqlTableModel *model;
    QDataWidgetMapper *mapper;
};

#endif // HORARIO_H
