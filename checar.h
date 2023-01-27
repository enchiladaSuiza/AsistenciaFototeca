#ifndef CHECAR_H
#define CHECAR_H

#include <QCamera>
#include <QWidget>

namespace Ui {
class Checar;
}

class Checar : public QWidget
{
    Q_OBJECT

public:
    explicit Checar(QWidget *parent = nullptr);
    ~Checar();

private slots:
    void on_toggleCamera_clicked();

private:
    Ui::Checar *ui;
    QCamera *camera;

    void mostrarInformacion(bool mostrar);
};

#endif // CHECAR_H
