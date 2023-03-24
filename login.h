#ifndef LOGIN_H
#define LOGIN_H

#include "dbmanager.h"

#include <QDialog>

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(QWidget *parent = nullptr);
    ~Login();

    int registrarAdmin();
    int iniciarSesion();

    static inline int hayAdmins = false;
    static inline int usuarioId = -1;

    static bool accesoPersonal();
    static bool accesoHistorial();
    static bool accesoOpciones();

    static void cerrarSesion();

    int editarUsuario(int id, QString nombre, int privilegios = 4);
    int registrarUsuario();
signals:
    void usuarioRegistrado(QString nombre);
    void usuarioActualizado();

private slots:
    void on_cancelarButton_clicked();
    void on_ingresarButton_clicked();
    void on_mostrarButton_clicked();

private:
    Ui::Login *ui;

    int tipoDialogo;
    int idUsuario;

    static bool conseguirPrivilegio(int posicion);
};

#endif // LOGIN_H
