//==============================================================================
//  blapi.c - Bluccino API layer
//  Bluccino debug logging
//
//  Created by Hugo Pristauz on 2021-11-19
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLAPI_H__
#define __BLAPI_H__

#include "blrtos.h"
#include "bltype.h"
#include "bllog.h"

   extern bool bl_attention;           // attention mode
   extern bool bl_provisioned;         // provisioned mode

//==============================================================================
// mesh opcodes
//==============================================================================

  #define BL_GOOLET 0x8203

//==============================================================================
// timing & sleep
//==============================================================================

  void bl_sleep(int ms);               // deep sleep for given milliseconds

//==============================================================================
// event message emission, posting and notification
//==============================================================================

  int bl_out(BL_ob *o, int value, BL_notify call);
  int bl_in(BL_ob *o, int value);

//==============================================================================
// gear input/output (post) functions
//==============================================================================

  int bl_input(BL_ob *o, int value);   // gear input function
  int bl_output(BL_ob *o, int value);  // gear output function

  void bl_driver(BL_notify cb);        // set gear driver

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_core_init(BL_notify cb);
  void bl_core_loop(void);

  void bl_gear_init(BL_notify cb);
  void bl_gear_loop(void);

  void bl_init(BL_notify cb, int verbose);
  void bl_loop(void);

#endif // __BLAPI_H__
