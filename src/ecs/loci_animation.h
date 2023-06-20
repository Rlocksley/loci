#ifndef LOCI_ANIMATION_H
#define LOCI_ANIMATION_H

#include "../core/loci_world.h"

typedef struct Loci_Animation
{
    ecs_entity_t entity;
    ecs_entity_t animation;
    float time;
}Loci_Animation;

void loci_createAnimation(ecs_entity_t entity, ecs_entity_t animationEntity, float time);
void loci_destroyAnimation(ecs_entity_t entity);

#endif