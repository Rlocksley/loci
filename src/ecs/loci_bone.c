#include "loci_bone.h"

void loci_createBone(ecs_entity_t entity, uint32_t index, mat4 inverse)
{
    Loci_Bone bone;
    bone.index = index;
    glm_mat4_copy(inverse, bone.inverse);

    loci_set(entity, Loci_Bone, bone);
}

void loci_destroyBone(ecs_entity_t entity)
{
    loci_remove(entity, Loci_Bone);
}