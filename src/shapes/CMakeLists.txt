cmake_minimum_required(VERSION 3.16.3)

project(loci)

add_library(loci_shapes STATIC
loci_cube.h
loci_cube.c
loci_sphere.h
loci_sphere.c
loci_tetra.h
loci_tetra.c
)

target_include_directories(loci_shapes PUBLIC  ../cglm/include)