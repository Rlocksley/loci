cmake_minimum_required(VERSION 3.16.3)

project(loci)

option(loci_examples "Build Examples" ON)


set_property(GLOBAL PROPERTY C_STANDARD 99)

include_directories(./src/glm/include)

add_subdirectory(./src/core/flecs)
add_subdirectory(./src/core)
add_subdirectory(./src/ecs)
add_subdirectory(./src/shapes)
add_subdirectory(./src/gltf)

if(loci_examples)
    add_subdirectory(examples/example1)
    add_subdirectory(examples/example2)
    add_subdirectory(examples/example3)
endif()

