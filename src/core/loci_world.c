#include "loci_world.h"

//flec world
ecs_world_t* loci_pWorld;

//Pairs
ecs_entity_t loci_spawnedIn;
bool loci_spawned = false;
ecs_entity_t loci_isModelOfMesh;
ecs_entity_t loci_isModelOfTexture;
