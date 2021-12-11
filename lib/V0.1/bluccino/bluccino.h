//==============================================================================
//  bluccino.h
//  Bluccino overall header (for ZEPHYR RTOS)
//
//  Created by Hugo Pristauz on 2021-11-06
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLUCCINO_H__
#define __BLUCCINO_H__

#ifdef INCLUDE
  #include INCLUDE                     // possibility to include another header
#endif

  #include "blapi.h"

//==============================================================================
// Bluccino Core Migration
//==============================================================================

  #define MIGRATION_STEP1         1    // introduce bl_core_init(), bl_core_loop

#endif // __BLUCCINO_H__
