//==============================================================================
//  bllog.h
//  bluccino logging and us/ms clock support
//
//  Created by Hugo Pristauz on 2021-11-06
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLLOG_H__
#define __BLLOG_H__

#include "blrtos.h"
#include "bltype.h"
#include "blapi.h"

#define BL_DEBUG 1                     // debug level

//==============================================================================
// XYZ level generic logging shorthands
//==============================================================================

// #define LOG                     LOG_XYZ
// #define LOGO(lvl,col,o,val)     LOGO_XYZ(lvl,col"xyz:",o,val)
// #define LOG0(lvl,col,o,val)     LOGO_XYZ(lvl,col,o,val)

//==============================================================================
// generic log function
//==============================================================================

    #define BL_LOG(lvl,fmt,...)                  \
            if (bl_dbg(lvl))                     \
            {                                    \
                bl_prt(fmt BL_0, ##__VA_ARGS__); \
                if (*fmt) bl_prt("\n");          \
            }

//==============================================================================
// API Logging
//==============================================================================

#ifndef CFG_LOG_API
    #define CFG_LOG_API    1           // no API logging by default
#endif

#if (CFG_LOG_API)
    #define LOG_API(l,f,...)    BL_LOG(CFG_LOG_API-1+l,f,##__VA_ARGS__)
    #define LOGO_API(l,f,o,v)   bl_logo(CFG_LOG_API-1+l,f,o,v)
#else
    #define LOG_API(l,f,...)    {}     // empty
    #define LOGO_API(l,f,o,v)   {}     // empty
#endif

//==============================================================================
// CORE Logging
//==============================================================================

#ifndef CFG_LOG_CORE
    #define CFG_LOG_CORE    1           // no CORE logging by default
#endif

#if (CFG_LOG_CORE)
    #define LOG_CORE(l,f,...)    BL_LOG(CFG_LOG_CORE-1+l,f,##__VA_ARGS__)
    #define LOGO_CORE(l,f,o,v)   bl_logo(CFG_LOG_CORE-1+l,f,o,v)
#else
    #define LOG_CORE(l,f,...)    {}     // empty
    #define LOGO_CORE(l,f,o,v)   {}     // empty
#endif

//==============================================================================
// APP Logging
//==============================================================================

#ifndef CFG_LOG_APP
    #define CFG_LOG_APP    1           // no APP logging by default
#endif

#if (CFG_LOG_APP)
    #define LOG_APP(l,f,...)    BL_LOG(CFG_LOG_APP-1+l,f,##__VA_ARGS__)
    #define LOGO_APP(l,f,o,v)   bl_logo(CFG_LOG_APP-1+l,f,o,v)
#else
    #define LOG_APP(l,f,...)    {}     // empty
    #define LOGO_APP(l,f,o,v)   {}     // empty
#endif

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
// shut off after n calls (for n > 0)
// - note: function is deactivated for n=0
//==============================================================================

  static inline void bl_shut(int n)    // set verbose level 0 after n calls
  {
    static int count = 0;
    if (n == 0 || count > n)
       return;                         // function is deactivated for n=0
    if (bl_dbg(0))
      bl_prt(BL_Y"verbose countdown: %d",n-count);
    if (++count > n)
      bl_verbose(0);
  }

//==============================================================================
// assertion
//==============================================================================

  void bl_assert(bool assertion);

#endif // __BLLOG_H__
