#ifndef LOCI_GLOBAL_H
#define LOCI_GLOBAL_H
#include "loci_global.h"
#endif

uint32_t loci_windowWidth;
uint32_t loci_windowHeight;
const char* loci_windowTitle;
GLFWwindow* loci_pGLFWwindow;

void loci_createWindow();
void loci_destroyWindow();



void loci_createWindow()
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

    if((loci_pGLFWwindow = glfwCreateWindow
    (loci_windowWidth, loci_windowHeight, 
    loci_windowTitle, NULL, NULL)) == NULL)
    {
        LOCI_LOGI("createWindow", "glfwCreateWindow", "failed")
    }

    glfwSetInputMode(loci_pGLFWwindow, GLFW_STICKY_KEYS, GLFW_TRUE);
    glfwSetInputMode(loci_pGLFWwindow, GLFW_STICKY_MOUSE_BUTTONS, GLFW_TRUE);

    #ifdef LOCI_DEBUG
    LOCI_LOGI("GLFWwindow", "created", "")
    #endif
}

void loci_destroyWindow()
{
    glfwDestroyWindow(loci_pGLFWwindow);
    glfwTerminate();
}

