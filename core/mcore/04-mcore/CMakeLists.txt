# SPDX-License-Identifier: Apache-2.0

	cmake_minimum_required(VERSION 3.20.0)
	set(QEMU_EXTRA_FLAGS -s)

	find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

#===============================================================================
# project definition and path setup
#===============================================================================

  project(04-mcore)

  set (LIB ../../../lib/V0.6)
  set (BLU ${LIB}/bluccino)
  set (SRC src)

  set (MAIN ${SRC}/main)
  set (CORE ${SRC}/core)

  include_directories(${SRC} ${BLU} ${MAIN} ${CORE})

  add_definitions(-DBL_CONFIG="config.h")   # push define down to C level
  add_definitions(-DBL_LOGGING="logging.h") # push define down to C level

#===============================================================================
# source files
#===============================================================================

target_sources(app PRIVATE
	       ${MAIN}/main.c
				 ${BLU}/bluccino.c
	       ${CORE}/mcore.c
	       ${CORE}/app_gpio.c
	       ${CORE}/storage.c
	       ${CORE}/ble_mesh.c
	       ${CORE}/device_composition.c
	       ${CORE}/no_transition_work_handler.c
	       ${CORE}/publisher.c
	       ${CORE}/state_binding.c
	       ${CORE}/transition.c
	       )

target_sources_ifdef(CONFIG_MCUMGR app PRIVATE ${CORE}/smp_svr.c)

zephyr_include_directories(
	src/
	src/mesh
)
