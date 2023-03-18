#ifndef VENTANAPRINCIPAL_H
#define VENTANAPRINCIPAL_H

#include <QMainWindow>
#include <QSvgWidget>
#include <checar.h>
#include <historial.h>
#include <opciones.h>
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
    Checar* checar;
    Personal* personal;
    Historial* historial;
    Opciones *opciones;

    QDateTime ahora;

    void actualizarTiempo();
    void timerEvent(QTimerEvent *event);

    QSvgWidget *cultura, *inah, *sinafo;

    void activarTodosLosBotones();
private slots:
    void on_checarButton_clicked();
    void on_personalButton_clicked();
    void on_stackedWidget_currentChanged(int index);
    void on_historialButton_clicked();
    void on_opcionesButton_clicked();
    void cambiarLogos(int tema);
};
#endif // VENTANAPRINCIPAL_H
