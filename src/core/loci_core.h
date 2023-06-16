#ifndef LOCI_WINDOW_H
#define LOCI_WINDOW_H
#include "loci_window.h"
#endif
#ifndef LOCI_TIME_H
#define LOCI_TIME_H
#include "loci_time.h"
#endif
#ifndef LOCI_INPUT_H
#define LOCI_INPUT_H
#include "loci_input.h"
#endif
#ifndef LOCI_INSTANCE_H
#define LOCI_INSTANCE_H
#include "loci_instance.h"
#endif
#ifndef LOCI_SURFACE_H
#define LOCI_SURFACE_H
#include "loci_surface.h"
#endif
#ifndef LOCI_PHYSICALDEVICE_H
#define LOCI_PHYSICALDEVICE_H
#include "loci_physicalDevice.h"
#endif
#ifndef LOCI_DEVICE_H
#define LOCI_DEVICE_H
#include "loci_device.h"
#endif
#ifndef LOCI_QUEUE_H
#define LOCI_QUEUE_H
#include "loci_queue.h"
#endif
#ifndef LOCI_SWAPCHAIN_H
#define LOCI_SWAPCHAIN_H
#include "loci_swapchain.h"
#endif
#ifndef LOCI_SEMAPHORE_H
#define LOCI_SEMAPHORE_H
#include "loci_semaphore.h"
#endif
#ifndef LOCI_FENCE_H
#define LOCI_FENCE_H
#include "loci_fence.h"
#endif
#ifndef LOCI_COMMANDPOOL_H
#define LOCI_COMMANDPOOL_H
#include "loci_commandPool.h"
#endif
#ifndef LOCI_COMMANDBUFFER_H
#define LOCI_COMMANDBUFFER_H
#include "loci_commandBuffer.h"
#endif
#ifndef LOCI_DESCRIPTORPOOL_H
#define LOCI_DESCRIPTORPOOL_H
#include "loci_descriptorPool.h"
#endif
#ifndef LOCI_IMAGE_H
#define LOCI_IMAGE_H
#include "loci_image.h"
#endif
#ifndef LOCI_BUFFER_H
#define LOCI_BUFFER_H
#include "loci_buffer.h"
#endif
#ifndef LOCI_EYE_H
#define LOCI_EYE_H
#include "loci_eye.h"
#endif
#ifndef LOCI_CAMERA_H
#define LOCI_CAMERA_H
#include "loci_camera.h"
#endif
#ifndef LOCI_SHADER_H
#define LOCI_SHADER_H
#include "loci_shader.h"
#endif
#ifndef LOCI_WORLD_H
#define LOCI_WORLD_H
#include "loci_world.h"
#endif

void loci_createCore();
void loci_destroyCore();
bool loci_updateCore();

void loci_createCore()
{
    //GLFW
    loci_createWindow();
    loci_createTime();
    loci_createCursor();

    //Vulkan Base
    loci_createInstance();
    loci_createSurface();
    loci_pickPhysicalDevice();
    loci_createDevice();
    loci_getQueue();

    //Vulkan Present
    loci_createSwapchain();  

    //Vulkan Sync
    loci_createSemaphore(&loci_imageIndexVkSemaphore);
    loci_createSemaphore(&loci_animationUpdateVkSemaphore);
    loci_createSemaphore(&loci_bottomAccelerationUpdateVkSemaphore);
    loci_createSemaphore(&loci_topAccelerationUpdateVkSemaphore);
    loci_createSemaphore(&loci_drawVkSemaphore);
    loci_createFence(&loci_buildVkFence, 0);
    loci_createFence(&loci_drawVkFence, 0);

    //Vulkan Command
    loci_createCommandPool();
    loci_createCommandBuffer(&loci_buildVkCommandBuffer);
    loci_createCommandBuffer(&loci_animationUpdateVkCommandBuffer);
    loci_createCommandBuffer(&loci_bottomAccelerationUpdateVkCommandBuffer);
    loci_createCommandBuffer(&loci_topAccelerationUpdateVkCommandBuffer);
    loci_createCommandBuffer(&loci_drawVkCommandBuffer);

    //Vulkan Descriptors
    loci_createDescriptorPool();

    //Vulkan RayImage
    loci_createRayImage();

    //Vulkan Eye
    loci_eyeBuffer = 
    loci_createBufferInterface
    (1,sizeof(Loci_Eye),
    VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT);

    //Flecs World
    loci_createWorld()
}

void loci_destroyCore()
{
    //Flecs World
    loci_destroyWorld()

    //Vulkan Eye
    loci_destroyBufferInterface(loci_eyeBuffer);

    //Vulkan RayImage
    loci_destroyRayImage();

    //Vulkan Descriptors
    loci_destroyDescriptorPool();

    //Vulkan Command
    loci_destroyCommandBuffer(&loci_drawVkCommandBuffer);
    loci_destroyCommandBuffer(&loci_topAccelerationUpdateVkCommandBuffer);
    loci_destroyCommandBuffer(&loci_bottomAccelerationUpdateVkCommandBuffer);
    loci_destroyCommandBuffer(&loci_animationUpdateVkCommandBuffer);
    loci_destroyCommandBuffer(&loci_buildVkCommandBuffer);
    loci_destroyCommandPool();

    //Vulkan Sync
    loci_destroyFence(loci_drawVkFence);
    loci_destroyFence(loci_buildVkFence);
    loci_destroySemaphore(loci_drawVkSemaphore);
    loci_destroySemaphore(loci_topAccelerationUpdateVkSemaphore);
    loci_destroySemaphore(loci_bottomAccelerationUpdateVkSemaphore);
    loci_destroySemaphore(loci_animationUpdateVkSemaphore);
    loci_destroySemaphore(loci_imageIndexVkSemaphore);

    //Vulkan Present
    loci_destroySwapchain();

    //Vulkan Base
    loci_destroyDevice();
    loci_destroySurface();
    loci_destroyInstance();

    //GLFW
    loci_destroyWindow();
}

bool loci_updateCore()
{
    glfwPollEvents();

    loci_updateTime();

    loci_updateKeys();
    loci_updateButtons();
    loci_updateCursor();

    return !glfwWindowShouldClose(loci_pGLFWwindow);
}