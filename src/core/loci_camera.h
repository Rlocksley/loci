#ifndef LOCI_EYE_H
#define LOCI_EYE_H
#include "loci_eye.h"
#endif
#ifndef LOCI_INPUT_H
#define LOCI_INPUT_H
#include "loci_input.h"
#endif

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



Loci_FlyingCamera loci_createFlyingCamera
(vec3 position, vec2 angle, 
float speed, float rotationSpeed,
float fov, float nearClip, float farClip)
{
    Loci_FlyingCamera camera;

    glm_vec3_copy(position, camera.eye.position); 
    glm_vec3_copy
    ((vec3)
    {cos(angle[1])*sin(angle[0]),
    sin(angle[1]),
    cos(angle[1])*cos(angle[0])},
    camera.eye.direction);
    
    vec3 target;
    glm_vec3_add(position, camera.eye.direction, target);
    glm_lookat
    (position, target, 
    (vec3){0.f,1.f,0.f},
    camera.eye.viewInverse);
    glm_mat4_inv
    (camera.eye.viewInverse, 
    camera.eye.viewInverse);
    
    glm_perspective
    (fov, ((float)loci_windowWidth)/((float)loci_windowHeight),
    nearClip, farClip,
    camera.eye.projInverse);
    glm_mat4_inv
    (camera.eye.projInverse,
    camera.eye.projInverse);

    glm_vec2_copy(angle, camera.angle);
    camera.speed = speed;
    camera.rotationSpeed = rotationSpeed;
    camera.fov = fov;
    camera.nearClip = nearClip;
    camera.farClip = farClip;
    camera.eye.clip[0] = nearClip;
    camera.eye.clip[1] = farClip;
    
    return camera;
}

void loci_updateFlyingCamera(Loci_FlyingCamera* pCamera)
{
    Loci_FlyingCamera camera = *pCamera;

    vec2 deltaCursor;
    glm_vec2_copy(loci_cursor.deltaPosition,deltaCursor);
    glm_vec2_scale
    (deltaCursor, 
    camera.rotationSpeed*loci_buttonRight.down,
    deltaCursor);

    glm_vec2_add(camera.angle, deltaCursor, camera.angle);

    glm_vec3_copy
    ((vec3)
    {cos(camera.angle[1])*sin(camera.angle[0]),
    sin(camera.angle[1]),
    cos(camera.angle[1])*cos(camera.angle[0])},
    camera.eye.direction);
    
    vec3 scaledDirection;
    glm_vec3_scale
    (camera.eye.direction, 
    camera.speed*loci_deltaTime*
    (loci_keyW.down-loci_keyS.down),
    scaledDirection);

    glm_vec3_add(camera.eye.position, scaledDirection, camera.eye.position);

    vec3 right;
    glm_vec3_cross(camera.eye.direction, (vec3){0.f,1.f,0.f}, right);

    vec3 scaledRight;
    glm_vec3_scale
    (right,
    camera.speed*loci_deltaTime*
    (loci_keyD.down-loci_keyA.down),
    scaledRight);

    glm_vec3_add(camera.eye.position, scaledRight, camera.eye.position);

    vec3 upView;
    glm_vec3_cross
    (right, camera.eye.direction, upView);
    
    vec3 target;
    glm_vec3_add
    (camera.eye.position, camera.eye.direction, target);

    glm_lookat(camera.eye.position, target, upView, camera.eye.viewInverse);
    glm_mat4_inv(camera.eye.viewInverse, camera.eye.viewInverse);

    loci_writeToEyeBuffer(&camera.eye);

    *pCamera = camera;
}