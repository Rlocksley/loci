#include "snake.h"

SnakeMesh createSnakeMesh
(uint32_t numberSections, 
float space, float width, float height, float depth, vec3 color)
{
    Loci_Cube cube = loci_createCube(width, height, depth, color);

    SnakeMesh snake;
    snake.numberVertices = numberSections*24;
    snake.pVertices = (Loci_Vertex*) malloc(sizeof(Loci_Vertex)*numberSections*24);
    if(snake.pVertices == NULL)
    {
        LOCI_LOGI("createSnakeMesh", "malloc() for pVertices", "failed")
    }
    snake.numberIndices = numberSections*36;
    snake.pIndices = (uint32_t*) malloc(sizeof(uint32_t)*numberSections*36);
    if(snake.pIndices == NULL)
    {
        LOCI_LOGI("createSnakeMesh", "malloc() for pIndices", "failed")
    } 

    for(uint32_t i = 0; i < numberSections; i++)
    {
        for(uint32_t j = 0; j < 24; j++)
        {
            snake.pVertices[i*24+j] = cube.pVertices[j];
        }

        for(uint32_t j = 0; j < 24; j++)
        {
            glm_vec3_add
            (cube.pVertices[j].position, (vec3){0.f,0.f,space+depth}, 
            cube.pVertices[j].position);
        }
        
        for(uint32_t j = 0; j < 36; j++)
        {
            snake.pIndices[i*36+j] = cube.pIndices[j];
        }

        for(uint32_t j = 0; j < 36; j++)
        {
            cube.pIndices[j] += 24;
        }
    }

    loci_destroyCube(cube);

    return snake;
}

void destroySnakeMesh(SnakeMesh mesh)
{
    free(mesh.pIndices);
    free(mesh.pVertices);
}


SnakeSkeleton createSnakeSkeleton
(uint32_t numberSections, 
float space,  float width, float height, float depth)
{
    Loci_Cube cube = loci_createCube(width, height, depth, (vec3){1.f,1.f,1.f});

    SnakeSkeleton snake;
    snake.numberVertices = numberSections*24;
    snake.pVertices = (Loci_SkeletonVertex*) malloc(sizeof(Loci_SkeletonVertex)*numberSections*24);
    if(snake.pVertices == NULL)
    {
        LOCI_LOGE("createSnakeSkeleton", "malloc() for pVertices", "failed")
    }

    for(uint32_t i = 0; i < numberSections; i++)
    {
        for(uint32_t j = 0; j < 24; j++)
        {
            glm_vec3_copy
            (cube.pVertices[j].position,
            snake.pVertices[i*24+j].position);

            glm_vec3_copy
            (cube.pVertices[j].normal,
            snake.pVertices[i*24+j].normal);

            snake.pVertices[i*24+j].boneNumbers[0] = i;

            snake.pVertices[i*24+j].numberBones[0] = 1;

            snake.pVertices[i*24+j].weights[0] = 1.f;
        }

        for(uint32_t j = 0; j < 24; j++)
        {
            glm_vec3_add
            (cube.pVertices[j].position, (vec3){0.f,0.f,space+depth}, 
            cube.pVertices[j].position);
        }
    }

    loci_destroyCube(cube);

    return snake;
}

void destroySnakeSkeleton(SnakeSkeleton skeleton)
{
    free(skeleton.pVertices);
}

void createSnakeBones
(ecs_entity_t entity, 
uint32_t numberSections, 
float space,  float width, float height, float depth)
{
    mat4 transform;
    glm_mat4_identity(transform);
    vec3 position;
    glm_vec3_copy((vec3){0.f,0.f,depth/2.f}, position);
    glm_translate(transform, position);
    mat4 inverse;
    glm_mat4_inv(transform, inverse);
    
    ecs_entity_t child = loci_createEntity()
    loci_addPair(child, EcsChildOf, entity);
    loci_createBone(child, 0, inverse);

    glm_vec3_copy((vec3){0.f,0.f,space+depth}, position);

    ecs_entity_t parent = child;        
    for(uint32_t i = 1; i < numberSections; i++)
    {
        glm_translate(transform, position);
        glm_mat4_inv(transform, inverse);

        child = loci_createEntity()
        loci_addPair(child, EcsChildOf, parent);
        loci_createBone(child, i, inverse);

        parent = child;
    }
}

void destroySnakeBones(ecs_entity_t entity)
{
    ecs_iter_t it = ecs_children(loci_pWorld, entity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            loci_removePair(it.entities[i], EcsChildOf, entity)
            destroySnakeBones2(it.entities[i]);
        }
    }
}

void destroySnakeBones2(ecs_entity_t entity)
{
    ecs_iter_t it = ecs_children(loci_pWorld, entity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            loci_removePair(it.entities[i], EcsChildOf, entity)
            destroySnakeBones2(it.entities[i]);
        }
    }
    
    loci_remove(entity, Loci_Bone);
    ecs_delete(loci_pWorld, entity);
}



void createSnakeAnimation1
(ecs_entity_t entity, ecs_entity_t animationEntity,
uint32_t numberSections,
float space, float depth,
float bending, float scratching, float radius, float timeStepSize, 
uint32_t numberKeyFrames, uint32_t boneLevel)
{
    ecs_iter_t it = ecs_children(loci_pWorld, entity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            Loci_KeyFrameScale* pScales = (Loci_KeyFrameScale*) malloc(sizeof(Loci_KeyFrameScale));
            if(pScales == NULL)
            {
                LOCI_LOGE("createSnakeAnimation1", "malloc() for pScales", "failed")
            }
            glm_vec3_copy((vec3){1.f,1.f,1.f}, pScales[0].scale);
            pScales[0].time = 0.f;

            Loci_KeyFrameRotation* pRotations = (Loci_KeyFrameRotation*) malloc(sizeof(Loci_KeyFrameRotation)*numberKeyFrames);
            if(pRotations == NULL)
            {
                LOCI_LOGE("createSnakeAnimation1", "malloc() for pRotations", "failed")
            }
            for(uint32_t j = 0; j < numberKeyFrames; j++)
            {
                glm_quatv
                (pRotations[j].rotation, 
                bending*sin(2*GLM_PI*(j*timeStepSize + ((float)boneLevel)/((float)numberSections))), 
                (vec3){0.f, 1.f, 0.f});
                pRotations[j].time = j*timeStepSize;
            }

            Loci_KeyFramePosition* pPositions = (Loci_KeyFramePosition*) malloc(sizeof(Loci_KeyFramePosition)*numberKeyFrames);
            if(pPositions == NULL)
            {
                LOCI_LOGE("createSnakeAnimation1", "malloc() for pPositions", "failed")
            }
            if(boneLevel == 0)
            {
                for(uint32_t j = 0; j < numberKeyFrames; j++)
                {
                    glm_vec3_copy
                    ((vec3){radius*-cos(2*GLM_PI*j*timeStepSize), 
                    0.f, depth/2.f},
                    pPositions[j].position);
                    pPositions[j].time = j*timeStepSize;
                }
            
            }
            else
            {   
                for(uint32_t j = 0; j < numberKeyFrames; j++)
                {
                    glm_vec3_copy
                    ((vec3){0.f, 
                    0.f, space+depth},
                    pPositions[j].position);
                    pPositions[j].time = j*timeStepSize;
                }
            }

            loci_createChannel
            (it.entities[i], animationEntity,
            pScales,1,
            pRotations, numberKeyFrames,
            pPositions, numberKeyFrames);

            createSnakeAnimation1
            (it.entities[i], animationEntity,
            numberSections,
            space, depth, 
            bending, scratching, radius, timeStepSize,
            numberKeyFrames, boneLevel+1);
        }
    }
}

void destroySnakeAnimation1_1
(ecs_entity_t entity, ecs_entity_t animationEntity)
{
    ecs_iter_t it = ecs_children(loci_pWorld, entity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            destroySnakeAnimation1_2(it.entities[i], animationEntity);
        }
    }
}

void destroySnakeAnimation1_2
(ecs_entity_t entity, ecs_entity_t animationEntity)
{
    ecs_iter_t it = ecs_children(loci_pWorld, entity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            destroySnakeAnimation1_2(it.entities[i], animationEntity);
        }
    }

    loci_destroyChannel(entity, animationEntity);
}

