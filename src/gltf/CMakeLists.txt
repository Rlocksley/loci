cmake_minimum_required(VERSION 3.16.3)

project(loci)

add_library(loci_gltf STATIC
loci_gltf.h
loci_gltf.c
)

target_include_directories(loci_gltf PUBLIC ../../src/cglm/include)
target_link_libraries(loci_gltf PUBLIC loci_flecs loci_core loci_ecs)