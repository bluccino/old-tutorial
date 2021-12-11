# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(QEMU_EXTRA_FLAGS -s)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(05-toggle)

include_directories(bluccino/include bluccino/cpp)

target_sources(app PRIVATE
               src/main.cpp
               bluccino/core/ocore4.c
               bluccino/src/blapi.c
               bluccino/src/bllog.c
               bluccino/cpp/bluccino.cpp
               )
