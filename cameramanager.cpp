#include "cameramanager.h"
#include <QCameraDevice>
#include <QMediaDevices>
#include <QMessageBox>

bool CameraManager::verificarCamaras() {
    if (QMediaDevices::videoInputs().count() < 1)
    {
        QMessageBox box;
        box.setIcon(QMessageBox::Critical);
        box.setWindowTitle("Sistema de Control de Asistencia de la Fototeca Nacional");
        box.setText("No se encuentra ninguna cámara.");
        box.setInformativeText("El programa podrá usarse para fines de administración pero no será posible capturar."
                               "Asegúrese de conectar correctamente una cámara y vuevla a ejecutar el programa.");
        return false;
    }
    return true;
}

QCamera* CameraManager::nuevaCamara() { return new QCamera(QMediaDevices::defaultVideoInput()); }
