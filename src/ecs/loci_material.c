#include "loci_material.h"

void loci_createMaterial(ecs_entity_t entity, vec3 ambient, float roughness, float metallic, float reflection)
{
    Loci_Material material;
    glm_vec3_copy(ambient, material.ambient);
    material.roughness = roughness;
    material.metallic = metallic;
    material.reflection = reflection;

    loci_set(entity, Loci_Material, material);
}

void loci_destroyMaterial(ecs_entity_t entity)
{
    loci_remove(entity, Loci_Material)
}
