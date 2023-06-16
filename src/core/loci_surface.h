#ifndef LOCI_SURFACE_H
#define LOCI_SURFACE_H

#include "loci_window.h"
#include "loci_instance.h"

VkSurfaceKHR loci_vkSurfaceKHR;

void loci_createSurface();
void loci_destroySurface();

#endif