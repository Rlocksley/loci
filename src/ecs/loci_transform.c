#include "loci_transform.h"

void loci_createTransform
(ecs_entity_t entity,
vec3 scale, float angle, vec3 axis, vec3 translation)
{
    Loci_Transform transform;

    glm_mat4_identity(transform.transform);
    glm_scale(transform.transform, scale);
    glm_rotate(transform.transform, angle, axis);
    glm_translate(transform.transform, translation);

    loci_set(entity, Loci_Transform, transform);
}

void loci_createTransform2(ecs_entity_t entity, mat4 transform)
{
    Loci_Transform _transform;

    glm_mat4_copy(transform, _transform.transform);

    loci_set(entity, Loci_Transform, _transform);
}

void loci_destroyTransform(ecs_entity_t entity)
{
    loci_remove(entity, Loci_Transform);
}

