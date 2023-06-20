#include "loci_scene.h"


void loci_createScene(ecs_entity_t entity,
uint32_t maxNumberInstances,
uint32_t maxNumberImages,
uint32_t maxNumberSkeletons,
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
        LOCI_LOGE("createScene", "malloc() for pImages", "failed")
    }

    scene.pGeometries = (VkAccelerationStructureGeometryKHR*) malloc(sizeof(VkAccelerationStructureGeometryKHR)*maxNumberSkeletons);
    if(scene.pGeometries == NULL)
    {
        LOCI_LOGE("createScene", "malloc() for pGeometries", "failed")
    }

    scene.pBuildInfos = (VkAccelerationStructureBuildGeometryInfoKHR*) malloc(sizeof(VkAccelerationStructureBuildGeometryInfoKHR)*maxNumberSkeletons);
    if(scene.pBuildInfos == NULL)
    {
        LOCI_LOGE("createScene", "malloc() for pBuildInfos", "failed")
    }

    scene.pRangeInfos = (VkAccelerationStructureBuildRangeInfoKHR*) malloc(sizeof(VkAccelerationStructureBuildRangeInfoKHR)*maxNumberSkeletons);
    if(scene.pRangeInfos == NULL)
    {
        LOCI_LOGE("createScene", "malloc() for pRangeInfos", "failed")
    }

    scene.ppRangeInfos = (VkAccelerationStructureBuildRangeInfoKHR**) malloc(sizeof(VkAccelerationStructureBuildRangeInfoKHR*)*maxNumberSkeletons);
    if(scene.ppRangeInfos == NULL)
    {
        LOCI_LOGE("createScene", "malloc() for ppRangeInfos", "failed")
    }

    scene.pointLightBuffer =
    loci_createBufferInterface
    (16, sizeof(Loci_PointLightObject),
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

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

    ECS_COMPONENT(loci_pWorld, Loci_Animation);
    ECS_COMPONENT(loci_pWorld, Loci_Channel);
    ECS_COMPONENT(loci_pWorld, Loci_Bone);
    ECS_COMPONENT(loci_pWorld, Loci_Skeleton);
    ECS_COMPONENT(loci_pWorld, Loci_Transform);
    ECS_COMPONENT(loci_pWorld, Loci_Mesh);
    ECS_COMPONENT(loci_pWorld, Loci_Material);
    ECS_COMPONENT(loci_pWorld, Loci_Texture);
    ECS_COMPONENT(loci_pWorld, Loci_PointLight);

    scene.animationUpdateQuery = 
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms = 
        {
            {ecs_id(Loci_Transform)},
            {ecs_id(Loci_Animation)},
            {ecs_id(Loci_Skeleton)},
            {.id = ecs_pair(loci_spawnedIn, entity)}
        }
    });

    scene.skeletonUpdateQuery = 
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms = 
        {
            {ecs_id(Loci_Skeleton)},
            {.id = ecs_pair(loci_spawnedIn, entity)}
        }
    });

    scene.bottomAccelerationUpdateQuery = 
    ecs_query_init(loci_pWorld, &(ecs_query_desc_t)
    {
        .filter.terms = 
        {
            {ecs_id(Loci_Mesh)},
            {ecs_id(Loci_Skeleton)},
            {.id = ecs_pair(loci_spawnedIn, entity)}
        }
    });

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

    #ifdef LOCI_DEBUG
    LOCI_LOGI("Scene", "created", "")
    #endif
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

    free(scene.ppRangeInfos);
    free(scene.pRangeInfos);
    free(scene.pBuildInfos);
    free(scene.pGeometries);

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
    
    bool skeletonUpdate = false;
    ecs_iter_t animationIt = ecs_query_iter(loci_pWorld, scene.animationUpdateQuery);
    while(ecs_query_next(&animationIt))
    {
        Loci_Transform* transforms = ecs_field(&animationIt, Loci_Transform, 1);
        Loci_Animation* animations = ecs_field(&animationIt, Loci_Animation, 2);
        Loci_Skeleton*  skeletons = ecs_field(&animationIt, Loci_Skeleton, 3);
    
        for(int i = 0; i < animationIt.count; i++)
        {
            ecs_iter_t boneIt = ecs_children(loci_pWorld, animations[i].entity);
            while(ecs_children_next(&boneIt))
            {
                for(int j = 0; j < boneIt.count; j++)
                {
                    loci_updateAnimation(boneIt.entities[j], animations[i].animation,
                    animations[i].time, transforms[i].transform,
                    (Loci_Bones*)skeletons[i].bonesBuffer.pMemory);
                }
            }
        }
        skeletonUpdate = true;
    }

    if(skeletonUpdate)
    {
        ecs_iter_t skeletonIt = ecs_query_iter(loci_pWorld, scene.skeletonUpdateQuery);
        loci_beginCommand(loci_skeletonUpdateVkCommandBuffer);
        while(ecs_query_next(&skeletonIt))
        {
            Loci_Skeleton* skeletons = ecs_field(&skeletonIt, Loci_Skeleton, 1);

            for(int i = 0; i < skeletonIt.count; i++)
            {   
                vkCmdBindPipeline
                (loci_skeletonUpdateVkCommandBuffer,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                skeletons[i].pipeline.vkPipeline);

                vkCmdBindDescriptorSets
                (loci_skeletonUpdateVkCommandBuffer,
                VK_PIPELINE_BIND_POINT_COMPUTE,
                skeletons[i].pipeline.vkPipelineLayout,
                0,1, &skeletons[i].descriptorSet.vkDescriptorSet,
                0, NULL);

                vkCmdDispatch
                (loci_skeletonUpdateVkCommandBuffer,
                skeletons[i].vertexBuffer.numberElements,1,1); 
            }
        }
    
        loci_endCommand(loci_skeletonUpdateVkCommandBuffer);
        loci_submitCommand
        (VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        loci_imageIndexVkSemaphore,
        loci_skeletonUpdateVkCommandBuffer,
        loci_skeletonUpdateVkSemaphore, NULL);

        uint32_t numberSkeletons = 0;
        ecs_iter_t bottomAccIter = ecs_query_iter(loci_pWorld, scene.bottomAccelerationUpdateQuery);
        while(ecs_query_next(&bottomAccIter))
        {
            Loci_Mesh* meshes = ecs_field(&bottomAccIter, Loci_Mesh, 1);
            Loci_Skeleton* skeletons = ecs_field(&bottomAccIter, Loci_Skeleton, 2);

            for(int i = 0; i < bottomAccIter.count; i++)
            {
                VkDeviceOrHostAddressConstKHR vertexAddress;
                vertexAddress.deviceAddress = meshes[i].vertexBuffer.deviceAddress;
                VkDeviceOrHostAddressConstKHR indexAddress;
                indexAddress.deviceAddress = meshes[i].indexBuffer.deviceAddress;
                VkDeviceOrHostAddressConstKHR transformAddress;
                transformAddress.deviceAddress = meshes[i].transformBuffer.deviceAddress;

                VkAccelerationStructureGeometryKHR geometry;
                geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
                geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
                geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
                geometry.geometry.triangles.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
                geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
                geometry.geometry.triangles.vertexData = vertexAddress;
                geometry.geometry.triangles.maxVertex = meshes[i].vertexBuffer.numberElements;
                geometry.geometry.triangles.vertexStride = meshes[i].vertexBuffer.elementSize;
                geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
                geometry.geometry.triangles.indexData = indexAddress;
                geometry.geometry.triangles.transformData = transformAddress;
                geometry.geometry.triangles.pNext = NULL;
                geometry.pNext = NULL;

                scene.pGeometries[numberSkeletons] = geometry;

                VkAccelerationStructureBuildGeometryInfoKHR buildInfo;
                buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
                buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
                buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR | VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
                buildInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
                buildInfo.srcAccelerationStructure = meshes[i].acceleration.vkAccelerationStructureKHR;
                buildInfo.dstAccelerationStructure = meshes[i].acceleration.vkAccelerationStructureKHR;
                buildInfo.geometryCount = 1;
                buildInfo.pGeometries = &scene.pGeometries[numberSkeletons];
                buildInfo.ppGeometries = NULL;
                buildInfo.scratchData.deviceAddress = meshes[i].acceleration.scratchBuffer.deviceAddress;
                buildInfo.pNext = NULL;

                scene.pBuildInfos[numberSkeletons] = buildInfo;

                VkAccelerationStructureBuildRangeInfoKHR rangeInfo;
                rangeInfo.primitiveCount = meshes[i].indexBuffer.numberElements/3;
                rangeInfo.primitiveOffset = 0;
                rangeInfo.firstVertex = 0;
                rangeInfo.transformOffset = 0;

                scene.pRangeInfos[numberSkeletons++] = rangeInfo;
            }
        }
        
        for(uint32_t i = 0; i < numberSkeletons; i++)
        {
            scene.ppRangeInfos[i] = &scene.pRangeInfos[i];
        }
        loci_beginCommand(loci_bottomAccelerationUpdateVkCommandBuffer);

        loci_vkCmdBuildAccelerationStructuresKHR
        (loci_bottomAccelerationUpdateVkCommandBuffer,
        numberSkeletons,
        scene.pBuildInfos,
        (const VkAccelerationStructureBuildRangeInfoKHR**)scene.ppRangeInfos);

        loci_endCommand(loci_bottomAccelerationUpdateVkCommandBuffer);
        loci_submitCommand
        (VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
        loci_skeletonUpdateVkSemaphore,
        loci_bottomAccelerationUpdateVkCommandBuffer,
        loci_bottomAccelerationUpdateVkSemaphore, NULL);
    }

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

    //printf("numberInstances:    %u\n", numberInstances);
    //printf("numberImages:    %u\n", numberImages);
    //printf("numberPointLights: %u\n", numberPointLights);

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
        skeletonUpdate);

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
        skeletonUpdate);
    
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


//helper Functions
float loci_getScaleFactor(float minTime, float maxTime, float time)
{
    float midTime = time - minTime;
    float timeDiff = maxTime - minTime;
    return midTime/timeDiff; 
}

void loci_interpolateScale(float time, Loci_KeyFrameScale* pScales, uint32_t numberScales, mat4 outTransform)
{
    glm_mat4_identity(outTransform);
    if(numberScales == 1)
    {
        glm_scale(outTransform, pScales[0].scale);
        return;
    }

    bool foundTime = false;
    uint32_t maxIndex;
    uint32_t minIndex;
    for(uint32_t i = 1; i < numberScales; i++)
    {
        if(time < pScales[i].time)
        {
            maxIndex = i;
            minIndex = i-1;
            foundTime = true;
            break;
        }
    }
    if(!foundTime)
    {
        LOCI_LOGE("interpolateScale", "time out of range", "")
    }

    float scaleFactor = loci_getScaleFactor(pScales[minIndex].time, pScales[maxIndex].time, time);

    vec3 scale;
    glm_vec3_lerp(pScales[minIndex].scale, pScales[maxIndex].scale, scaleFactor, scale);

    glm_scale(outTransform, scale);
}

void loci_interpolateRotation(float time, Loci_KeyFrameRotation* pRotations, uint32_t numberRotations, mat4 outTransform)
{
    glm_mat4_identity(outTransform);
    if(numberRotations == 1)
    {
        glm_quat_rotate(outTransform, pRotations[0].rotation, outTransform);
        return;
    }

    bool foundTime = false;
    uint32_t maxIndex;
    uint32_t minIndex;
    for(uint32_t i = 1; i < numberRotations; i++)
    {
        if(time < pRotations[i].time)
        {
            maxIndex = i;
            minIndex = i-1;
            foundTime = true;
            break;
        }
    }
    if(!foundTime)
    {
        LOCI_LOGE("interpolateRotation", "time out of range", "")
    }

    float scaleFactor = loci_getScaleFactor(pRotations[minIndex].time, pRotations[maxIndex].time, time);

    versor rotation;
    glm_quat_slerp(pRotations[minIndex].rotation, pRotations[maxIndex].rotation, scaleFactor, rotation);

    glm_quat_rotate(outTransform, rotation, outTransform);
}

void loci_interpolatePosition(float time, Loci_KeyFramePosition* pPositions, uint32_t numberPositions, mat4 outTransform)
{
    glm_mat4_identity(outTransform);
    if(numberPositions == 1)
    {
        glm_translate(outTransform, pPositions[0].position);
        return;
    }

    bool foundTime = false;
    uint32_t maxIndex;
    uint32_t minIndex;
    for(uint32_t i = 1; i < numberPositions; i++)
    {
        if(time < pPositions[i].time)
        {
            maxIndex = i;
            minIndex = i-1;
            foundTime = true;
            break;
        }
    }
    if(!foundTime)
    {
        LOCI_LOGE("interpolatePosition", "time out of range", "")
    }

    float scaleFactor = loci_getScaleFactor(pPositions[minIndex].time, pPositions[maxIndex].time, time);

    vec3 position;
    glm_vec3_lerp(pPositions[minIndex].position, pPositions[maxIndex].position, scaleFactor, position);

    glm_translate(outTransform, position);
}

void loci_getChannelTransform(float time, const Loci_Channel* pChannel, mat4 outTransform)
{
    mat4 scale;
    loci_interpolateScale(time, pChannel->pScales, pChannel->numberScales, scale);
    mat4 rotation;
    loci_interpolateRotation(time, pChannel->pRotations, pChannel->numberRotations, rotation);
    mat4 position;
    loci_interpolatePosition(time, pChannel->pPositions, pChannel->numberPositions, position);

    glm_mat4_identity(outTransform);
    glm_mat4_mul(rotation, scale, outTransform);
    glm_mat4_mul(position, outTransform, outTransform);
}

void loci_updateAnimation
(ecs_entity_t boneEntity, ecs_entity_t animationEntity, float time, mat4 globalTransform, Loci_Bones* pBones)
{
    mat4 globalTransform2;
    glm_mat4_copy(globalTransform, globalTransform2);

    const Loci_Channel* pChannel;
    loci_getPairSecond(boneEntity, animationEntity, Loci_Channel, pChannel)

    mat4 transform;
    loci_getChannelTransform(time, pChannel, transform);
    glm_mat4_mul(globalTransform2, transform, globalTransform2);

    Loci_Bone* pBone;
    loci_get_mut(boneEntity, Loci_Bone, pBone);

    glm_mat4_mul(globalTransform2, pBone->inverse, pBones->bones[pBone->index]);

    ecs_iter_t it = ecs_children(loci_pWorld, boneEntity);
    while(ecs_children_next(&it))
    {
        for(int i = 0; i < it.count; i++)
        {
            loci_updateAnimation(it.entities[i], animationEntity, time, globalTransform2, pBones);
        }
    }
}