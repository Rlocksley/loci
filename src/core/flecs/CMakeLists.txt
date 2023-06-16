cmake_minimum_required(VERSION 3.16.3)

project(loci)

add_library(loci_flecs STATIC
flecs.h
flecs.c)