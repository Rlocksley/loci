#include "loci_pipeline.h"

Loci_RayPipeline loci_createRayPipeline
(Loci_RayDescriptorSet descriptorSet,
Loci_Shader generationShader,
Loci_Shader missShaders[], uint32_t numberMissShaders,
Loci_Shader closestHitShaders[], uint32_t numberClosestHitShaders,
uint32_t maxRecursionDepth)
{
    Loci_RayPipeline pipeline;

    VkPushConstantRange range;
    range.stageFlags = 
    VK_SHADER_STAGE_RAYGEN_BIT_KHR |
    VK_SHADER_STAGE_MISS_BIT_KHR |
    VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
    range.offset = 0;
    range.size = sizeof(Loci_RayPipelinePushConstants);

    VkPipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet.vkDescriptorSetLayout;
    layoutInfo.pushConstantRangeCount = 1;
    layoutInfo.pPushConstantRanges = &range;
    layoutInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreatePipelineLayout
    (loci_vkDevice,
    &layoutInfo, NULL,
    &pipeline.vkPipelineLayout),
    "createRayPipeline",
    "vkCreatePipelineLayout")

    VkPipelineShaderStageCreateInfo 
    shaderStages[1+numberMissShaders+numberClosestHitShaders];

    {
        VkPipelineShaderStageCreateInfo genStage;
        genStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        genStage.flags = 0;
        genStage.stage = VK_SHADER_STAGE_RAYGEN_BIT_KHR;
        genStage.module = loci_createShaderModule(generationShader);
        genStage.pName = "main";
        genStage.pSpecializationInfo = NULL;
        genStage.pNext = NULL;
        shaderStages[0] = genStage;
    }

    for(uint32_t i = 0; i < numberMissShaders; i++)
    {
        VkPipelineShaderStageCreateInfo missStage;
        missStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        missStage.flags = 0;
        missStage.stage = VK_SHADER_STAGE_MISS_BIT_KHR;
        missStage.module = loci_createShaderModule(missShaders[i]);
        missStage.pName = "main";
        missStage.pSpecializationInfo = NULL;
        missStage.pNext = NULL;
        shaderStages[i+1] = missStage;
    }

    for(uint32_t i = 0; i < numberClosestHitShaders; i++)
    {
        VkPipelineShaderStageCreateInfo chitStage;
        chitStage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        chitStage.flags = 0;
        chitStage.stage = VK_SHADER_STAGE_CLOSEST_HIT_BIT_KHR;
        chitStage.module = loci_createShaderModule(closestHitShaders[i]);
        chitStage.pName = "main";
        chitStage.pSpecializationInfo = NULL;
        chitStage.pNext = NULL;
        shaderStages[i+numberMissShaders+1] = chitStage;
    }

    VkRayTracingShaderGroupCreateInfoKHR 
    groupEntries[1+numberMissShaders+numberClosestHitShaders];

    {
        VkRayTracingShaderGroupCreateInfoKHR genEntry;
        genEntry.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        genEntry.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        genEntry.generalShader = 0;
        genEntry.closestHitShader = VK_SHADER_UNUSED_KHR;
        genEntry.anyHitShader = VK_SHADER_UNUSED_KHR;
        genEntry.intersectionShader = VK_SHADER_UNUSED_KHR;
        genEntry.pShaderGroupCaptureReplayHandle = NULL;
        genEntry.pNext = NULL;
        groupEntries[0] = genEntry;
    }

    for(uint32_t i = 0; i < numberMissShaders; i++)
    {
        VkRayTracingShaderGroupCreateInfoKHR missEntry;
        missEntry.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        missEntry.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR;
        missEntry.generalShader = i+1;
        missEntry.closestHitShader = VK_SHADER_UNUSED_KHR;
        missEntry.anyHitShader = VK_SHADER_UNUSED_KHR;
        missEntry.intersectionShader = VK_SHADER_UNUSED_KHR;
        missEntry.pShaderGroupCaptureReplayHandle = NULL;
        missEntry.pNext = NULL;
        groupEntries[i+1] = missEntry;
    }

    for(uint32_t i = 0; i < numberClosestHitShaders; i++)
    {
        VkRayTracingShaderGroupCreateInfoKHR chitEntry;
        chitEntry.sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR;
        chitEntry.type = VK_RAY_TRACING_SHADER_GROUP_TYPE_TRIANGLES_HIT_GROUP_KHR;
        chitEntry.generalShader = VK_SHADER_UNUSED_KHR;
        chitEntry.closestHitShader = i+numberMissShaders+1;
        chitEntry.anyHitShader = VK_SHADER_UNUSED_KHR;
        chitEntry.intersectionShader = VK_SHADER_UNUSED_KHR;
        chitEntry.pShaderGroupCaptureReplayHandle = NULL;
        chitEntry.pNext = NULL;
        groupEntries[i+numberMissShaders+1] = chitEntry;
    }

    VkRayTracingPipelineCreateInfoKHR createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR;
    createInfo.flags = 0;
    createInfo.stageCount = 1 + numberMissShaders + numberClosestHitShaders;
    createInfo.pStages = shaderStages;
    createInfo.groupCount = 1 + numberMissShaders + numberClosestHitShaders;
    createInfo.pGroups = groupEntries;
    createInfo.maxPipelineRayRecursionDepth = maxRecursionDepth;
    createInfo.layout = pipeline.vkPipelineLayout;
    createInfo.basePipelineIndex = 0;
    createInfo.basePipelineHandle = NULL;
    createInfo.pDynamicState = NULL;
    createInfo.pLibraryInfo = NULL;
    createInfo.pLibraryInterface = NULL;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (loci_vkCreateRayTracingPipelinesKHR
    (loci_vkDevice,
    VK_NULL_HANDLE, VK_NULL_HANDLE,
    1, &createInfo, NULL,
    &pipeline.vkPipeline),
    "createRayPipeline",
    "vkCreateRayTracingPipelineKHR")

    for(uint32_t i = 0; i < 1 + numberMissShaders + numberClosestHitShaders; i++)
    {
        loci_destroyShaderModule(shaderStages[i].module);
    }

    #ifdef LOCI_DEBUG
    LOCI_LOGI("RayPipeline", "created", "")
    #endif

    return pipeline;
}   

void loci_destroyRayPipeline(Loci_RayPipeline pipeline)
{
    vkDestroyPipeline
    (loci_vkDevice,
    pipeline.vkPipeline, 
    NULL);

    vkDestroyPipelineLayout
    (loci_vkDevice,
    pipeline.vkPipelineLayout,
    NULL);
}



Loci_SkeletonPipeline loci_createSkeletonPipeline
(Loci_SkeletonDescriptorSet descriptorSet)
{
    Loci_SkeletonPipeline pipeline;

    VkPipelineLayoutCreateInfo layoutInfo;
    layoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    layoutInfo.flags = 0;
    layoutInfo.setLayoutCount = 1;
    layoutInfo.pSetLayouts = &descriptorSet.vkDescriptorSetLayout;
    layoutInfo.pushConstantRangeCount = 0;
    layoutInfo.pPushConstantRanges = NULL;
    layoutInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreatePipelineLayout
    (loci_vkDevice,
    &layoutInfo, NULL,
    &pipeline.vkPipelineLayout),
    "createSkeletonPipeline",
    "vkCreatePipelineLayout")

    VkPipelineShaderStageCreateInfo shaderInfo;
    shaderInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    shaderInfo.flags = 0;
    shaderInfo.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    shaderInfo.module = loci_createShaderModule(loci_skeletonShader);
    shaderInfo.pName = "main";
    shaderInfo.pSpecializationInfo = NULL;
    shaderInfo.pNext = NULL;

    VkComputePipelineCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.layout = pipeline.vkPipelineLayout;
    createInfo.stage = shaderInfo;
    createInfo.basePipelineIndex = -1;
    createInfo.basePipelineHandle = NULL;
    createInfo.pNext = NULL;

    LOCI_CHECK_VULKAN
    (vkCreateComputePipelines
    (loci_vkDevice, NULL,
    1, &createInfo, NULL,
    &pipeline.vkPipeline),
    "createSkeletonPipeline",
    "vkCreateComputePipeline")

    loci_destroyShaderModule(shaderInfo.module);

    return pipeline;
}

void loci_destroySkeletonPipeline(Loci_SkeletonPipeline pipeline)
{
    vkDestroyPipeline
    (loci_vkDevice,
    pipeline.vkPipeline,
    NULL);

    vkDestroyPipelineLayout
    (loci_vkDevice,
    pipeline.vkPipelineLayout,
    NULL);
}
