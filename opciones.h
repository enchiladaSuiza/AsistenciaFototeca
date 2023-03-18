#ifndef OPCIONES_H
#define OPCIONES_H

#include <QSettings>
#include <QWidget>

namespace Ui {
class Opciones;
}

class Opciones : public QWidget
{
    Q_OBJECT

public:
    explicit Opciones(QWidget *parent = nullptr);
    ~Opciones();

    void actualizarTema();

signals:
    void temaActualizado(int tema);

private slots:
    void on_seleccionarArchivoButton_clicked();
    void on_contratacionesEdit_textChanged();

    void on_guardarButton_clicked();

    void on_toleranciaSbox_valueChanged(int arg1);

private:
    Ui::Opciones *ui;

    QSettings opciones;
    void establecerTemaClaro();
    void establecerTemaOscuro();
};

#endif // OPCIONES_H
