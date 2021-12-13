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

//==============================================================================
// useful macros
//==============================================================================

  #define BL_PAIR(cl,op)    (((cl)<<16) | ((op) & 0xffff))
  #define BL_MSG(o)         BL_PAIR(o->cl,o->op)

  #define BL_LENGTH(a)      (sizeof(a)/sizeof(a[0]))     // array length

//==============================================================================
// extern declarations
//==============================================================================

  extern bool bl_attention;            // attention mode
  extern bool bl_provisioned;          // provisioned mode

//==============================================================================
// sleep & wait
//==============================================================================

  BL_us bl_zero(void);                 // reset clock
  void bl_sleep(BL_ms ms);             // deep sleep for given milliseconds
  void bl_wait(BL_ms tick);            // wait until time tick

//==============================================================================
// event message emission, posting and notification
//==============================================================================

  int bl_out(BL_ob *o, int value, BL_fct call);
  int bl_in(BL_ob *o, int value);

//==============================================================================
// run system operation on a module (syntactic sugar: id = 0, val = 0)
//==============================================================================

  int bl_sys(int op, BL_fct module, BL_fct cb);

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_fct module, BL_fct cb, int val);    // init API or module
  void bl_loop(BL_fct module);                        // loop API or module

#endif // __BLAPI_H__
