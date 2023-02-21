#ifndef CAMERAMANAGER_H
#define CAMERAMANAGER_H

#include <QCamera>
#include <QMediaDevices>

class CameraManager
{
public:
    static bool verificarCamaras();
    static QCamera* getCamara();

private:
    inline static QCamera* camara = new QCamera(QMediaDevices::defaultVideoInput());
};

#endif // CAMERAMANAGER_H
