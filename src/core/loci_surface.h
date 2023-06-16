#ifndef LOCI_WINDOW_H
#define LOCI_WINDOW_H
#include "loci_window.h"
#endif
#ifndef LOCI_INSTANCE_H
#define LOCI_INSTANCE_H
#include "loci_instance.h"
#endif

VkSurfaceKHR loci_vkSurfaceKHR;

void loci_createSurface();
void loci_destroySurface();

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