cmake_minimum_required(VERSION 3.16.3)

project(loci)


add_executable(loci_example2 main.c)

target_include_directories(loci_example2 PUBLIC ../../src/cglm/include)
target_link_libraries(loci_example2 PRIVATE loci_flecs loci_core loci_shapes loci_ecs loci_gltf m)
