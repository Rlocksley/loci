#ifndef LOCI_WINDOW_H
#define LOCI_WINDOW_H

#include "loci_global.h"

uint32_t loci_windowWidth;
uint32_t loci_windowHeight;
const char* loci_windowTitle;
GLFWwindow* loci_pGLFWwindow;

void loci_createWindow();
void loci_destroyWindow();



#endif