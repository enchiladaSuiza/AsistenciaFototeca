#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <QAbstractButton>
#include <QSqlRecord>
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
    void actualizarComboBox();
private slots:
    void on_yearSelect_clicked();
    void on_monthSelect_clicked();
    void on_weekSelect_clicked();
    void on_daySelect_clicked();
    void on_rangeSelect_clicked();

    void on_exportarButton_clicked();

    void on_fortnightSelect_clicked();
    void on_cboxAtrasButton_clicked();
    void on_cboxAdelanteButton_clicked();

    void actualizarConsultaSlotDate(const QDate &dia);
    void actualizarConsultaSlotInt(int slotInt);
private:
    Ui::Historial *ui;
    QTableWidgetItem *crearItemTabla(QString texto);
    QString nombreReporte = "";
    QList<int> indicesIds;

    void consultarRango(QDate inicio, QDate fin);
    int numeroDeSemana(QDate dia);
    void insertarItemTabla(QString texto, int fila, int columna, int filaSpan = 1, int columnaSpan = 1);
    void consultaTodos(QDate inicio, QDate fin);
    QPair<int, int> colocarStringsCaptura(QSqlRecord registro, int fila, int columna);
    void colocarHeadersDias(QDate inicio, QDate fin, bool colocarTotales = false);
    void colocarTotales(int demora, int anticipacion, int faltas);
    QStringList conseguirTotales(int demora, int anticipacion, int faltas);
};

#endif // HISTORIAL_H
