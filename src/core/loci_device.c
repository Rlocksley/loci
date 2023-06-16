#include "loci_device.h"


void loci_loadRayFunctions()
{
    loci_vkGetBufferDeviceAddressKHR = (PFN_vkGetBufferDeviceAddressKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkGetBufferDeviceAddressKHR");
    loci_vkCmdBuildAccelerationStructuresKHR = (PFN_vkCmdBuildAccelerationStructuresKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkCmdBuildAccelerationStructuresKHR");
    loci_vkBuildAccelerationStructuresKHR = (PFN_vkBuildAccelerationStructuresKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkBuildAccelerationStructuresKHR");
    loci_vkCreateAccelerationStructureKHR = (PFN_vkCreateAccelerationStructureKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkCreateAccelerationStructureKHR");
    loci_vkDestroyAccelerationStructureKHR = (PFN_vkDestroyAccelerationStructureKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkDestroyAccelerationStructureKHR");
    loci_vkGetAccelerationStructureBuildSizesKHR = (PFN_vkGetAccelerationStructureBuildSizesKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkGetAccelerationStructureBuildSizesKHR");
    loci_vkGetAccelerationStructureDeviceAddressKHR = (PFN_vkGetAccelerationStructureDeviceAddressKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkGetAccelerationStructureDeviceAddressKHR");
    loci_vkCmdTraceRaysKHR = (PFN_vkCmdTraceRaysKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkCmdTraceRaysKHR");
    loci_vkGetRayTracingShaderGroupHandlesKHR = (PFN_vkGetRayTracingShaderGroupHandlesKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkGetRayTracingShaderGroupHandlesKHR");
    loci_vkCreateRayTracingPipelinesKHR = (PFN_vkCreateRayTracingPipelinesKHR) vkGetDeviceProcAddr(loci_vkDevice, "vkCreateRayTracingPipelinesKHR");
    
}

void loci_createDevice()
{
        float queuePriority = 1.f;
        VkDeviceQueueCreateInfo deviceQueueCreateInfo;
        deviceQueueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
        deviceQueueCreateInfo.pNext = NULL;
        deviceQueueCreateInfo.flags = 0;
        deviceQueueCreateInfo.queueFamilyIndex = loci_queueFamilyIndex;
        deviceQueueCreateInfo.queueCount = 1;
        deviceQueueCreateInfo.pQueuePriorities = &queuePriority;
        deviceQueueCreateInfo.pNext = NULL;

        VkPhysicalDeviceVulkan12Features onetwoFeatures;
        onetwoFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;
        onetwoFeatures.bufferDeviceAddress = VK_TRUE;
        onetwoFeatures.bufferDeviceAddressCaptureReplay = VK_FALSE;
        onetwoFeatures.bufferDeviceAddressMultiDevice = VK_FALSE;
        onetwoFeatures.descriptorBindingPartiallyBound = VK_FALSE;
        onetwoFeatures.descriptorBindingSampledImageUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingStorageBufferUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingStorageImageUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingStorageTexelBufferUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingUniformBufferUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingUniformTexelBufferUpdateAfterBind = VK_FALSE;
        onetwoFeatures.descriptorBindingUpdateUnusedWhilePending = VK_FALSE;
        onetwoFeatures.descriptorBindingVariableDescriptorCount = VK_TRUE;
        onetwoFeatures.descriptorIndexing = VK_TRUE;
        onetwoFeatures.drawIndirectCount = VK_FALSE;
        onetwoFeatures.hostQueryReset = VK_FALSE;
        onetwoFeatures.imagelessFramebuffer = VK_FALSE;
        onetwoFeatures.pNext = NULL;
        onetwoFeatures.runtimeDescriptorArray = VK_TRUE;
        onetwoFeatures.samplerFilterMinmax = VK_FALSE;
        onetwoFeatures.samplerMirrorClampToEdge = VK_FALSE;
        onetwoFeatures.scalarBlockLayout = VK_FALSE;
        onetwoFeatures.separateDepthStencilLayouts = VK_FALSE;
        onetwoFeatures.shaderBufferInt64Atomics = VK_FALSE;
        onetwoFeatures.shaderFloat16 = VK_FALSE;
        onetwoFeatures.shaderInputAttachmentArrayDynamicIndexing = VK_FALSE;
        onetwoFeatures.shaderInputAttachmentArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.shaderInt8 = VK_FALSE;
        onetwoFeatures.shaderOutputLayer = VK_FALSE;
        onetwoFeatures.shaderOutputViewportIndex = VK_FALSE;
        onetwoFeatures.shaderSampledImageArrayNonUniformIndexing = VK_TRUE;
        onetwoFeatures.shaderSharedInt64Atomics = VK_FALSE;
        onetwoFeatures.shaderStorageBufferArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.shaderStorageImageArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.shaderStorageTexelBufferArrayDynamicIndexing = VK_FALSE;
        onetwoFeatures.shaderStorageTexelBufferArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.shaderSubgroupExtendedTypes = VK_FALSE;
        onetwoFeatures.shaderUniformBufferArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.shaderUniformTexelBufferArrayDynamicIndexing = VK_FALSE;
        onetwoFeatures.shaderUniformTexelBufferArrayNonUniformIndexing = VK_FALSE;
        onetwoFeatures.storageBuffer8BitAccess = VK_FALSE;
        onetwoFeatures.storagePushConstant8 = VK_FALSE;
        onetwoFeatures.subgroupBroadcastDynamicId = VK_FALSE;
        onetwoFeatures.timelineSemaphore = VK_FALSE;
        onetwoFeatures.uniformAndStorageBuffer8BitAccess = VK_FALSE;
        onetwoFeatures.uniformBufferStandardLayout = VK_FALSE;
        onetwoFeatures.vulkanMemoryModel = VK_FALSE;
        onetwoFeatures.vulkanMemoryModelAvailabilityVisibilityChains = VK_FALSE;
        onetwoFeatures.vulkanMemoryModelDeviceScope = VK_FALSE;

        VkPhysicalDeviceVulkan11Features elevenFeatures;
        elevenFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;
        elevenFeatures.multiview = VK_FALSE;
        elevenFeatures.multiviewGeometryShader = VK_FALSE;
        elevenFeatures.multiviewTessellationShader = VK_FALSE;
        elevenFeatures.pNext = &onetwoFeatures;
        elevenFeatures.protectedMemory = VK_FALSE;
        elevenFeatures.samplerYcbcrConversion = VK_FALSE;
        elevenFeatures.shaderDrawParameters = VK_TRUE;
        elevenFeatures.storageBuffer16BitAccess = VK_FALSE;
        elevenFeatures.storageInputOutput16 = VK_FALSE;
        elevenFeatures.storagePushConstant16 = VK_FALSE;
        elevenFeatures.uniformAndStorageBuffer16BitAccess = VK_FALSE;
        elevenFeatures.variablePointers  = VK_FALSE;
        elevenFeatures.variablePointersStorageBuffer  = VK_FALSE;
        
        VkPhysicalDeviceRayTracingPipelineFeaturesKHR rayFeatures;
        rayFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR;
        rayFeatures.pNext = &elevenFeatures;
        rayFeatures.rayTracingPipeline = VK_TRUE;
        rayFeatures.rayTracingPipelineShaderGroupHandleCaptureReplay = VK_FALSE;
        rayFeatures.rayTracingPipelineShaderGroupHandleCaptureReplayMixed = VK_FALSE;
        rayFeatures.rayTracingPipelineTraceRaysIndirect = VK_FALSE;
        rayFeatures.rayTracingPipelineTraceRaysIndirect = VK_FALSE;
        rayFeatures.rayTraversalPrimitiveCulling = VK_FALSE;

        VkPhysicalDeviceAccelerationStructureFeaturesKHR accFeatures;
        accFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
        accFeatures.accelerationStructure = VK_TRUE;
        accFeatures.accelerationStructureCaptureReplay = VK_FALSE;
        accFeatures.accelerationStructureHostCommands = VK_FALSE;
        accFeatures.accelerationStructureIndirectBuild = VK_FALSE;
        accFeatures.accelerationStructureIndirectBuild = VK_FALSE;
        accFeatures.descriptorBindingAccelerationStructureUpdateAfterBind = VK_FALSE;
        accFeatures.pNext = &rayFeatures;

        VkPhysicalDeviceFeatures2 features2;
        features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
        features2.features = loci_vkPhysicalDeviceFeatures;
        features2.pNext = &accFeatures;

        VkDeviceCreateInfo deviceCreateInfo;
        deviceCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        deviceCreateInfo.pNext = &features2;
        deviceCreateInfo.flags = 0;
        deviceCreateInfo.enabledLayerCount = 0;
        deviceCreateInfo.ppEnabledLayerNames = NULL;
        deviceCreateInfo.queueCreateInfoCount = 1;
        deviceCreateInfo.pQueueCreateInfos = &deviceQueueCreateInfo;
        deviceCreateInfo.pEnabledFeatures = NULL;
        deviceCreateInfo.enabledExtensionCount = loci_numberDeviceExtensions;
        deviceCreateInfo.ppEnabledExtensionNames = loci_deviceExtensions;

        LOCI_CHECK_VULKAN
        (vkCreateDevice
        (loci_vkPhysicalDevice, 
        &deviceCreateInfo, NULL, 
        &loci_vkDevice),
        "createVkDevice",
        "vkCreateDevice")

        #ifdef LOCI_DEBUG
        LOCI_LOGI("VkDevice", "created", "")
        #endif
 
        loci_loadRayFunctions();

        #ifdef LOCI_DEBUG
        LOCI_LOGI("RayFunctions", "loaded", "")
        #endif
}

void loci_destroyDevice()
{
    vkDestroyDevice
    (loci_vkDevice,
    NULL);
}