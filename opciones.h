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

private slots:
    void on_seleccionarArchivoButton_clicked();
    void on_contratacionesPlainTextEdit_textChanged();

    void on_guardarContratacionesButton_clicked();

private:
    Ui::Opciones *ui;

    QSettings opciones;
};

#endif // OPCIONES_H
