cmake_minimum_required(VERSION 3.12)
project(yoho)

set(CMAKE_CXX_STANDARD 14)

include_directories(include)

add_executable(yoho
    boot/boot.s
    include/types.h
    init/entry.c
    scripts/kernel.ld
    floppy.img
    floppy.img.back
    LICENSE
    Makefile
    README.md)
