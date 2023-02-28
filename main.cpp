#include "ventanaprincipal.h"

#include <QApplication>
#include <QFile>
#include <QSettings>
#include <cameramanager.h>
#include <dbmanager.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("SINAFO");
    QCoreApplication::setApplicationName("Sistema de Registro");
    if (!DbManager::abrirConexion()) return 1;
    QFile qss(":/stylesheets/stylesheet.qss");
    qss.open(QFile::ReadOnly);
    a.setStyleSheet(qss.readAll());
    CameraManager::verificarCamaras();
    VentanaPrincipal w;
    w.show();
    return a.exec();
}
