#include "loci_scene.h"


void loci_createScene(ecs_entity_t entity,
uint32_t maxNumberInstances,
uint32_t maxNumberImages,
Loci_Shader generationShader,
Loci_Shader missShaders[], uint32_t numberMissShaders,
Loci_Shader closestHitShaders[], uint32_t numberClosestHitShaders,
uint32_t maxRecursionDepth)
{
    Loci_Scene scene;
    uint32_t handleSize = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleSize;
    uint32_t alignment = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleAlignment;
    scene.recordSize = (handleSize + alignment - 1) & ~(alignment - 1);
    

    scene.maxRecursionDepth = maxRecursionDepth;
    scene.spawnOperationDirty = false;

    scene.numberMissShaders = numberMissShaders;
    scene.numberClosestHitShaders = numberClosestHitShaders;

    scene.numberInstances = 0;
    scene.numberImages = 0;

    scene.objectBuffer =
    loci_createBufferInterface
    (maxNumberInstances, sizeof(Loci_Object),
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    scene.instanceBuffer =
    loci_createBufferInterface
    (maxNumberInstances, sizeof(VkAccelerationStructureInstanceKHR),
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT |
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_BUILD_INPUT_READ_ONLY_BIT_KHR);

    scene.pImages = (Loci_Image *)malloc(maxNumberImages * sizeof(Loci_Image));
    if (scene.pImages == NULL)
    {
        LOCI_LOGE("createSceneContainer", "malloc() for pImages", "failed")
    }

    scene.pointLightBuffer =
    loci_createBufferInterface
    (16, sizeof(Loci_PointLightObject),
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    printf("before topacceleration\n");
    scene.acceleration =
    loci_createTopAcceleration(scene.instanceBuffer, maxNumberInstances);

    scene.descriptorSet =
    loci_createRayDescriptorSet(maxNumberImages);

    scene.pipeline =
    loci_createRayPipeline
    (scene.descriptorSet, generationShader,
    missShaders, numberMissShaders,
    closestHitShaders, numberClosestHitShaders,
    maxRecursionDepth);

    scene.shaderTables =
    loci_createShaderTables(scene.pipeline, numberMissShaders, numberClosestHitShaders);

    ECS_COMPONENT(loci_pWorld, Loci_Transform);
    ECS_COMPONENT(loci_pWorld, Loci_Mesh);
    ECS_COMPONENT(loci_pWorld, Loci_Material);
    ECS_COMPONENT(loci_pWorld, Loci_Texture);
    ECS_COMPONENT(loci_pWorld, Loci_PointLight);

    scene.colorMeshInstanceUpdateQuery =
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms =
        {
            {ecs_id(Loci_Transform)},
            {ecs_id(Loci_Mesh)},
            {ecs_id(Loci_Material)},
            {ecs_id(Loci_Texture), .oper = EcsNot},
            {.id = ecs_pair(loci_spawnedIn, entity)}

        }
    });

    scene.textureMeshInstanceUpdateQuery =
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms =
        {
            {ecs_id(Loci_Transform)},
            {ecs_id(Loci_Mesh)},
            {ecs_id(Loci_Material)},
            {ecs_id(Loci_Texture)},
            {.id = ecs_pair(loci_spawnedIn, entity)}
            
        }
    });

    scene.pointLightUpdateQuery =
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms = 
        {
            {ecs_id(Loci_Transform)},
            {ecs_id(Loci_PointLight)},
            {.id = ecs_pair(loci_spawnedIn, entity)}
        }
    });

    loci_set(entity, Loci_Scene, scene);
}

void loci_destroyScene(ecs_entity_t entity)
{
    const Loci_Scene *pScene;
    loci_get(entity, Loci_Scene, pScene);
    Loci_Scene scene = *pScene;

    ecs_query_fini(scene.textureMeshInstanceUpdateQuery);

    ecs_query_fini(scene.colorMeshInstanceUpdateQuery);

    loci_destroyShaderTables(scene.shaderTables);

    loci_destroyRayPipeline(scene.pipeline);

    loci_destroyRayDescriptorSet(scene.descriptorSet);

    loci_destroyTopAcceleration(scene.acceleration);

    loci_destroyBufferInterface(scene.pointLightBuffer);

    free(scene.pImages);

    loci_destroyBufferInterface(scene.instanceBuffer);

    loci_destroyBufferInterface(scene.objectBuffer);
}

void loci_updateScene(ecs_entity_t entity)
{
    const Loci_Scene *pScene;
    loci_get(entity, Loci_Scene, pScene);
    Loci_Scene scene = *pScene;

    uint32_t numberInstances = 0;
    VkAccelerationStructureInstanceKHR *pInstances =
    (VkAccelerationStructureInstanceKHR *)scene.instanceBuffer.pMemory;
    Loci_Object *pObjects =
    (Loci_Object *)scene.objectBuffer.pMemory;
    Loci_Image *pImages = scene.pImages;
    
    ECS_COMPONENT(loci_pWorld, Loci_Transform);
    ECS_COMPONENT(loci_pWorld, Loci_Mesh);
    ECS_COMPONENT(loci_pWorld, Loci_Material);
    ECS_COMPONENT(loci_pWorld, Loci_Texture);
    ECS_COMPONENT(loci_pWorld, Loci_PointLight);

    ecs_iter_t colorMeshIt = ecs_query_iter(loci_pWorld, scene.colorMeshInstanceUpdateQuery);
    while (ecs_query_next(&colorMeshIt))
    {
        Loci_Transform* transforms = ecs_field(&colorMeshIt, Loci_Transform, 1);
        Loci_Mesh* meshes = ecs_field(&colorMeshIt, Loci_Mesh, 2);
        Loci_Material* materials = ecs_field(&colorMeshIt, Loci_Material, 3);

        for (int i = 0; i < colorMeshIt.count; i++)
        {
            VkTransformMatrixKHR matrix;
            for (uint32_t j = 0; j < 3; j++)
            {
                for (uint32_t k = 0; k < 4; k++)
                {
                    matrix.matrix[j][k] = transforms[i].transform[k][j];
                }
            }
            
            
            Loci_Object object;
            object.vertexBufferAddress = meshes[i].vertexBuffer.deviceAddress;
            object.indexBufferAddress = meshes[i].indexBuffer.deviceAddress;
            object.textureIndex = -1;
            glm_vec3_copy( materials[i].ambient, object.ambient);
            object.roughness = materials[i].roughness;
            object.metallic = materials[i].metallic;
            object.reflection = materials[i].reflection;
            
            pObjects[numberInstances] = object;

            VkAccelerationStructureInstanceKHR instance;
            instance.transform = matrix;
            instance.instanceCustomIndex = numberInstances;
            instance.mask = 0xFF;
            instance.instanceShaderBindingTableRecordOffset = 0;
            instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
            instance.accelerationStructureReference = meshes[i].acceleration.deviceAddress;

            pInstances[numberInstances++] = instance;
        }
    }

    uint32_t numberImages = 0;

    ecs_iter_t textureMeshIt = ecs_query_iter(loci_pWorld, scene.textureMeshInstanceUpdateQuery);
    while (ecs_query_next(&textureMeshIt))
    {
        Loci_Transform* transforms = ecs_field(&textureMeshIt, Loci_Transform, 1);
        Loci_Mesh* meshes = ecs_field(&textureMeshIt, Loci_Mesh, 2);
        Loci_Material* materials = ecs_field(&textureMeshIt, Loci_Material, 3);
        Loci_Texture* textures = ecs_field(&textureMeshIt, Loci_Texture, 4);

        for (int i = 0; i < textureMeshIt.count; i++)
        {
            VkTransformMatrixKHR matrix;
            for (uint32_t j = 0; j < 3; j++)
            {
                for (uint32_t k = 0; k < 4; k++)
                {
                    matrix.matrix[j][k] = transforms[i].transform[k][j];
                }
            }

            Loci_Object object;
            object.vertexBufferAddress = meshes[i].vertexBuffer.deviceAddress;
            object.indexBufferAddress = meshes[i].indexBuffer.deviceAddress;
            glm_vec3_copy( materials[i].ambient, object.ambient);
            object.roughness = materials[i].roughness;
            object.metallic = materials[i].metallic;
            object.reflection = materials[i].reflection;
            object.textureIndex = numberImages;

            pObjects[numberInstances] = object;

            pImages[numberImages++] = textures[i].image;

            VkAccelerationStructureInstanceKHR instance;
            instance.transform = matrix;
            instance.instanceCustomIndex = numberInstances;
            instance.mask = 0xFF;
            instance.instanceShaderBindingTableRecordOffset = 0;
            instance.flags = VK_GEOMETRY_INSTANCE_TRIANGLE_FACING_CULL_DISABLE_BIT_KHR;
            instance.accelerationStructureReference = meshes[i].acceleration.deviceAddress;

            pInstances[numberInstances++] = instance;
        }
    }
    
    uint32_t numberPointLights = 0;
    Loci_PointLightObject* pPointLightObjects = (Loci_PointLightObject*) scene.pointLightBuffer.pMemory;

    ecs_iter_t pointLightIt = ecs_query_iter(loci_pWorld, scene.pointLightUpdateQuery);
    while(ecs_query_next(&pointLightIt))
    {
        Loci_Transform* transforms = ecs_field(&pointLightIt, Loci_Transform, 1);
        Loci_PointLight* pointLights = ecs_field(&pointLightIt, Loci_PointLight, 2);

        for(int i = 0; i < pointLightIt.count; i++)
        {
            Loci_PointLightObject pointLightObject;
            glm_vec4_copy
            ((vec4){pointLights[i].color[0],
            pointLights[i].color[1],
            pointLights[i].color[2],0.f}, pointLightObject.color);
            glm_vec4_copy
            ((vec4){transforms[i].transform[3][0],
            transforms[i].transform[3][1],
            transforms[i].transform[3][2],0.f}, pointLightObject.translation);

            pPointLightObjects[numberPointLights++] = pointLightObject;
        }
    }

    printf("numberInstances:    %u\n", numberInstances);
    printf("numberImages:    %u\n", numberImages);
    printf("numberPointLights: %u\n", numberPointLights);

    scene.numberImages = numberImages;
    scene.numberInstances = numberInstances;
    scene.numberPointLights = numberPointLights;
    
    if (scene.spawnOperationDirty)
    {
    
        loci_destroyTopAcceleration(scene.acceleration);

        scene.acceleration = loci_createTopAcceleration
        (scene.instanceBuffer, scene.numberInstances);

        loci_updateTopAcceleration
        (scene.acceleration, 
        scene.instanceBuffer, scene.numberInstances,
        false, false);

        scene.descriptorSet = loci_updateRayDescriptorSet2
        (scene.descriptorSet, scene.acceleration,
        scene.objectBuffer, scene.numberInstances, 
        scene.pImages, scene.numberImages,
        scene.pointLightBuffer);

        scene.spawnOperationDirty = false;

        loci_set(entity, Loci_Scene, scene);
    }
    else
    {
        loci_updateTopAcceleration
        (scene.acceleration, 
        scene.instanceBuffer, scene.numberInstances,
        false, false);
    
        loci_updateRayDescriptorSet
        (scene.descriptorSet,
        scene.pImages, scene.numberImages);
    }
}

void loci_spawnInScene(ecs_entity_t entity, ecs_entity_t scene)
{
    loci_addPair(entity, loci_spawnedIn, scene);

    Loci_Scene* pScene;
    loci_get_mut(scene, Loci_Scene, pScene);
    pScene->spawnOperationDirty = true;
}

void loci_despawnInScene(ecs_entity_t entity, ecs_entity_t scene)
{
    loci_removePair(entity, loci_spawnedIn, scene);

    Loci_Scene* pScene;
    loci_get_mut(scene, Loci_Scene, pScene);
    pScene->spawnOperationDirty = true;
}

void loci_drawScene(ecs_entity_t entity)
{
    const Loci_Scene* pScene;
    loci_get(entity, Loci_Scene, pScene);
    Loci_Scene scene = *pScene;

    uint32_t handleSize = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleSize;
    uint32_t alignment = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleAlignment;
    uint32_t handleSizeAligned = (handleSize + alignment - 1) & ~(alignment - 1);

    VkStridedDeviceAddressRegionKHR genShaderEntry;
    genShaderEntry.deviceAddress = scene.shaderTables.genShaderTableBuffer.deviceAddress;
    genShaderEntry.stride = handleSizeAligned;
    genShaderEntry.size = handleSizeAligned;

    VkStridedDeviceAddressRegionKHR missShaderEntry;
    missShaderEntry.deviceAddress = scene.shaderTables.missShaderTableBuffer.deviceAddress;
    missShaderEntry.stride = handleSizeAligned;
    missShaderEntry.size = handleSizeAligned * scene.numberMissShaders;

    VkStridedDeviceAddressRegionKHR hitShaderEntry;
    hitShaderEntry.deviceAddress = scene.shaderTables.chitShaderTableBuffer.deviceAddress;
    hitShaderEntry.stride = handleSizeAligned;
    hitShaderEntry.size = handleSizeAligned * scene.numberClosestHitShaders;

    VkStridedDeviceAddressRegionKHR callableShaderEntry;
    callableShaderEntry.deviceAddress = 0;
    callableShaderEntry.stride = 0;
    callableShaderEntry.size = 0;

    VkImageMemoryBarrier barrier0;
    barrier0.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier0.srcAccessMask = 0;
    barrier0.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier0.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    barrier0.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier0.subresourceRange.aspectMask =  VK_IMAGE_ASPECT_COLOR_BIT;
    barrier0.subresourceRange.baseMipLevel = 0;
    barrier0.subresourceRange.levelCount = 1;
    barrier0.subresourceRange.baseArrayLayer = 0;
    barrier0.subresourceRange.layerCount = 1;
    barrier0.image = loci_swapchain.pVkImages[loci_swapchain.imageIndex];
    barrier0.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier0.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier0.pNext = NULL;

    VkImageMemoryBarrier barrier1;
    barrier1.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier1.srcAccessMask = 0;
    barrier1.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier1.oldLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier1.newLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier1.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier1.subresourceRange.baseMipLevel = 0;
    barrier1.subresourceRange.levelCount = 1;
    barrier1.subresourceRange.baseArrayLayer = 0;
    barrier1.subresourceRange.layerCount = 1;
    barrier1.image = loci_rayImage.vkImage;
    barrier1.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier1.pNext = NULL;

    VkImageCopy copy;
    copy.srcSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.srcSubresource.layerCount = 1;
    copy.srcSubresource.baseArrayLayer = 0;
    copy.srcSubresource.mipLevel = 0;
    copy.srcOffset.x = 0;
    copy.srcOffset.y = 0;
    copy.srcOffset.z = 0;
    copy.dstSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    copy.dstSubresource.layerCount = 1;
    copy.dstSubresource.baseArrayLayer = 0;
    copy.dstSubresource.mipLevel = 0;
    copy.dstOffset.x = 0;
    copy.dstOffset.y = 0;
    copy.dstOffset.z = 0;
    copy.extent.width = loci_windowWidth;
    copy.extent.height = loci_windowHeight;
    copy.extent.depth = 1;

    VkImageMemoryBarrier barrier2;
    barrier2.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier2.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
    barrier2.dstAccessMask = 0;
    barrier2.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
    barrier2.newLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    barrier2.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier2.subresourceRange.baseMipLevel = 0;
    barrier2.subresourceRange.levelCount = 1;
    barrier2.subresourceRange.baseArrayLayer = 0;
    barrier2.subresourceRange.layerCount = 1;
    barrier2.image = loci_swapchain.pVkImages[loci_swapchain.imageIndex];
    barrier2.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier2.pNext = NULL;

    VkImageMemoryBarrier barrier3;
    barrier3.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
    barrier3.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
    barrier3.dstAccessMask = 0;
    barrier3.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL;
    barrier3.newLayout = VK_IMAGE_LAYOUT_GENERAL;
    barrier3.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    barrier3.subresourceRange.baseMipLevel = 0;
    barrier3.subresourceRange.levelCount = 1;
    barrier3.subresourceRange.baseArrayLayer = 0;
    barrier3.subresourceRange.layerCount = 1;
    barrier3.image = loci_rayImage.vkImage;
    barrier3.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier3.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
    barrier3.pNext = NULL;

    Loci_RayPipelinePushConstants pushConstants;
    pushConstants.numberPointLights = scene.numberPointLights;
    
    loci_beginCommand(loci_drawVkCommandBuffer);

    vkCmdBindPipeline
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
    scene.pipeline.vkPipeline);

    vkCmdPushConstants
    (loci_drawVkCommandBuffer,
    scene.pipeline.vkPipelineLayout,
    VK_SHADER_STAGE_RAYGEN_BIT_KHR |
    VK_SHADER_STAGE_MISS_BIT_KHR |
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR,
    0, sizeof(Loci_RayPipelinePushConstants),
    &pushConstants);

    vkCmdBindDescriptorSets
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_BIND_POINT_RAY_TRACING_KHR,
    scene.pipeline.vkPipelineLayout,
    0,1,
    &scene.descriptorSet.vkDescriptorSet,
    0,0);

    loci_vkCmdTraceRaysKHR
    (loci_drawVkCommandBuffer,
    &genShaderEntry,
    &missShaderEntry,
    &hitShaderEntry,
    &callableShaderEntry,
    loci_windowWidth,
    loci_windowHeight,
    1);

    vkCmdPipelineBarrier
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    0,
    0, NULL, 0, NULL,
    1, &barrier0);

    vkCmdPipelineBarrier
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    0,
    0, NULL, 0, NULL,
    1, &barrier1);

    vkCmdCopyImage
    (loci_drawVkCommandBuffer,
    loci_rayImage.vkImage,
    VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL,
    loci_swapchain.pVkImages[loci_swapchain.imageIndex],
    VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL,
    1, &copy);

    vkCmdPipelineBarrier
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    0,
    0, NULL, 0, NULL,
    1, &barrier2);


    vkCmdPipelineBarrier
    (loci_drawVkCommandBuffer,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    VK_PIPELINE_STAGE_ALL_COMMANDS_BIT,
    0,
    0, NULL, 0, NULL,
    1, &barrier3);


    loci_endCommand(loci_drawVkCommandBuffer);

    loci_submitCommand
    (VK_PIPELINE_STAGE_RAY_TRACING_SHADER_BIT_KHR,
    loci_topAccelerationUpdateVkSemaphore,
    loci_drawVkCommandBuffer,
    loci_drawVkSemaphore,
    loci_drawVkFence);

    loci_presentCommand(loci_drawVkSemaphore);

    loci_waitForFence(&loci_drawVkFence);
}
