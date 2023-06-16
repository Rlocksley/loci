#ifndef LOCI_GLTF_H
#define LOCI_GLTF_H

#include "loci_transform.h"
#include "../ecs/loci_mesh.h"
#include "../ecs/loci_texture.h"

#define CGLTF_IMPLEMENTATION
#include "./cgltf/cgltf.h"

void loci_createMeshModel(ecs_entity_t entity, const char* pDirectoryFilePath, const char* pFilePath);
void loci_spawnMeshModelInScene(ecs_entity_t entity, ecs_entity_t scene);
void loci_despawnMeshModelInScene(ecs_entity_t entity, ecs_entity_t scene);
void loci_destroyMeshModel(ecs_entity_t entity);


#endif