#ifndef HISTORIAL_H
#define HISTORIAL_H

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

private slots:
    void on_yearSelect_clicked();

    void on_monthSelect_clicked();

    void on_weekSelect_clicked();

    void on_daySelect_clicked();

    void on_rangeSelect_clicked();

private:
    Ui::Historial *ui;
    void consultarDia();
};

#endif // HISTORIAL_H
