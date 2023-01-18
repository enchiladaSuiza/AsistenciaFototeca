#ifndef DATOS_H
#define DATOS_H

#include <QItemSelectionModel>
#include <QWidget>
#include <qdatawidgetmapper.h>
#include <qsqltablemodel.h>

namespace Ui {
class Datos;
}

class Datos : public QWidget
{
    Q_OBJECT

public:
    explicit Datos(QWidget *parent = nullptr);
    ~Datos();

    QItemSelectionModel *getSelectionModel() const;
private slots:
    void on_guardarButton_clicked();

    void on_revertirButton_clicked();

    void on_eliminarButton_clicked();

    void on_agregarButton_clicked();

    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

signals:
    void registroSeleccionado(int id);
    void filaSeleccionada(int fila);

private:
    Ui::Datos *ui;
    QSqlTableModel *model;
    int filaActual;

    void actualizarSeleccion(const QModelIndex &index);
};

#endif // DATOS_H
