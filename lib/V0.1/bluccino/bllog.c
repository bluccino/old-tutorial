//==============================================================================
//  bllog.c
//  Bluccino debug logging
//
//  Created by Hugo Pristauz on 2021-11-19
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#include "bluccino.h"

#ifndef CFG_PRETTY_LOGGING
  #define CFG_PRETTY_LOGGING    0      // pretty text for class tag & opcode
#endif

  static BL_txt color = "";            // text color for time header
  static int debug = 0;                // debug level

//==============================================================================
// RTT log driver
//==============================================================================

  void bl_vprintf(const char * format, va_list arguments)
  {
    BL_VPRINTF(0, format, &arguments);
  }

  void bl_printf(const char * format, ...)
  {
    va_list arguments;  // lint -save -esym(530,args) sym args not initialized
    va_start(arguments, format);
    bl_vprintf(format, arguments);
    va_end(arguments);  // lint -restore
  }

//==============================================================================
// set log color
//==============================================================================

  void bl_log_color(bool attention, bool provisioned)
  {
    color = attention ? BL_G : (provisioned ? BL_Y : "");
  }

  void bl_verbose(int verbose)              // set verbose level
  {
    debug = verbose;
  }

//==============================================================================
// assertion
//==============================================================================

  void bl_assert(bool assertion)
  {
    if (!assertion)
    {
      bl_log(0,BL_R"assertion violated");
      for(;;);
    }
  }

//==============================================================================
// get clock time as minutes, seconds, milliseconds
//==============================================================================

  static void now(int *pmin, int *psec, int *pms, int *pus)  // split us time
  {
    static int min = 0;
    static int sec = 0;
    static BL_ms offset = 0;
    BL_us us = bl_us();                    // clock time now in us

    *pus = us % 1000;                      // map us to range 0 .. 999
    *pms = us/1000 - offset;

      // calculate trace time tick

    for (; *pms >= 1000; offset += 1000, sec++)
      *pms -= 1000;

    for (; sec >= 60; min++)
      sec -= 60;

    *pmin = min;  *psec = sec;
  }

//==============================================================================
// debug tracing
//==============================================================================

  bool bl_dbg(int lev)
  {
    if (lev > debug)
      return false;

    int min, sec, ms, us;
    now(&min,&sec,&ms,&us);

      // print header in green if in attention mode,
      // yellow if node is provisioned, otherwise white by default

    printk("%s#%d[%03d:%02d:%03d.%03d] " BL_0, color,lev, min,sec,ms,us);

    for (int i=0; i < lev; i++)
    {
      printk("  ");                   // indentation
    }

    return true;
  }

//==============================================================================
// log helper
//==============================================================================
#if CFG_PRETTY_LOGGING

  static BL_txt cltext(BL_cl cl)
  {
    static BL_txt text[] = BL_CL_TEXT;
    return (cl < BL_LENGTH(text)) ? text[cl] : "???";
  }

  static BL_txt optext(int op)
  {
    static BL_txt text[] = BL_OP_TEXT;
    op = (op < 0) ? -op : op;
    return (op < BL_LENGTH(text)) ? text[op] : "???";
  }

#endif
//==============================================================================
// log messages
//==============================================================================

     // basic bl_log(lev,msg) function always automatically terminates with
     // newline, except an empty string is passed. So the possibilities arw:
     //
     //   1) bl_log(lev,"here we go");  // automatically adds "\n" at the end
     //   2) bl_log(lev,"");
     //      printk("head"); ...; printk("tail\n");

  void bl_log(int lev, BL_txt msg)              // log text msg without args
  {
    if ( !bl_dbg(lev) )
      return;

    if (msg[0] != 0)
      printk("%s\n", msg);
  }

  void bl_log1(int lev, BL_txt msg, int value)  // log text msg with 1 arg
  {
    if ( !bl_dbg(lev) )
     return;

    BL_txt col = (msg[0] != '@') ? "" : (value ? BL_G : BL_M);
    msg = (msg[0] == '@') ? msg+1 : msg;

    printk("%s%s: %d\n" BL_0, col, msg, value);
  }

  void bl_log2(int lev, BL_txt msg, int id, int value) // log text with 2 args
  {
    if ( !bl_dbg(lev) )
     return;

    BL_txt col = (msg[0] != '@') ? "" : (value ? BL_G : BL_M);
    msg = (msg[0] == '@') ? msg+1 : msg;
    printk("%s%s: @%d,%d\n"BL_0, col, msg, id,value);
  }

  void bl_logo(int lev, BL_txt msg, BL_ob *o, int value) // log event message
  {
    if ( !bl_dbg(lev) )
     return;

    BL_txt col = (msg[0] != '@') ? "" : (value ? BL_G : BL_M);
    msg = (msg[0] == '@') ? msg+1 : msg;

    #if CFG_PRETTY_LOGGING             // pretty text for class tag & opcode
      printk("%s%s [%s:%s @%d (%d)]\n"BL_0, col,msg,
             cltext(o->cl),optext(o->op),o->id,value);
    #else
      printk("%s%s [%d:%d @%d (%d)]\n"BL_0,col,msg,
             o->cl,o->op, o->id,value);
    #endif
  }
