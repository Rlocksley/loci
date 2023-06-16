#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H
#include "../core/loci_world.h"
#endif

typedef struct Loci_Material
{
    vec3 ambient;
    float roughness;
    float metallic;
    float reflection;
}Loci_Material;

void loci_createMaterial(ecs_entity_t entity, vec3 ambient, float roughness, float metallic, float reflection)\
{
    Loci_Material material;
    glm_vec3_copy(ambient, material.ambient);
    material.roughness = roughness;
    material.metallic = metallic;
    material.reflection = reflection;

    loci_set(entity, Loci_Material, material);
}

#define loci_destroyMaterial(entity)\
{\
    loci_remove(entity, Loci_Material)\
}
