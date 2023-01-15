#include "ventanaprincipal.h"

#include <QApplication>
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbManager::abrirConexion("/home/santiago/Databases/fototeca.db");
    VentanaPrincipal w;
    w.show();
    return a.exec();
}
