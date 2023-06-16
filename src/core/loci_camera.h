#ifndef LOCI_CAMERA_H
#define LOCI_CAMERA_H

#include "loci_eye.h"
#include "loci_input.h"

typedef struct Loci_FlyingCamera
{
    Loci_Eye eye;
    vec2 angle;
    float speed;
    float rotationSpeed;
    float fov;
    float nearClip;
    float farClip;
}Loci_FlyingCamera;


Loci_FlyingCamera loci_createFlyingCamera
(vec3 position, vec2 angle, 
float speed, float rotationSpeed,
float fov, float nearClip, float farClip);
void loci_updateFlyingCamera(Loci_FlyingCamera* pCamera);

#endif