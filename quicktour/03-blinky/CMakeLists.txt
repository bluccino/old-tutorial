# SPDX-License-Identifier: Apache-2.0

  cmake_minimum_required(VERSION 3.20.0)
  find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
# set(QEMU_EXTRA_FLAGS -s)

#===============================================================================
# project definition and path setup
#===============================================================================

  project(03-blinky)

  set (LIB ../../lib/V0.6)
  set (BLU ${LIB}/bluccino)
  set (HWC ${LIB}/hwcore)
  set (SRC src)

  include_directories(${SRC} ${BLU} ${HWC})

#===============================================================================
# source files
#===============================================================================

  target_sources(app PRIVATE
                 ${SRC}/main.c
                 ${BLU}/bluccino.c
                 ${HWC}/bl_hw.c
                )
