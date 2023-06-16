#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H

#include "loci_physicalDevice.h"

extern uint32_t loci_numberDeviceExtensions;
extern const char* loci_deviceExtensions[6];

extern VkDevice loci_vkDevice;

extern PFN_vkGetBufferDeviceAddressKHR loci_vkGetBufferDeviceAddressKHR;  
extern PFN_vkCmdBuildAccelerationStructuresKHR loci_vkCmdBuildAccelerationStructuresKHR;  
extern PFN_vkBuildAccelerationStructuresKHR loci_vkBuildAccelerationStructuresKHR;     
extern PFN_vkCreateAccelerationStructureKHR loci_vkCreateAccelerationStructureKHR;     
extern PFN_vkDestroyAccelerationStructureKHR loci_vkDestroyAccelerationStructureKHR;    
extern PFN_vkGetAccelerationStructureBuildSizesKHR loci_vkGetAccelerationStructureBuildSizesKHR;  
extern PFN_vkGetAccelerationStructureDeviceAddressKHR loci_vkGetAccelerationStructureDeviceAddressKHR;   
extern PFN_vkCmdTraceRaysKHR loci_vkCmdTraceRaysKHR;    
extern PFN_vkGetRayTracingShaderGroupHandlesKHR loci_vkGetRayTracingShaderGroupHandlesKHR;     
extern PFN_vkCreateRayTracingPipelinesKHR loci_vkCreateRayTracingPipelinesKHR;   

void loci_createDevice();
void loci_destroyDevice();

#endif