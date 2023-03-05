#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <QAbstractButton>
#include <QTableWidgetItem>
#include <QWidget>

namespace Ui {
class Historial;
}

class Historial : public QWidget
{
    Q_OBJECT

public:
    explicit Historial(QWidget *parent = nullptr);
    ~Historial();

    void actualizarConsulta();
private slots:
    void on_yearSelect_clicked();

    void on_monthSelect_clicked();

    void on_weekSelect_clicked();

    void on_daySelect_clicked();

    void on_rangeSelect_clicked();

    void on_rangoFin_dateChanged(const QDate &date);

    void on_yearPicker_valueChanged(int arg1);
    void on_monthPicker_currentIndexChanged(int index);
    void on_rangoInicio_dateChanged(const QDate &date);
    void on_weekPicker_valueChanged(int arg1);
    void on_exportarButton_clicked();

    void on_fortnightSelect_clicked();

private:
    Ui::Historial *ui;
    QTableWidgetItem *crearItemTabla(QString texto);
    QString nombreReporte = "";
    void consultarDia(QDate dia);
    void consultarRango(QDate inicio, QDate fin);
    int numeroDeSemana(QDate dia);
};

#endif // HISTORIAL_H
