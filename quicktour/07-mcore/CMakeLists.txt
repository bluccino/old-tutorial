# SPDX-License-Identifier: Apache-2.0

	cmake_minimum_required(VERSION 3.20.0)
	set(QEMU_EXTRA_FLAGS -s)

	find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#===============================================================================
# project definition and path setup
#===============================================================================

  project(07-mcore)

  set (LIB ../../lib/V0.6)
  set (BLU ${LIB}/bluccino)
  set (HWC ${LIB}/hwcore)              # hardware core
  set (WLC ${LIB}/mcore)               # wireless core
  set (LMO ${LIB}/module)              # library modules
  set (SRC src)

  set (MAIN ${SRC})
  set (CORE ${SRC}/core)

  include_directories(${SRC} ${BLU} ${HWC} ${LMO} ${MAIN} ${CORE})

  add_definitions(-DBL_CONFIG="config.h")   # push define down to C level
  add_definitions(-DBL_LOGGING="logging.h") # push define down to C level

#===============================================================================
# source files
#===============================================================================

target_sources(app PRIVATE
	       ${MAIN}/main.c
				 ${BLU}/bluccino.c
				 ${HWC}/bl_hw.c
				 ${WLC}/mcore.c
				 ${LMO}/bl_basis.c
	       )

#target_sources_ifdef(CONFIG_MCUMGR app PRIVATE ${CORE}/smp_svr.c)

#zephyr_include_directories(
#	src/
#	src/mesh
#)
