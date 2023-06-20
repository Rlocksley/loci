#ifndef LOCI_BONE_H
#define LOCI_BONE_H

#include "../core/loci_global.h"
#include "../core/loci_world.h"

typedef struct Loci_Bone
{
    uint32_t index;
    mat4 inverse;
}Loci_Bone;

void loci_createBone(ecs_entity_t entity, uint32_t index, mat4 inverse);
void loci_destroyBone(ecs_entity_t entity);

#endif