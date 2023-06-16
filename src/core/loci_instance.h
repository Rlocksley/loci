#ifndef LOCI_INSTANCE_H
#define LOCI_INSTANCE_H

#include "loci_global.h"


const char* loci_applicationName;


VkInstance loci_vkInstance;

#ifdef LOCI_DEBUG

uint32_t loci_numberValidationLayers = 1;
const char* loci_ppValidationLayers[1] = 
{"VK_LAYER_KHRONOS_validation"};

VkDebugUtilsMessengerEXT loci_vkDebugUtilsMessengerEXT;
#endif


void loci_createInstance();
void loci_destroyInstance();

VKAPI_ATTR VkBool32 VKAPI_CALL loci_debugCallback
(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
VkDebugUtilsMessageTypeFlagsEXT type,
const VkDebugUtilsMessengerCallbackDataEXT* callBackData,
void* userData);

#endif