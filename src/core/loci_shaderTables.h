#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H
#include "loci_device.h"
#endif
#ifndef LOCI_BUFFER_H
#define LOCI_BUFFER_H
#include "loci_buffer.h"
#endif
#ifndef LOCI_PIPELINE_H
#define LOCI_PIPELINE_H
#include "loci_pipeline.h"
#endif
#ifndef LOCI_COMMANDBUFFER_H
#define LOCI_COMMANDBUFFER_H
#include "loci_commandBuffer.h"
#endif

typedef struct Loci_ShaderTables
{
    Loci_Buffer genShaderTableBuffer;
    Loci_Buffer missShaderTableBuffer;
    Loci_Buffer chitShaderTableBuffer;
}Loci_ShaderTables;

Loci_ShaderTables loci_createShaderTables
(Loci_RayPipeline pipeline,
uint32_t numberMissShaders,
uint32_t numberClosestHitShaders);
void loci_destroyShaderTables(Loci_ShaderTables shaderTables);

Loci_ShaderTables loci_createShaderTables
(Loci_RayPipeline pipeline,
uint32_t numberMissShaders,
uint32_t numberClosestHitShaders)
{
    Loci_ShaderTables tables;

    uint32_t handleSize = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleSize;
    uint32_t alignment = loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR.shaderGroupHandleAlignment;
    uint32_t handleSizeAligned = (handleSize + alignment - 1) & ~(alignment - 1);
    uint32_t groupCount = 3;
    uint32_t stbSize = handleSizeAligned + 
    numberMissShaders * handleSizeAligned + 
    numberClosestHitShaders*handleSizeAligned;

    uint8_t shaderHandleStorage[stbSize];

    LOCI_CHECK_VULKAN
    (loci_vkGetRayTracingShaderGroupHandlesKHR
    (loci_vkDevice,
    pipeline.vkPipeline,
    0, 7, stbSize,
    shaderHandleStorage),
    "createShaderTables",
    "vkGetRayTracingShaderGroupHandlesKHR")

    Loci_BufferInterface genShaderBufferInterface = 
    loci_createBufferInterface
    (1, handleSizeAligned, 
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(genShaderBufferInterface.pMemory, 
    &shaderHandleStorage[0], handleSizeAligned);

    Loci_BufferInterface missShaderBufferInterface = 
    loci_createBufferInterface
    (numberMissShaders, handleSizeAligned, 
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(missShaderBufferInterface.pMemory, 
    &shaderHandleStorage[0]+handleSizeAligned, 
    handleSizeAligned * numberMissShaders);
 
    Loci_BufferInterface chitShaderBufferInterface = 
    loci_createBufferInterface
    (numberClosestHitShaders, handleSizeAligned,
    VK_BUFFER_USAGE_TRANSFER_SRC_BIT);
    memcpy(chitShaderBufferInterface.pMemory,
    &shaderHandleStorage[0]+handleSizeAligned*(numberMissShaders+1), 
    handleSizeAligned * numberClosestHitShaders);

    tables.genShaderTableBuffer = 
    loci_createBuffer
    (1, handleSizeAligned,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |  
    VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR);
    
    tables.missShaderTableBuffer = 
    loci_createBuffer
    (numberMissShaders, handleSizeAligned,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |  
    VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR);
    tables.chitShaderTableBuffer = 
    
    loci_createBuffer
    (numberClosestHitShaders, handleSizeAligned,
    VK_BUFFER_USAGE_TRANSFER_DST_BIT |  
    VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR);

    VkBufferCopy genBufferCopy;
    genBufferCopy.srcOffset = 0;
    genBufferCopy.dstOffset = 0;
    genBufferCopy.size = handleSizeAligned;

    VkBufferCopy missBufferCopy;
    missBufferCopy.srcOffset = 0;
    missBufferCopy.dstOffset = 0;
    missBufferCopy.size = handleSizeAligned * numberMissShaders;

    VkBufferCopy chitBufferCopy;
    chitBufferCopy.srcOffset = 0;
    chitBufferCopy.dstOffset = 0;
    chitBufferCopy.size = handleSizeAligned * numberClosestHitShaders;

    loci_beginCommand(loci_buildVkCommandBuffer);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    genShaderBufferInterface.vkBuffer,
    tables.genShaderTableBuffer.vkBuffer,
    1, &genBufferCopy);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    missShaderBufferInterface.vkBuffer,
    tables.missShaderTableBuffer.vkBuffer,
    1, &missBufferCopy);

    vkCmdCopyBuffer
    (loci_buildVkCommandBuffer,
    chitShaderBufferInterface.vkBuffer,
    tables.chitShaderTableBuffer.vkBuffer,
    1, &chitBufferCopy);

    loci_endCommand(loci_buildVkCommandBuffer);

    loci_submitSingleCommand
    (loci_buildVkCommandBuffer, loci_buildVkFence);

    loci_destroyBufferInterface(chitShaderBufferInterface);
    loci_destroyBufferInterface(missShaderBufferInterface);
    loci_destroyBufferInterface(genShaderBufferInterface);

    return tables;
}

void loci_destroyShaderTables(Loci_ShaderTables shaderTables)
{
    loci_destroyBuffer(shaderTables.chitShaderTableBuffer);
    loci_destroyBuffer(shaderTables.missShaderTableBuffer);
    loci_destroyBuffer(shaderTables.genShaderTableBuffer);
}
