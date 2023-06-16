#include "loci_instance.h"

const char* loci_applicationName;


VkInstance loci_vkInstance;

#ifdef LOCI_DEBUG

uint32_t loci_numberValidationLayers = 1;
const char* loci_ppValidationLayers[1] = 
{"VK_LAYER_KHRONOS_validation"};

VkDebugUtilsMessengerEXT loci_vkDebugUtilsMessengerEXT;
#endif


VKAPI_ATTR VkBool32 VKAPI_CALL loci_debugCallback
(VkDebugUtilsMessageSeverityFlagBitsEXT severity,
VkDebugUtilsMessageTypeFlagsEXT type,
const VkDebugUtilsMessengerCallbackDataEXT* callBackData,
void* userData)
{
    (void) (severity);
    (void) (type);
    (void) (userData);

    printf("%s\n", callBackData->pMessage);

    return VK_FALSE;
}

#ifdef LOCI_DEBUG
void loci_checkValidationLayerSupport()
{
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, NULL);
    VkLayerProperties availableLayers[layerCount];
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers);

    for(uint32_t i = 0; i < loci_numberValidationLayers; i++)
    {
        bool layerFound = false;
        for(uint32_t j = 0; j < layerCount; j++)
        {
            if(strcmp(loci_ppValidationLayers[i], availableLayers[j].layerName) == 0)
            {
                layerFound = true;
                break;
            }
        }

        if(!layerFound)
        {
            LOCI_LOGE("checkValidationLayerSupport", "no validationLayerSupport for", loci_ppValidationLayers[i])
        }
    }
}

void createVkDebugUtilsMessengerEXT()
{
    PFN_vkCreateDebugUtilsMessengerEXT func =
    (PFN_vkCreateDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(loci_vkInstance,"vkCreateDebugUtilsMessengerEXT");

    if(func == NULL)
    {
        LOCI_LOGE("createVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
    }

    VkDebugUtilsMessengerCreateInfoEXT vkDebugUtilsMessengerCreateInfoEXT;
    vkDebugUtilsMessengerCreateInfoEXT.sType =
    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.flags = 0;
    vkDebugUtilsMessengerCreateInfoEXT.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.pfnUserCallback =
    &loci_debugCallback;
    vkDebugUtilsMessengerCreateInfoEXT.pNext = NULL;

    if(func
    (loci_vkInstance,
    &vkDebugUtilsMessengerCreateInfoEXT,
    NULL,
    &loci_vkDebugUtilsMessengerEXT) != VK_SUCCESS)
    {
        LOCI_LOGE("createVkDebugUtilsMessengerEXT", "vkCreateDebugUtilsMessengerEXT()", "failed");
    }

}

void destroyVkDebugUtilsMessengerEXT()
{
    PFN_vkDestroyDebugUtilsMessengerEXT func =
    (PFN_vkDestroyDebugUtilsMessengerEXT)
    vkGetInstanceProcAddr(loci_vkInstance, "vkDestroyDebugUtilsMessengerEXT");

    if(func == NULL)
    {
        LOCI_LOGE("destroyVkDebugUtilsMessengerEXT", "vkGetInstanceProcAddr()", "failed");
    }
    
    func
    (loci_vkInstance,
    loci_vkDebugUtilsMessengerEXT,
    NULL);
    
}

#endif

void loci_createInstance()
{
    #ifdef LOCI_DEBUG
    loci_checkValidationLayerSupport();
    #endif

    VkApplicationInfo appInfo;
    appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    appInfo.apiVersion = VK_API_VERSION_1_3;
    appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
    appInfo.pApplicationName = loci_applicationName;
    appInfo.pEngineName = "LOCI";
    appInfo.pNext = NULL;

    VkInstanceCreateInfo createInfo;
    createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.flags = 0;
    createInfo.pApplicationInfo = &appInfo;

    
    #ifdef LOCI_DEBUG

    uint32_t glfwExtensionsCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    const char* instanceExtensions[glfwExtensionsCount+1];
    memcpy(instanceExtensions, glfwExtensions, glfwExtensionsCount*sizeof(const char*));
    instanceExtensions[glfwExtensionsCount] = VK_EXT_DEBUG_UTILS_EXTENSION_NAME;
    createInfo.enabledExtensionCount = glfwExtensionsCount+1;
    createInfo.ppEnabledExtensionNames = instanceExtensions;

    createInfo.enabledLayerCount = loci_numberValidationLayers;
    createInfo.ppEnabledLayerNames = loci_ppValidationLayers;

    VkDebugUtilsMessengerCreateInfoEXT vkDebugUtilsMessengerCreateInfoEXT;
    vkDebugUtilsMessengerCreateInfoEXT.sType =
    VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.flags = 0;
    vkDebugUtilsMessengerCreateInfoEXT.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
    vkDebugUtilsMessengerCreateInfoEXT.pfnUserCallback =
    &loci_debugCallback;
    vkDebugUtilsMessengerCreateInfoEXT.pNext = NULL;

    createInfo.pNext = &vkDebugUtilsMessengerCreateInfoEXT;
    
    #else

    uint32_t glfwExtensionsCount;
    const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionsCount);
    createInfo.enabledExtensionCount = glfwExtensionsCount;
    createInfo.ppEnabledExtensionNames = glfwExtensions;

    createInfo.enabledLayerCount = 0;
    createInfo.ppEnabledLayerNames = NULL;
    createInfo.pNext = NULL;

    #endif

    LOCI_CHECK_VULKAN
    (vkCreateInstance
    (&createInfo, NULL,
    &loci_vkInstance),
    "createInstance",
    "vkCreateInstance")

    #ifdef LOCI_DEBUG
    
    LOCI_LOGI("VkInstance", "created", "")
    
    createVkDebugUtilsMessengerEXT();

    LOCI_LOGI("VkDebugUtilsMessengerEXT", "created", "")
    
    #endif
}

void loci_destroyInstance()
{
    #ifdef LOCI_DEBUG

    destroyVkDebugUtilsMessengerEXT();

    #endif

    vkDestroyInstance(loci_vkInstance, NULL);
}

