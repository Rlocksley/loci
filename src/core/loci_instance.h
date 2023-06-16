#ifndef LOCI_INSTANCE_H
#define LOCI_INSTANCE_H

#include "loci_global.h"


extern const char* loci_applicationName;


extern VkInstance loci_vkInstance;

#ifdef LOCI_DEBUG

extern uint32_t loci_numberValidationLayers;
extern const char* loci_ppValidationLayers[1];

extern VkDebugUtilsMessengerEXT loci_vkDebugUtilsMessengerEXT;
#endif


void loci_createInstance();
void loci_destroyInstance();

VKAPI_ATTR VkBool32 VKAPI_CALL loci_debugCallback
(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
VkDebugUtilsMessageTypeFlagsEXT type,
const VkDebugUtilsMessengerCallbackDataEXT* callBackData,
void* userData);

#endif