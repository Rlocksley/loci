#ifndef LOCI_POINTLIGHT_H
#define LOCI_POINTLIGHT_H

#include "../core/loci_global.h"
#include "../core/loci_world.h"

typedef struct Loci_PointLight
{
    vec3 color;
}Loci_PointLight;

void loci_createPointLight(ecs_entity_t entity, vec3 color);
void loci_destroyPointLight(ecs_entity_t entity);


#endif