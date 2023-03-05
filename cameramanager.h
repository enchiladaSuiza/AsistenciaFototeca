#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QCamera>
#include <QMediaDevices>

class CameraManager
{
public:
    static bool verificarCamaras();
    static QCamera* nuevaCamara();
};

#endif // CAMERAMANAGER_H
