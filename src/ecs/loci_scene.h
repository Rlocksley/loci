#ifndef LOCI_SCENE_H
#define LOCI_SCENE_H

#include "../core/loci_acceleration.h"
#include "../core/loci_descriptorSet.h"
#include "../core/loci_pipeline.h"
#include "../core/loci_shaderTables.h"
#include "../core/loci_world.h"
#include "loci_transform.h"
#include "loci_mesh.h"
#include "loci_texture.h"
#include "loci_material.h"
#include "loci_pointLight.h"

typedef struct Loci_Object
{
    uint64_t vertexBufferAddress;
    uint64_t indexBufferAddress;
    int32_t textureIndex;
    vec3 ambient;
    float roughness;
    float metallic;
    float reflection;
}Loci_Object;

typedef struct Loci_PointLightObject
{
    vec4 color;
    vec4 translation;
}Loci_PointLightObject;

typedef struct Loci_Scene
{

    uint32_t numberInstances;
    Loci_BufferInterface objectBuffer;
    Loci_BufferInterface instanceBuffer;
    uint32_t numberImages;
    Loci_Image *pImages;
    uint32_t numberPointLights;
    Loci_BufferInterface pointLightBuffer;


    Loci_TopAcceleration acceleration;
    Loci_RayDescriptorSet descriptorSet;
    uint32_t maxRecursionDepth;
    Loci_RayPipeline pipeline;
    uint32_t numberMissShaders;
    uint32_t numberClosestHitShaders;
    Loci_ShaderTables shaderTables;
    uint32_t recordSize;

    ecs_query_t* colorMeshInstanceUpdateQuery;
    ecs_query_t* textureMeshInstanceUpdateQuery;
    ecs_query_t* pointLightUpdateQuery;

    bool spawnOperationDirty;
}Loci_Scene;

void loci_createScene(ecs_entity_t entity,
uint32_t maxNumberMeshInstances,
uint32_t maxNumberTexturesInstances,
Loci_Shader generationShader,
Loci_Shader missShaders[], uint32_t numberMissShaders,
Loci_Shader closestHitShaders[], uint32_t numberClosestHitShaders,
uint32_t maxRecursionDepth);
void loci_updateScene(ecs_entity_t entity);
void loci_spawnInScene(ecs_entity_t entity, ecs_entity_t scene);
void loci_despawnInScene(ecs_entity_t entity, ecs_entity_t scene);
void loci_drawScene(ecs_entity_t entity);

#endif