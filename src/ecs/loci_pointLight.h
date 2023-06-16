#ifndef LOCI_GLOBAL_H
#define LOCI_GLOBAL_H
#include "loci_global.h"
#endif
#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H
#include "loci_world.h"
#endif

typedef struct Loci_PointLight
{
    vec3 color;
}Loci_PointLight;

void loci_createPointLight(ecs_entity_t entity, vec3 color)
{
    Loci_PointLight light;
    glm_vec3_copy(color, light.color);

    loci_set(entity, Loci_PointLight, light);
}

void loci_destroyPointLight(ecs_entity_t entity)
{
    loci_remove(entity, Loci_PointLight);
}

