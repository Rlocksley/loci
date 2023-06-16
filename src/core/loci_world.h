#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H

#include "./flecs/flecs.h"

//flec world
ecs_world_t* loci_pWorld;

//Pairs
ecs_entity_t loci_spawnedIn;
bool loci_spawned = false;
ecs_entity_t loci_isModelOfMesh;
ecs_entity_t loci_isModelOfTexture;

#define loci_createWorld()\
{\
    loci_pWorld = ecs_init();\
    loci_spawnedIn = ecs_new_id(loci_pWorld);\
    loci_isModelOfMesh = ecs_new_id(loci_pWorld);\
    loci_isModelOfTexture = ecs_new_id(loci_pWorld);\
}

#define loci_destroyWorld() ecs_fini(loci_pWorld);

#define loci_createEntity() ecs_new_id(loci_pWorld);

#define loci_destroyEntity(entity) ecs_delete(loci_pWorld, entity);

#define loci_spawn(entity)\
{\
    ecs_add_id(loci_pWorld, entity, loci_spawnedTag);\
    loci_spawned = true;\
}

#define loci_addTag(entity, tagEntity)\
{\
    ecs_add_id(loci_pWorld, entity, tagEntity);\
}

#define loci_removeTag(entity, tagEntity)\
{\
    ecs_remove_id(loci_pWorld, entity, tagEntity);\
}

#define loci_set(entity, componentType, componentValue) \
{\
    ECS_COMPONENT(loci_pWorld, componentType);\
\
    ecs_set_id(loci_pWorld, entity, ecs_id(componentType), sizeof(componentType), &componentValue);\
}

#define loci_get(entity, componentType, pComponentValue)\
{\
    ECS_COMPONENT(loci_pWorld, componentType);\
\
    pComponentValue = ecs_get(loci_pWorld, entity, componentType);\
}

#define loci_get_mut(entity, componentType, pComponentValue)\
{\
    ECS_COMPONENT(loci_pWorld, componentType);\
\
    pComponentValue = ecs_get_mut(loci_pWorld, entity, componentType);\
}

#define loci_remove(entity, componentType)\
{\
    ECS_COMPONENT(loci_pWorld, componentType);\
\
    ecs_remove(loci_pWorld, entity, componentType);\
}

#define loci_addPair(entity0, pairEntity, entity1)\
{\
    ecs_add_pair(loci_pWorld, entity0, pairEntity, entity1);\
}

#define loci_removePair(entity0, pairEntity, entity1)\
{\
    ecs_remove_pair(loci_pWorld, entity0, pairEntity, entity1);\
}

#endif