#include "cameramanager.h"
#include <QCameraDevice>
#include <QMediaDevices>

bool CameraManager::verificarCamaras() { return QMediaDevices::videoInputs().count() > 0; }

QCamera* CameraManager::getCamara() { return new QCamera(QMediaDevices::defaultVideoInput()); }
