#ifndef CAMERA_H
#define CAMERA_H

#include <QMouseEvent>
#include "vector.h"
/**
    Camera struct
*/

//struct Camera{
//    Vector3 eye, center, up;
//    float fovy, near, far;
//};


struct OrbitCamera
{
    Vector3 center, up;
    float theta, phi;
    float fovy;
    float zoom;

    void mouseMove(const Vector2 &delta);
    void mouseWheel(float delta);
};

#endif // CAMERA_H
