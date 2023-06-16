#include "loci_acceleration.h"


Loci_TopAcceleration loci_createTopAcceleration
(Loci_BufferInterface instanceBuffer, uint32_t numberInstances)
{
    VkDeviceOrHostAddressConstKHR instanceAddress;
    instanceAddress.deviceAddress = instanceBuffer.deviceAddress;

    VkAccelerationStructureGeometryKHR geometry;
    geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
    geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
    geometry.geometry.instances.sType = 
    VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.geometry.instances.data = instanceAddress;
    geometry.geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.geometry.instances.pNext = NULL;
    geometry.pNext = NULL;

    VkAccelerationStructureBuildGeometryInfoKHR buildInfo0;
    buildInfo0.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildInfo0.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    buildInfo0.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR | 
                    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    buildInfo0.srcAccelerationStructure = NULL;
    buildInfo0.dstAccelerationStructure = NULL;
    buildInfo0.geometryCount = 1;
    buildInfo0.pGeometries = &geometry;
    buildInfo0.ppGeometries = NULL;
    buildInfo0.pNext = NULL;


    VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo;
    buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
    buildSizeInfo.accelerationStructureSize = 0;
    buildSizeInfo.buildScratchSize = 0;
    buildSizeInfo.updateScratchSize = 0;
    buildSizeInfo.pNext = NULL;

    loci_vkGetAccelerationStructureBuildSizesKHR
    (loci_vkDevice,
    VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
    &buildInfo0,
    &numberInstances,
    &buildSizeInfo);

    

    Loci_TopAcceleration acceleration;
    acceleration.buffer = 
    loci_createBuffer
    (buildSizeInfo.accelerationStructureSize, 1,
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR);

    VkAccelerationStructureCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    createInfo.buffer = acceleration.buffer.vkBuffer;
    createInfo.size = buildSizeInfo.accelerationStructureSize;
    createInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    createInfo.createFlags = 0;
    createInfo.offset = 0;
    createInfo.deviceAddress = 0;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (loci_vkCreateAccelerationStructureKHR
    (loci_vkDevice,
    &createInfo, NULL,
    &acceleration.vkAccelerationStructureKHR),
    "createTopAcceleration",
    "vkCreateAccelerationStructureKHR")

    acceleration.scratchBuffer = 
    loci_createBuffer(buildSizeInfo.buildScratchSize,1,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    VkAccelerationStructureBuildGeometryInfoKHR buildInfo1;
    buildInfo1.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildInfo1.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    buildInfo1.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR |
                        VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    buildInfo1.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    buildInfo1.dstAccelerationStructure = acceleration.vkAccelerationStructureKHR;
    buildInfo1.srcAccelerationStructure = 0;
    buildInfo1.geometryCount = 1;
    buildInfo1.pGeometries = &geometry;
    buildInfo1.ppGeometries = NULL;
    buildInfo1.scratchData.deviceAddress = acceleration.scratchBuffer.deviceAddress;
    buildInfo1.pNext = NULL;

    VkAccelerationStructureBuildRangeInfoKHR rangeInfo;
    rangeInfo.primitiveCount = numberInstances;
    rangeInfo.primitiveOffset = 0;
    rangeInfo.firstVertex = 0;
    rangeInfo.transformOffset = 0;

    const VkAccelerationStructureBuildRangeInfoKHR* rangeInfoArr[1] =
    {&rangeInfo};

    loci_beginCommand(loci_buildVkCommandBuffer);

    loci_vkCmdBuildAccelerationStructuresKHR
    (loci_buildVkCommandBuffer,
    1,
    &buildInfo1,
    rangeInfoArr);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);    

    VkAccelerationStructureDeviceAddressInfoKHR addressInfo;  
    addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    addressInfo.accelerationStructure = acceleration.vkAccelerationStructureKHR;
    addressInfo.pNext = NULL;
    
    acceleration.deviceAddress = 
    loci_vkGetAccelerationStructureDeviceAddressKHR
    (loci_vkDevice, &addressInfo);

    return acceleration;
}

void loci_destroyTopAcceleration(Loci_TopAcceleration acceleration)
{
    loci_vkDestroyAccelerationStructureKHR
    (loci_vkDevice,
    acceleration.vkAccelerationStructureKHR,
    NULL);

    loci_destroyBuffer(acceleration.scratchBuffer);

    loci_destroyBuffer(acceleration.buffer);
}

void loci_updateTopAcceleration
(Loci_TopAcceleration acceleration,
Loci_BufferInterface instanceBuffer, 
uint32_t numberInstances,
bool animationUpdate, bool bottomAccelerationUpdate)
{
     VkDeviceOrHostAddressConstKHR instanceAddress;
    instanceAddress.deviceAddress = instanceBuffer.deviceAddress;

    VkAccelerationStructureGeometryKHR geometry;
    geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
    geometry.geometryType = VK_GEOMETRY_TYPE_INSTANCES_KHR;
    geometry.geometry.instances.sType = 
    VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_INSTANCES_DATA_KHR;
    geometry.geometry.instances.data = instanceAddress;
    geometry.geometry.instances.arrayOfPointers = VK_FALSE;
    geometry.geometry.instances.pNext = NULL;
    geometry.pNext = NULL;

    VkAccelerationStructureBuildGeometryInfoKHR buildInfo;
    buildInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_TOP_LEVEL_KHR;
    buildInfo.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR |
                    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    buildInfo.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_UPDATE_KHR;
    buildInfo.srcAccelerationStructure = acceleration.vkAccelerationStructureKHR;
    buildInfo.dstAccelerationStructure = acceleration.vkAccelerationStructureKHR;
    buildInfo.geometryCount = 1;
    buildInfo.pGeometries = &geometry;
    buildInfo.ppGeometries = NULL;
    buildInfo.scratchData.deviceAddress = acceleration.scratchBuffer.deviceAddress;
    buildInfo.pNext = NULL;

    VkAccelerationStructureBuildRangeInfoKHR rangeInfo;
    rangeInfo.primitiveCount = numberInstances;
    rangeInfo.primitiveOffset = 0;
    rangeInfo.firstVertex = 0;
    rangeInfo.transformOffset = 0;

    const VkAccelerationStructureBuildRangeInfoKHR* rangeInfoArr[1] = {&rangeInfo};

    loci_beginCommand(loci_topAccelerationUpdateVkCommandBuffer);

    loci_vkCmdBuildAccelerationStructuresKHR
    (loci_topAccelerationUpdateVkCommandBuffer,
    1,
    &buildInfo,
    rangeInfoArr);

    loci_endCommand(loci_topAccelerationUpdateVkCommandBuffer);

    if(bottomAccelerationUpdate)
    {
        loci_submitCommand
        (VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
        loci_bottomAccelerationUpdateVkSemaphore,
        loci_topAccelerationUpdateVkCommandBuffer,
        loci_topAccelerationUpdateVkSemaphore,
        NULL);
    }
    else if(animationUpdate)
    {
        loci_submitCommand
        (VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
        loci_animationUpdateVkSemaphore,
        loci_topAccelerationUpdateVkCommandBuffer,
        loci_topAccelerationUpdateVkSemaphore,
        NULL);
    }
    else
    {
        loci_submitCommand
        (VK_PIPELINE_STAGE_ACCELERATION_STRUCTURE_BUILD_BIT_KHR,
        loci_imageIndexVkSemaphore,
        loci_topAccelerationUpdateVkCommandBuffer,
        loci_topAccelerationUpdateVkSemaphore,
        NULL);
    } 
}


Loci_BottomAcceleration loci_createBottomAcceleration
(Loci_Buffer vertexBuffer, Loci_Buffer indexBuffer,
Loci_Buffer transformBuffer)
{
    Loci_BottomAcceleration acceleration;

    VkDeviceOrHostAddressConstKHR vertexAddress;
    vertexAddress.deviceAddress = vertexBuffer.deviceAddress;
    VkDeviceOrHostAddressConstKHR indexAddress;
    indexAddress.deviceAddress = indexBuffer.deviceAddress;
    VkDeviceOrHostAddressConstKHR transformAddress;
    transformAddress.deviceAddress = transformBuffer.deviceAddress;

    VkAccelerationStructureGeometryKHR geometry;
    geometry.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_KHR;
    geometry.flags = VK_GEOMETRY_OPAQUE_BIT_KHR;
    geometry.geometryType = VK_GEOMETRY_TYPE_TRIANGLES_KHR;
    geometry.geometry.triangles.sType = 
    VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_GEOMETRY_TRIANGLES_DATA_KHR;
    geometry.geometry.triangles.vertexFormat = VK_FORMAT_R32G32B32_SFLOAT;
    geometry.geometry.triangles.vertexData = vertexAddress;
    geometry.geometry.triangles.maxVertex = vertexBuffer.numberElements;
    geometry.geometry.triangles.vertexStride = vertexBuffer.elementSize;
    geometry.geometry.triangles.indexType = VK_INDEX_TYPE_UINT32;
    geometry.geometry.triangles.indexData = indexAddress;
    geometry.geometry.triangles.transformData = transformAddress;
    geometry.geometry.triangles.pNext = NULL;
    geometry.pNext = NULL;

    VkAccelerationStructureBuildGeometryInfoKHR buildInfo0;
    buildInfo0.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildInfo0.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    buildInfo0.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR |
                    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    buildInfo0.srcAccelerationStructure = NULL;
    buildInfo0.dstAccelerationStructure = NULL;
    buildInfo0.geometryCount = 1;
    buildInfo0.pGeometries = &geometry;
    buildInfo0.ppGeometries = NULL;
    buildInfo0.pNext = NULL;

    const uint32_t numberTriangles = indexBuffer.numberElements/3;

    VkAccelerationStructureBuildSizesInfoKHR buildSizeInfo;
    buildSizeInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_SIZES_INFO_KHR;
    buildSizeInfo.accelerationStructureSize = 0;
    buildSizeInfo.buildScratchSize = 0;
    buildSizeInfo.updateScratchSize = 0;
    buildSizeInfo.pNext = NULL;

    loci_vkGetAccelerationStructureBuildSizesKHR
    (loci_vkDevice,
    VK_ACCELERATION_STRUCTURE_BUILD_TYPE_DEVICE_KHR,
    &buildInfo0,
    &numberTriangles,
    &buildSizeInfo);

    acceleration.buffer = 
    loci_createBuffer
    (1, buildSizeInfo.accelerationStructureSize,
    VK_BUFFER_USAGE_ACCELERATION_STRUCTURE_STORAGE_BIT_KHR |
    VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT);

    VkAccelerationStructureCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_CREATE_INFO_KHR;
    createInfo.buffer = acceleration.buffer.vkBuffer;
    createInfo.size = buildSizeInfo.accelerationStructureSize;
    createInfo.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    createInfo.deviceAddress = 0;
    createInfo.offset = 0;
    createInfo.createFlags = 0;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (loci_vkCreateAccelerationStructureKHR
    (loci_vkDevice,
    &createInfo, NULL,
    &acceleration.vkAccelerationStructureKHR),
    "createBottomAcceleration",
    "vkCreateAccelerationStructureKHR") 

    acceleration.scratchBuffer = 
    loci_createBuffer
    (1, buildSizeInfo.buildScratchSize,
    VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);

    VkAccelerationStructureBuildGeometryInfoKHR buildInfo1;
    buildInfo1.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_BUILD_GEOMETRY_INFO_KHR;
    buildInfo1.type = VK_ACCELERATION_STRUCTURE_TYPE_BOTTOM_LEVEL_KHR;
    buildInfo1.flags = VK_BUILD_ACCELERATION_STRUCTURE_ALLOW_UPDATE_BIT_KHR |
                    VK_BUILD_ACCELERATION_STRUCTURE_PREFER_FAST_BUILD_BIT_KHR;
    buildInfo1.mode = VK_BUILD_ACCELERATION_STRUCTURE_MODE_BUILD_KHR;
    buildInfo1.srcAccelerationStructure = NULL;
    buildInfo1.dstAccelerationStructure = acceleration.vkAccelerationStructureKHR;
    buildInfo1.geometryCount = 1;
    buildInfo1.pGeometries = &geometry;
    buildInfo1.ppGeometries = NULL;
    buildInfo1.scratchData.deviceAddress = acceleration.scratchBuffer.deviceAddress;
    buildInfo1.pNext = NULL;

    VkAccelerationStructureBuildRangeInfoKHR rangeInfo;
    rangeInfo.primitiveCount = numberTriangles;
    rangeInfo.primitiveOffset = 0;
    rangeInfo.firstVertex = 0;
    rangeInfo.transformOffset = 0;

    const VkAccelerationStructureBuildRangeInfoKHR* rangeInfoArr[1] = 
    {&rangeInfo};

    loci_beginCommand(loci_buildVkCommandBuffer);

    loci_vkCmdBuildAccelerationStructuresKHR
    (loci_buildVkCommandBuffer,
    1,
    &buildInfo1,
    rangeInfoArr);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);


    VkAccelerationStructureDeviceAddressInfoKHR addressInfo;
    addressInfo.sType = VK_STRUCTURE_TYPE_ACCELERATION_STRUCTURE_DEVICE_ADDRESS_INFO_KHR;
    addressInfo.accelerationStructure = acceleration.vkAccelerationStructureKHR;
    addressInfo.pNext = NULL;

    acceleration.deviceAddress = 
    loci_vkGetAccelerationStructureDeviceAddressKHR
    (loci_vkDevice, &addressInfo);

    return acceleration;
}

void loci_destroyBottomAcceleration
(Loci_BottomAcceleration acceleration)
{
    loci_vkDestroyAccelerationStructureKHR
    (loci_vkDevice,
    acceleration.vkAccelerationStructureKHR,
    NULL);

    loci_destroyBuffer(acceleration.scratchBuffer);

    loci_destroyBuffer(acceleration.buffer);
}
