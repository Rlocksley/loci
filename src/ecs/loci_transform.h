#ifndef LOCI_TRANSFORM_H
#define LOCI_TRANSFORM_H

#include "../core/loci_global.h"
#include "../core/loci_world.h"

typedef struct Loci_Transform
{
    mat4 transform;
}Loci_Transform;

void loci_createTransform
(ecs_entity_t entity,
vec3 scale, float angle, vec3 axis, vec3 translation);
void loci_createTransform2(ecs_entity_t entity, mat4 transform);
void loci_destroyTransform(ecs_entity_t entity);


#endif