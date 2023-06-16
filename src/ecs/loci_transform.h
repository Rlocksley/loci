#ifndef LOCI_TRANSFORM_H
#define LOCI_TRANSFORM_H

#include "loci_global.h"
#include "loci_world.h"

typedef struct Loci_Transform
{
    mat4 transform;
}Loci_Transform;

void loci_createTransform
(ecs_entity_t entity,
vec3 scale, float angle, vec3 axis, vec3 translation);
void destroyTransform(ecs_entity_t entity);


#endif