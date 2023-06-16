#include "loci_pointLight.h"

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

