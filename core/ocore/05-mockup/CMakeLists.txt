# SPDX-License-Identifier: Apache-2.0

cmake_minimum_required(VERSION 3.20.0)
set(QEMU_EXTRA_FLAGS -s)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#===============================================================================
# project definition and path setup
#===============================================================================

  project(05-mockup)

  set (LIB ../../../lib/V0.6)
  set (BLU ${LIB}/bluccino)
  set (HWC ${LIB}/hwcore)
  set (SRC src)

  include_directories(${SRC} ${BLU} ${HWC})

  target_sources(app PRIVATE
               ${SRC}/main.c
               ${BLU}/bluccino.c
              )
