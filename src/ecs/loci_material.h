#ifndef LOCI_MATERIAL_H
#define LOCI_MATERIAL_H

#include "../core/loci_global.h"
#include "../core/loci_world.h"

typedef struct Loci_Material
{
    vec3 ambient;
    float roughness;
    float metallic;
    float reflection;
}Loci_Material;

void loci_createMaterial(ecs_entity_t entity, vec3 ambient, float roughness, float metallic, float reflection);
void loci_destroyMaterial(ecs_entity_t entity);

#endif