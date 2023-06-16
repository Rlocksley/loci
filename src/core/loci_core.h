#ifndef LOCI_CORE_H
#define LOCI_CORE_H

#include "loci_window.h"
#include "loci_time.h"
#include "loci_input.h"
#include "loci_instance.h"
#include "loci_surface.h"
#include "loci_physicalDevice.h"
#include "loci_device.h"
#include "loci_queue.h"
#include "loci_swapchain.h"
#include "loci_semaphore.h"
#include "loci_fence.h"
#include "loci_commandPool.h"
#include "loci_commandBuffer.h"
#include "loci_descriptorPool.h"
#include "loci_image.h"
#include "loci_buffer.h"
#include "loci_eye.h"
#include "loci_camera.h"
#include "loci_shader.h"
#include "loci_world.h"

void loci_createCore();
void loci_destroyCore();
bool loci_updateCore();

#endif