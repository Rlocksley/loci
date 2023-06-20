cmake_minimum_required(VERSION 3.16.3)

project(loci)

set(SOURCES
    main.c
    snake.c
    snake.h
)

add_executable(loci_example3 ${SOURCES})

target_include_directories(loci_example3 PUBLIC ../../src/cglm/include)
target_link_libraries(loci_example3 PRIVATE loci_flecs loci_core loci_shapes loci_ecs loci_gltf m)
