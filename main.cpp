#include "ventanaprincipal.h"

#include <QApplication>
#include <dbmanager.h>
#include <cameramanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DbManager::abrirConexion("/home/santiago/Databases/fototeca.db");
    CameraManager::verificarCamaras();
    VentanaPrincipal w;
    w.show();
    return a.exec();
}
