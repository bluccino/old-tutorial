//==============================================================================
//  bllog.h
//  Bluccino logging and us/ms clock support
//
//  Created by Hugo Pristauz on 2021-11-06
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLLOG_H__
#define __BLLOG_H__

#include "blrtos.h"
#include "bltype.h"

#define BL_DEBUG 1                     // debug level

//==============================================================================
// ANSI color sequences
//==============================================================================

    #define BL_R     "\x1b[31m"        // red
    #define BL_G     "\x1b[32m"        // green
    #define BL_Y     "\x1b[33m"        // yellow
    #define BL_B     "\x1b[34m"        // blue
    #define BL_M     "\x1b[35m"        // magenta
    #define BL_C     "\x1b[36m"        // cyan
    #define BL_0     "\x1b[0m"         // reset

//==============================================================================
// us/ms clock
//==============================================================================

  BL_us bl_us(void);                   // get current clock time in us
  BL_ms bl_ms(void);                   // get current clock time in ms

//==============================================================================
// formatted print - like printf("...",...)
//==============================================================================

  void bl_prt(const char * format, ...);

//==============================================================================
// debug tracing
//==============================================================================

  bool bl_dbg(int lev);                // if debug level trace timestamp
  void bl_log(int lev, BL_txt msg);
  void bl_log1(int lev, BL_txt msg, int value);
  void bl_log2(int lev, BL_txt msg, int id, int value);
  void bl_logo(int lev, BL_txt msg, BL_ob *o, int value);

  void bl_log_color(bool attention, bool provisioned);
  void bl_verbose(int verbose);        // set verbose level

//==============================================================================
// assertion
//==============================================================================

  void bl_assert(bool assertion);

#endif // __BLLOG_H__
