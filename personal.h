#ifndef PERSONAL_H
#define PERSONAL_H

#include <QSqlTableModel>
#include <QWidget>
#include <QVector>

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
    void on_revertirButton_clicked();

    void on_guardarButton_clicked();

    void on_agregarButton_clicked();

    void on_eliminarButton_clicked();

    void actualizarSeleccion(const QModelIndex &index);

    void on_tableView_activated(const QModelIndex &index);

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Personal *ui;
    QSqlTableModel *model;
    int filaSeleccionada;
    QVector<int> filasEliminadas;
};

#endif // PERSONAL_H
