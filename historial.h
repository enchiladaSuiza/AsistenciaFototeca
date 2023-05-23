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
    void yearSeleccion();
    void monthSeleccion();
    void weekSeleccion();
    void fortnightSeleccion();
    void daySeleccion();
    void rangeSeleccion();

    void on_exportarButton_clicked();

    void on_cboxAtrasButton_clicked();
    void on_cboxAdelanteButton_clicked();

    void actualizarConsultaSlotDate(const QDate &dia);
    void actualizarConsultaSlotInt(int slotInt);
    void on_cboxPeriodo_currentIndexChanged(int index);

private:
    Ui::Historial *ui;
    QTableWidgetItem *crearItemTabla(QString texto);
    QString nombreReporte = "";
    QList<int> indicesIds;

    void consultarRango(QDate inicio, QDate fin);
    int numeroDeSemana(QDate dia);
    int numeroDeQuincena(QDate dia);
    void insertarItemTabla(QString texto, int fila, int columna, int filaSpan = 1, int columnaSpan = 1);
    void consultaTodos(QDate inicio, QDate fin);
    QPair<int, int> colocarStringsCaptura(QSqlRecord registro, int fila, int columna);
    void colocarHeadersDias(QDate inicio, QDate fin,
                            bool colocarDemora = false, bool colocarAntelacion = false, bool colcarFaltas = false);
    void colocarTotales(int demora, int anticipacion, int faltas);
    QStringList conseguirTotales(int demora, int anticipacion, int faltas);
    QString reemplazarCaracteresEspeciales(QString texto);

    QList<QPair<QString, QString>> reemplazos = {
        {",", " "}, {"\n", "   "}, {"Á", "A"}, {"É", "E"}, {"Í", "I"}, {"Ó", "O"},
        {"Ú", "U"}, {"Ñ", "N"}, {"á", "a"}, {"é", "e"}, {"í", "i"}, {"ó", "o"},
        {"ú", "u"}, {"ñ", "n"}
    };
};

#endif // HISTORIAL_H
