#ifndef LOCI_WINDOW_H
#define LOCI_WINDOW_H

#include "loci_global.h"

extern uint32_t loci_windowWidth;
extern uint32_t loci_windowHeight;
extern const char* loci_windowTitle;
extern GLFWwindow* loci_pGLFWwindow;

void loci_createWindow();
void loci_destroyWindow();



#endif