#include "../../src/core/loci_core.h"
#include "../../src/ecs/loci_ecs.h"
#include "../../src/shapes/loci_sphere.h"
#include "snake.h"


#define random() ((float)rand())/((float)RAND_MAX)

int main()
{
    loci_windowWidth = 1080;
    loci_windowHeight = 720;
    loci_windowTitle = "Example3";
    loci_minDeltaTime = 0.001f;
    loci_maxDeltaTime = 0.02f;
    loci_keyPressTime = 0.5f;
    loci_applicationName = "Example3";
    loci_maxNumberUniformBuffers = 30;
    loci_maxNumberStorageBuffers = 30;
    loci_maxNumberCombinedImageSamplers = 10;
    loci_maxNumberStorageImages = 10;
    loci_maxNumberTopAccelerations = 1;
    loci_maxNumberDescriptorSets = 10;
    loci_pShaderDirectory = "/home/robin/Desktop/Github/loci/src/shaders";

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
    ((vec3){0.f,-50.f,1000.f}, (vec2){GLM_PI,0.f},
    100.f, 0.01f, 3.14f/3.f, 0.01f, 100000.f);

    //Scene
    ecs_entity_t scene = loci_createEntity()
    loci_createScene(scene,
    10001,10001,1,
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
    loci_createTexture(sky, "/home/robin/Desktop/Github/Loki/src/test/Bronze03_1K_BaseColor.png");
    loci_spawnInScene(sky, scene);

    ecs_entity_t light = loci_createEntity()
    loci_createTransform(light, (vec3){1.f,1.f,1.f}, 0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, -100.f, -1000.f});
    loci_createPointLight(light, (vec3){0.6f,0.6f,0.6f});
    loci_spawnInScene(light, scene);


    ecs_entity_t floor = loci_createEntity()
    loci_createTransform(floor, (vec3){1.f,1.f,1.f}, 
    0.f, (vec3){0.f,1.f,0.f},(vec3){0.f, 50.f, 0.f}); 
    Loci_Cube cube = loci_createCube(3000.f, 100.f, 3000.f, (vec3){1.f,1.f,1.f});
    loci_createMesh(floor, 
    cube.pVertices, cube.numberVertices, cube.pIndices, cube.numberIndices, identity);
    loci_createMaterial(floor, (vec3){0.5f,0.5f,0.5f}, 0.1f, 0.8f, 0.5f);
    loci_createTexture(floor, "/home/robin/Desktop/Github/Loki/src/test/rustediron1-alt2-bl/rustediron2_basecolor.png");
    loci_spawnInScene(floor, scene);

    ecs_entity_t snake = loci_createEntity();
    loci_createTransform2(snake, identity);
    SnakeMesh mesh = createSnakeMesh(30, 1, 5, 5, 5, (vec3){0.6,0.6,0.6});
    loci_createMesh(snake, mesh.pVertices, mesh.numberVertices, 
    mesh.pIndices, mesh.numberIndices, identity);
    loci_createMaterial(snake, (vec3){0.5f, 0.5f, 0.5f}, 0.1f, 0.8f, 0.3f);    
    SnakeSkeleton skeleton = createSnakeSkeleton(30, 1, 5, 5, 5);
    loci_createSkeleton(snake, skeleton.pVertices, skeleton.numberVertices, identity);
    createSnakeBones(snake, 30, 1, 5, 5, 5);
    ecs_entity_t animation = loci_createEntity()
    createSnakeAnimation1(snake, animation, 30, 1.f, 5.f, 0.1f, 1.f, 30.f, 0.1f, 100, 0);
    loci_createAnimation(snake, animation, 0.f);
    loci_spawnInScene(snake, scene);

    float animationTime = 0.f;
    float animationScale = 0.3f;
    float speed = 20.f;
    mat4 transform;
    glm_mat4_identity(transform);
    while(loci_updateCore())
    {
        loci_getSwapchainImageIndex();
        loci_updateFlyingCamera(&camera);
        
        glm_translate(transform, (vec3){0.f,0.f,speed*loci_deltaTime});
        loci_createTransform2(snake, transform);

        animationTime = fmodf(animationTime+loci_deltaTime*animationScale, 9.f);
        loci_createAnimation(snake, animation, animationTime);

        loci_updateScene(scene);
        loci_drawScene(scene);
    }

    loci_destroyScene(scene);

    loci_destroyAnimation(snake);
    destroySnakeAnimation1_1(snake, animation);
    destroySnakeBones(snake);
    loci_destroySkeleton(snake);
    loci_destroyMaterial(snake);
    loci_destroyMesh(snake);
    loci_destroyTransform(snake);
    destroySnakeMesh(mesh);

    loci_destroyTexture(floor);
    loci_destroyMaterial(floor);
    loci_destroyMesh(floor);
    loci_destroyTransform(floor);


    loci_destroyPointLight(light);
    loci_destroyTransform(light);

    loci_destroyTexture(sky);
    loci_destroyMesh(sky);
    loci_destroyMaterial(sky);
    loci_destroyTransform(sky);

    for(uint32_t i = 0; i < 3; i++)
    {
        loci_destroyShader(chitShader[i]);
        loci_destroyShader(missShader[i]);
    }
    loci_destroyShader(genShader);

    loci_destroyCore();
}