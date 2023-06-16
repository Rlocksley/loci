#include "loci_physicalDevice.h"

bool loci_physicalDeviceIsComputeGraphicsPresent(VkPhysicalDevice device)
{
    uint32_t numberPropertiess;
    vkGetPhysicalDeviceQueueFamilyProperties
    (device,&numberPropertiess, NULL);
    
    if(numberPropertiess == 0)
    {
        return false;
    }

    VkQueueFamilyProperties propertiess[numberPropertiess];
    vkGetPhysicalDeviceQueueFamilyProperties
    (device,
    &numberPropertiess, propertiess);

    for(uint32_t i = 0; i < numberPropertiess; i++)
    {
        bool compute = false;
        bool graphics = false;
        bool present = false;
        
        if(propertiess[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            compute = true;
        }
        if(propertiess[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics = true;
        }
        
        VkBool32 vkPresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR
        (device, i,
        loci_vkSurfaceKHR,
        &vkPresent);
        
        if(vkPresent)
        {
            present = true;
        }

        if(compute&&graphics&&present)
        {
            return true;
        }
    }

    return false;
}

bool loci_physicalDeviceHasVkSurfaceFormatKHRSupport(VkPhysicalDevice device)
{
    uint32_t numberFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR
    (device,
    loci_vkSurfaceKHR,
    &numberFormats, NULL);

    return (numberFormats > 0);
}

bool loci_physicalDeviceHasVkPresentModeKHRSupport(VkPhysicalDevice device)
{
    uint32_t numberModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR
    (device,
    loci_vkSurfaceKHR,
    &numberModes, NULL);

    return (numberModes > 0);
}

bool loci_physicalDeviceHasVkAccelerationStructureSupport(VkPhysicalDevice device)
{
    VkPhysicalDeviceFeatures2 features;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accfeatures;
    accfeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    accfeatures.pNext = NULL;
    features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features.pNext = &accfeatures;

    vkGetPhysicalDeviceFeatures2(device, &features);

    return (accfeatures.accelerationStructure == VK_TRUE);
}

bool loci_physicalDeviceHasVkSamplerAnisotropySupport(VkPhysicalDevice device)
{
    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures
    (device, &features);

    return (features.samplerAnisotropy == VK_TRUE);
}

bool vkPhysicalDeviceFullfillsRequirements(VkPhysicalDevice device)
{
    return loci_physicalDeviceIsComputeGraphicsPresent(device) &&
    loci_physicalDeviceHasVkSurfaceFormatKHRSupport(device) &&
    loci_physicalDeviceHasVkPresentModeKHRSupport(device) &&
    loci_physicalDeviceHasVkAccelerationStructureSupport(device) &&
    loci_physicalDeviceHasVkSamplerAnisotropySupport(device);
}

void loci_pickQueueFamilyIndex()
{
    uint32_t numberPropertiess;
    vkGetPhysicalDeviceQueueFamilyProperties
    (loci_vkPhysicalDevice,&numberPropertiess, NULL);
    
    if(numberPropertiess == 0)
    {
        LOCI_LOGE("pickVkPhysicalDevice", "no VkQueueFamilyPropertiess", "found")
    }

    VkQueueFamilyProperties propertiess[numberPropertiess];
    vkGetPhysicalDeviceQueueFamilyProperties
    (loci_vkPhysicalDevice,
    &numberPropertiess, propertiess);

    for(uint32_t i = 0; i < numberPropertiess; i++)
    {
        bool compute = false;
        bool graphics = false;
        bool present = false;
        
        if(propertiess[i].queueFlags & VK_QUEUE_COMPUTE_BIT)
        {
            compute = true;
        }
        if(propertiess[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            graphics = true;
        }
        
        VkBool32 vkPresent = false;
        vkGetPhysicalDeviceSurfaceSupportKHR
        (loci_vkPhysicalDevice, i,
        loci_vkSurfaceKHR,
        &vkPresent);
        
        if(vkPresent)
        {
            present = true;
        }

        if(compute&&graphics&&present)
        {
            loci_queueFamilyIndex = i;
            return;
        }
    }
    
    LOCI_LOGE("pickQueueFamilyIndex", "no suitable queueFamilyIndex", "found")
}

void loci_pickVkSurfaceFormatKHR()
{
    uint32_t numberFormats;
    vkGetPhysicalDeviceSurfaceFormatsKHR
    (loci_vkPhysicalDevice, loci_vkSurfaceKHR, &numberFormats, NULL);

    VkSurfaceFormatKHR formats[numberFormats];
    vkGetPhysicalDeviceSurfaceFormatsKHR
    (loci_vkPhysicalDevice, loci_vkSurfaceKHR, &numberFormats, formats);

    for(uint32_t i = 0; i < numberFormats; i++)
    {
        if(formats[i].format == VK_FORMAT_B8G8R8A8_UNORM &&
        formats[i].colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
        {
            loci_vkSurfaceFormatKHR = formats[i];
            return;
        }
    }
    loci_vkSurfaceFormatKHR = formats[0];
}

void loci_pickVkPresentModeKHR()
{
    uint32_t numberPresentModes;
    vkGetPhysicalDeviceSurfacePresentModesKHR
    (loci_vkPhysicalDevice, loci_vkSurfaceKHR, &numberPresentModes, NULL);

    if(numberPresentModes == 0)
    {
        LOCI_LOGE("pickVkPresentModeKHR", "no VkPresentModeKHR", "found")
    }

    VkPresentModeKHR modes[numberPresentModes];
    vkGetPhysicalDeviceSurfacePresentModesKHR
    (loci_vkPhysicalDevice, loci_vkSurfaceKHR, &numberPresentModes, modes);

    for(size_t i = 0; i < numberPresentModes; i++)
    {
        if(modes[i] == VK_PRESENT_MODE_MAILBOX_KHR)
        {
            loci_vkPresentModeKHR =  modes[i];
            return;
        }
    }
    loci_vkPresentModeKHR = modes[0];
}

uint32_t loci_clamp(uint32_t value, uint32_t minValue, uint32_t maxValue)
{
    return (value)*(minValue<=value&&value<=maxValue)+
    (minValue)*(value<minValue) + (maxValue)*(maxValue<value);
}

void loci_chooseVkExtent2D()
{
    VkSurfaceCapabilitiesKHR capab;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(loci_vkPhysicalDevice, loci_vkSurfaceKHR, &capab);

    if(capab.currentExtent.width != UINT32_MAX &&
    capab.currentExtent.height != UINT32_MAX)
    {
        loci_vkExtent2D =  capab.currentExtent;
        return;
    }

    int width, height;
    glfwGetFramebufferSize
    (loci_pGLFWwindow, &width, &height);

    VkExtent2D extent;
    extent.width = (uint32_t)width;
    extent.height = (uint32_t)height;

    extent.width =
    loci_clamp(extent.width,
    capab.minImageExtent.width,
    capab.maxImageExtent.width);
    extent.height =
    loci_clamp(extent.height,
    capab.minImageExtent.height,
    capab.maxImageExtent.height);

    loci_vkExtent2D = extent;
}

void loci_pickPhysicalDevice()
{
    uint32_t physicalDeviceCount = 0;
    vkEnumeratePhysicalDevices
    (loci_vkInstance, &physicalDeviceCount, NULL);

    if(physicalDeviceCount == 0)
    {
        LOCI_LOGE("pickVkPhysicalDevices", "no VkPhysicalDevice", "found")
    }

    VkPhysicalDevice physicalDevices[physicalDeviceCount];
    vkEnumeratePhysicalDevices
    (loci_vkInstance, &physicalDeviceCount, physicalDevices);

    bool found = false;
    for(size_t i = 0; i < physicalDeviceCount; i++)
    {
        if(vkPhysicalDeviceFullfillsRequirements(physicalDevices[i]))
        {
            loci_vkPhysicalDevice = physicalDevices[i];
            found = true;
            break;
        }
    }
    if(!found)
    {
        LOCI_LOGE("pickVkPhysicalDevice", "no suitable VkPhysicalDevice", "found")
    }

    #ifdef LOCI_DEBUG
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(loci_vkPhysicalDevice, &props);
    LOCI_LOGI("VkPhysicalDevice", props.deviceName, "picked")
    #endif

    loci_pickQueueFamilyIndex();

    loci_pickVkSurfaceFormatKHR();

    loci_pickVkPresentModeKHR();

    loci_chooseVkExtent2D();

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkPhysicalDeviceInfo", "filled", "")
    #endif

    VkPhysicalDeviceFeatures features;
    vkGetPhysicalDeviceFeatures(loci_vkPhysicalDevice, &features);
    loci_vkPhysicalDeviceFeatures = features;

    VkPhysicalDeviceFeatures2 features2;
    VkPhysicalDeviceAccelerationStructureFeaturesKHR accFeatures;
    accFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR;
    accFeatures.pNext = NULL;
    features2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;
    features2.pNext = &accFeatures;
    vkGetPhysicalDeviceFeatures2(loci_vkPhysicalDevice, &features2);

    VkPhysicalDeviceProperties2 properties2;
    VkPhysicalDeviceRayTracingPipelinePropertiesKHR rayProperties;
    rayProperties.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR;
    rayProperties.pNext = NULL;
    properties2.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2;
    properties2.pNext = &rayProperties;
    vkGetPhysicalDeviceProperties2(loci_vkPhysicalDevice, &properties2);
    loci_vkPhysicalDeviceRayTracingPipelinePropertiesKHR = rayProperties;
    
    #ifdef LOCI_DEBUG
    LOCI_LOGI("Ray Tracing", "activated", "");
    #endif

    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(loci_vkPhysicalDevice, &properties);
    loci_vkPhysicalDeviceProperties = properties;
    
    VkPhysicalDeviceMemoryProperties memProps;
    vkGetPhysicalDeviceMemoryProperties
    (loci_vkPhysicalDevice, &memProps);
    loci_vkPhysicalDeviceMemoryProperties = memProps;

    VkSurfaceCapabilitiesKHR capab;
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR
    (loci_vkPhysicalDevice, loci_vkSurfaceKHR, &capab);
    loci_vkSurfaceCapabilitiesKHR = capab;
}

uint32_t loci_findMemoryType
(uint32_t typeFilter, VkMemoryPropertyFlags properties)
{
   
    for(uint32_t i = 0; i < loci_vkPhysicalDeviceMemoryProperties.memoryTypeCount; i++)
    {
        if((typeFilter&(1<<i)) &&
        (loci_vkPhysicalDeviceMemoryProperties.memoryTypes[i].propertyFlags & properties)
        == properties)
        {
            return i;
        }
    }

    LOCI_LOGE("findMemoryType", "to find suitable memory type", "failed")
}