#include "loci_core.h"



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
    loci_createSemaphore(&loci_skeletonUpdateVkSemaphore);
    loci_createSemaphore(&loci_bottomAccelerationUpdateVkSemaphore);
    loci_createSemaphore(&loci_topAccelerationUpdateVkSemaphore);
    loci_createSemaphore(&loci_drawVkSemaphore);
    loci_createFence(&loci_buildVkFence, 0);
    loci_createFence(&loci_drawVkFence, 0);

    //Vulkan Command
    loci_createCommandPool();
    loci_createCommandBuffer(&loci_buildVkCommandBuffer);
    loci_createCommandBuffer(&loci_skeletonUpdateVkCommandBuffer);
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

    //Skeleton Shader
    loci_skeletonShader = loci_createShader("skeletonShader.spv");
}

void loci_destroyCore()
{
    //Skeleton Shader
    loci_destroyShader(loci_skeletonShader);

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
    loci_destroyCommandBuffer(&loci_skeletonUpdateVkCommandBuffer);
    loci_destroyCommandBuffer(&loci_buildVkCommandBuffer);
    loci_destroyCommandPool();

    //Vulkan Sync
    loci_destroyFence(loci_drawVkFence);
    loci_destroyFence(loci_buildVkFence);
    loci_destroySemaphore(loci_drawVkSemaphore);
    loci_destroySemaphore(loci_topAccelerationUpdateVkSemaphore);
    loci_destroySemaphore(loci_bottomAccelerationUpdateVkSemaphore);
    loci_destroySemaphore(loci_skeletonUpdateVkSemaphore);
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