#define LOCI_DEBUG

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../cglm/include/cglm/cglm.h"

//C
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <time.h>

#define LOCI_LOGI(X, Y, Z)\
{\
    printf("Info@LOCI: %s :: %s :: %s\n", X, Y, Z);\
}

#define LOCI_LOGE(X, Y, Z)\
{\
    printf("Info@LOCI: %s :: %s :: %s\n", X, Y, Z);\
    exit(1);\
}

#define LOCI_CHECK_VULKAN(VulkanExpression, Function, VulkanFunction)\
{\
    VkResult vkRes;\
    if((vkRes = VulkanExpression) != VK_SUCCESS)\
    {\
        printf("Error@LOCI: %s :: %s\n", Function, VulkanFunction);\
        printf("Vulkan Error Code: %d\n", vkRes);\
        exit(1);\
    }\
}
