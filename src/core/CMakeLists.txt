cmake_minimum_required(VERSION 3.16.3)

project(loci)

add_library(loci_core STATIC
loci_global.h
loci_window.h
loci_window.c
loci_time.h
loci_time.c
loci_input.h
loci_input.c
loci_instance.h
loci_instance.c
loci_surface.h
loci_surface.c
loci_physicalDevice.h
loci_physicalDevice.c
loci_device.h
loci_device.c
loci_queue.h
loci_queue.c
loci_swapchain.h
loci_swapchain.c
loci_semaphore.h
loci_semaphore.c
loci_fence.h
loci_fence.c
loci_commandPool.h
loci_commandPool.c
loci_commandBuffer.h
loci_commandBuffer.c
loci_descriptorPool.h
loci_descriptorPool.c
loci_descriptorSet.h
loci_descriptorSet.c
loci_acceleration.h
loci_acceleration.c
loci_buffer.h
loci_buffer.c
loci_image.h
loci_image.c
loci_pipeline.h
loci_pipeline.c
loci_shader.h
loci_shader.c
loci_shaderTables.h
loci_shaderTables.c
loci_eye.h
loci_eye.c
loci_camera.h
loci_camera.c
loci_world.h
loci_world.c
loci_core.h
loci_core.c
)

target_include_directories(loci_core PUBLIC ../cglm/include ../stb)
target_link_libraries(loci_core PUBLIC glfw vulkan loci_flecs)