#include "ventanaprincipal.h"

#include <QApplication>
#include <QSettings>
#include <cameramanager.h>
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("INAH");
    QCoreApplication::setApplicationName("Sistema de Registro");
    if (!DbManager::abrirConexion()) return 1;
    CameraManager::verificarCamaras();
    VentanaPrincipal w;
    w.show();
    return a.exec();
}
