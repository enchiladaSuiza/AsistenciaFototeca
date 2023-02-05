#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <checar.h>
#include <personal.h>

QT_BEGIN_NAMESPACE
namespace Ui { class VentanaPrincipal; }
QT_END_NAMESPACE

class VentanaPrincipal : public QMainWindow
{
    Q_OBJECT

public:
    VentanaPrincipal(QWidget *parent = nullptr);
    ~VentanaPrincipal();

private:
    Ui::VentanaPrincipal *ui;
    Checar checar;
    Personal personal;

    void actualizarTiempo();
    void timerEvent(QTimerEvent *event);

private slots:
    void on_checarButton_clicked();
    void on_personalButton_clicked();
    void on_stackedWidget_currentChanged(int arg1);
};
#endif // VENTANAPRINCIPAL_H
