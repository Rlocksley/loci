#include "loci_surface.h"

void loci_createSurface()
{
    LOCI_CHECK_VULKAN
    (glfwCreateWindowSurface
    (loci_vkInstance, loci_pGLFWwindow,
    NULL,
    &loci_vkSurfaceKHR),
    "createSurface",
    "glfwCreateWindowSurface")

    #ifdef LOCI_DEBUG
    LOCI_LOGI("VkSurfaceKHR", "created", "")
    #endif
}

void loci_destroySurface()
{
    vkDestroySurfaceKHR
    (loci_vkInstance,
    loci_vkSurfaceKHR,
    NULL);
}