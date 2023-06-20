#include "../../src/core/loci_core.h"
#include "../../src/ecs/loci_ecs.h"
#include "../../src/shapes/loci_sphere.h"
#include "../../src/shapes/loci_cube.h"

#define random() ((float)rand())/((float)RAND_MAX)

int main()
{
    loci_windowWidth = 1080;
    loci_windowHeight = 720;
    loci_windowTitle = "Example1";
    loci_minDeltaTime = 0.001f;
    loci_maxDeltaTime = 0.02f;
    loci_keyPressTime = 0.5f;
    loci_applicationName = "Example1";
    loci_maxNumberUniformBuffers = 30;
    loci_maxNumberStorageBuffers = 30;
    loci_maxNumberCombinedImageSamplers = 10;
    loci_maxNumberStorageImages = 10;
    loci_maxNumberTopAccelerations = 1;
    loci_maxNumberDescriptorSets = 10;
    loci_pShaderDirectory = "/path/to/your/shader/directory";

    loci_createCore();

    Loci_Shader genShader =  
    loci_createShader("genShader.spv");
    Loci_Shader missShader[3] = 
    {loci_createShader("missShader.spv"),
    loci_createShader("missShadowShader.spv"),
    loci_createShader("missReflectionShader.spv")};
    Loci_Shader chitShader[3] = 
    {loci_createShader("chitShader.spv"),
    loci_createShader("chitShadowShader.spv"),
    loci_createShader("chitReflectionShader.spv")};

    Loci_FlyingCamera camera = 
    loci_createFlyingCamera
    ((vec3){0.f,0.f,-10.f}, (vec2){0.f,0.f},
    10.f, 0.01f, 3.14f/3.f, 0.01f, 100000.f);

    //Scene
    uint32_t numberCubes = 100;
    
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
    
    loci_createTexture(sky, "/your/texture/path");

    ecs_entity_t baseCube = loci_createEntity();
    Loci_Cube cubeShape = loci_createCube(10, 10, 10, (vec3){0.8f, 0.8f, 0.8f});
    loci_createMaterial(baseCube,(vec3){0.8,0.9,0.9}, 1.f, 1.f, 0.8f);
    loci_createMesh(baseCube,
    cubeShape.pVertices, cubeShape.numberVertices,
    cubeShape.pIndices, cubeShape.numberIndices,
    identity);
    loci_createTexture(baseCube, "/your/texture/path");

    srand(time(NULL));
    ecs_entity_t cubes[numberCubes][numberCubes];
    for(uint32_t i = 0; i < numberCubes; i++)
    {

        for(uint32_t j = 0; j < numberCubes; j++)
        {

            cubes[i][j] = ecs_new_w_pair(loci_pWorld, EcsIsA, baseCube);

            loci_createTransform(cubes[i][j],
            (vec3){1.f,1.f*random(),1.f}, 0.f, (vec3){0.f,1.f,0.f},
            (vec3){20.f*i,200.f,20.f*j});

        }
    }


    ecs_entity_t light = loci_createEntity()
    loci_createTransform(light, (vec3){1.f,1.f,1.f}, 0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, -200.f, 0.f});
    loci_createPointLight(light, (vec3){1.f,1.f,1.f});

    ecs_entity_t light2 = loci_createEntity()
    loci_createTransform(light2, (vec3){1.f,1.f,1.f}, 0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, 2000.f, 400.f});
    loci_createPointLight(light2, (vec3){1.f,1.f,1.f});

    ecs_entity_t scene = loci_createEntity()
    loci_createScene(scene,
    10001,10001, 0,
    genShader, missShader, 3, chitShader, 3, 4);


    //spawn
    loci_spawnInScene(sky, scene);

    for(uint32_t i = 0; i < numberCubes; i++)
    {
        for(uint32_t j = 0; j < numberCubes; j++)
        {
            loci_spawnInScene(cubes[i][j], scene);
        }
    }
    loci_spawnInScene(light, scene);

    bool isLight2 = false;

    //Loop
    while(loci_updateCore())
    {
        loci_getSwapchainImageIndex();
        if(loci_keyX.pressed && !isLight2)
        {
            loci_despawnInScene(light, scene);
            loci_spawnInScene(light2, scene);
            isLight2 = true;
        }
        else if(loci_keyX.pressed && isLight2)
        {
            loci_despawnInScene(light2, scene);
            loci_spawnInScene(light, scene);
            isLight2 = false;
        }
        
        loci_updateFlyingCamera(&camera);
        loci_updateScene(scene);
        loci_drawScene(scene);
    }

    loci_destroyScene(scene);

    for(uint32_t i = 0; i < numberCubes; i++)
    {
        for(uint32_t j = 0; j < numberCubes; j++)
        {
            loci_destroyTransform(cubes[i][j]);
        }
    }

    loci_destroyTexture(baseCube);
    loci_destroyMesh(baseCube);
    loci_destroyCube(cubeShape);

    loci_destroyTexture(sky);
    loci_destroyMesh(sky);
    loci_destroyTransform(sky);

    for(uint32_t i = 0; i < 3; i++)
    {
        loci_destroyShader(chitShader[i]);
        loci_destroyShader(missShader[i]);
    }
    loci_destroyShader(genShader);

    loci_destroyCore();
}