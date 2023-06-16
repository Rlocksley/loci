#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H

#include "loci_physicalDevice.h"

uint32_t loci_numberDeviceExtensions = 6;
const char* loci_deviceExtensions[6] = 
{
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
    "VK_KHR_acceleration_structure",
    "VK_KHR_ray_tracing_pipeline",
    "VK_KHR_deferred_host_operations",
    "VK_KHR_buffer_device_address",
    "VK_EXT_descriptor_indexing"  
};

VkDevice loci_vkDevice;

PFN_vkGetBufferDeviceAddressKHR loci_vkGetBufferDeviceAddressKHR;  
PFN_vkCmdBuildAccelerationStructuresKHR loci_vkCmdBuildAccelerationStructuresKHR;  
PFN_vkBuildAccelerationStructuresKHR loci_vkBuildAccelerationStructuresKHR;     
PFN_vkCreateAccelerationStructureKHR loci_vkCreateAccelerationStructureKHR;     
PFN_vkDestroyAccelerationStructureKHR loci_vkDestroyAccelerationStructureKHR;    
PFN_vkGetAccelerationStructureBuildSizesKHR loci_vkGetAccelerationStructureBuildSizesKHR;  
PFN_vkGetAccelerationStructureDeviceAddressKHR loci_vkGetAccelerationStructureDeviceAddressKHR;   
PFN_vkCmdTraceRaysKHR loci_vkCmdTraceRaysKHR;    
PFN_vkGetRayTracingShaderGroupHandlesKHR loci_vkGetRayTracingShaderGroupHandlesKHR;     
PFN_vkCreateRayTracingPipelinesKHR loci_vkCreateRayTracingPipelinesKHR;   

void loci_createDevice();
void loci_destroyDevice();

#endif