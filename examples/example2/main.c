#ifndef LOCI_CORE_H
#define LOCI_CORE_H
#include "../../src/core/loci_core.h"
#endif
#ifndef LOCI_ECS_H
#define LOCI_ECS_H
#include "../../src/ecs/loci_ecs.h"
#endif
#ifndef LOCI_SPHERE_H
#define LOCI_SPHERE_H
#include "../../src/shapes/loci_sphere.h"
#endif
#ifndef LOCI_TETRA_H
#define LOCI_TETRA_H
#include "../../src/shapes/loci_tetra.h"
#endif
#ifndef LOCI_CUBE_H
#define LOCI_CUBE_H
#include "../../src/shapes/loci_cube.h"
#endif
#ifndef LOCI_GLTF_H
#define LOCI_GLTF_H
#include "../../src/gltf/loci_gltf.h"
#endif

#define random() ((float)rand())/((float)RAND_MAX)

int main()
{
    loci_windowWidth = 1080;
    loci_windowHeight = 720;
    loci_windowTitle = "Example2";
    loci_minDeltaTime = 0.001f;
    loci_maxDeltaTime = 0.02f;
    loci_keyPressTime = 0.5f;
    loci_applicationName = "Example2";
    loci_maxNumberUniformBuffers = 30;
    loci_maxNumberStorageBuffers = 30;
    loci_maxNumberCombinedImageSamplers = 10;
    loci_maxNumberStorageImages = 10;
    loci_maxNumberTopAccelerations = 1;
    loci_maxNumberDescriptorSets = 10;

    loci_createCore();

    Loci_Shader genShader =  
    loci_createShader("../../src/shaders/genShader.spv");
    Loci_Shader missShader[3] = 
    {loci_createShader("../../src/shaders/missShader.spv"),
    loci_createShader("../../src/shaders/missShadowShader.spv"),
    loci_createShader("../../src/shaders/missReflectionShader.spv")};
    Loci_Shader chitShader[3] = 
    {loci_createShader("../../src/shaders/chitShader.spv"),
    loci_createShader("../../src/shaders/chitShadowShader.spv"),
    loci_createShader("../../src/shaders/chitReflectionShader.spv")};

    Loci_FlyingCamera camera = 
    loci_createFlyingCamera
    ((vec3){0.f,0.f,0.f}, (vec2){0.f,0.f},
    10.f, 0.01f, 3.14f/3.f, 0.01f, 100000.f);

    //Scene
    ecs_entity_t scene = loci_createEntity()
    loci_createScene(scene,
    10001,10001,
    genShader, missShader, 3, chitShader, 3, 4);

    mat4 identity;
    glm_mat4_identity(identity);
    
    ecs_entity_t sky = loci_createEntity();
    loci_createTransform(sky, 
    (vec3){1.f,1.f,1.f}, 1.f, (vec3){0.f,1.f,0.f},
    (vec3){0.f,0.f,0.f});
    loci_createMaterial(sky, (vec3){0.5f,0.5f,0.5f}, 0.2f, 0.2f, 0.f);
    Loci_Sphere shape0 = loci_createSphereIn(40000, 10);
    loci_createMesh(sky,
    shape0.pVertices,shape0.numberVertices,
    shape0.pIndices, shape0.numberIndices,
    identity);
    loci_createTexture(sky, "filePath/to/your/texture");


    

    ecs_entity_t light = loci_createEntity()
    loci_createTransform(light, (vec3){1.f,1.f,1.f}, 0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, -100.f, 0.f});
    loci_createPointLight(light, (vec3){1.f,1.f,1.f});

    ecs_entity_t light2 = loci_createEntity()
    loci_createTransform(light2, (vec3){1.f,1.f,1.f}, 0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, -2.f, -0.f});
    loci_createPointLight(light2, (vec3){1.f,1.f,1.f});

    ecs_entity_t model = loci_createEntity()
    loci_createMeshModel(model,"filePath/to/your/GTLF/Directory/", 
    "filePath/to/your/GTLF/Directory/file.gltf");

 


    //spawn
    loci_spawnInScene(sky, scene);
    loci_spawnInScene(light, scene);
    loci_spawnInScene(light2, scene);
    loci_spawnMeshModelInScene(model, scene);

    printf("before Loop\n");
    bool isLight2 = false;

    //Loop
    while(loci_updateCore())
    {
        loci_getSwapchainImageIndex();
        loci_updateFlyingCamera(&camera);
        loci_updateScene(scene);
        loci_drawScene(scene);
    }

    loci_destroyScene(scene);

    loci_destroyMeshModel(model);

    loci_destroyTexture(sky);
    loci_destroyMesh(sky);
    loci_destroyTransform(sky);

    loci_destroyCore();
}