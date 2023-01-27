#include "cameramanager.h"
#include <QCameraDevice>
#include <QMediaDevices>

bool CameraManager::verificarCamaras()
{
    if (QMediaDevices::videoInputs().count() > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}
