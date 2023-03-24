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

    void on_pantallaButton_clicked();

    void on_listView_clicked(const QModelIndex &index);

    void on_editarUsuarioButton_clicked();

    void cambiosUsuarios();
    void on_eliminarUsuarioButton_clicked();

    void on_nuevoUsuarioButton_clicked();

private:
    Ui::Opciones *ui;

    QSettings opciones;

    int idUsuario = -1;
    int privilegiosUsuario = 0;
    void establecerTemaClaro();
    void establecerTemaOscuro();
};

#endif // OPCIONES_H
