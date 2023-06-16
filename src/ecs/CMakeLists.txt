cmake_minimum_required(VERSION 3.16.3)

project(loci)

add_library(loci_ecs STATIC
loci_transform.h
loci_transform.c
loci_pointLight.h
loci_pointLight.c
loci_mesh.h
loci_mesh.c
loci_material.h
loci_material.c
loci_texture.h
loci_texture.c
loci_scene.h
loci_scene.c
loci_ecs.h
)

target_include_directories(loci_ecs PUBLIC ../cglm/include  ../stb)
target_link_libraries(loci_ecs PUBLIC glfw vulkan loci_flecs loci_core)