//==============================================================================
//  bllog.c
//  Bluccino debug logging
//
//  Created by Hugo Pristauz on 2021-11-19
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#include "blapi.h"
#include "bllog.h"

  static BL_txt color = "";            // text color for time header
  static int debug = 0;                // debug level

//==============================================================================
// set log color
//==============================================================================

  void bl_log_color(bool attention, bool provisioned)
  {
    color = attention ? BL_G : (provisioned ? BL_Y : "");
  }

  void bl_verbose(int verbose)        // set verbose level
  {
    debug = verbose;
  }

//==============================================================================
// timing
//==============================================================================

  static BL_ms m_offset_ms = 0;

  BL_ms bl_clock(void)                 // get current clock time in ms
  {
    return k_uptime_get() - m_offset_ms;
  }

//==============================================================================
// get clock time as minutes, seconds, milliseconds
//==============================================================================

  static void min_sec_ms(int *pmin, int *psec, int *pms)
  {
    static int min = 0;
    static int sec = 0;
    static BL_ms offset = 0;
    BL_ms now = bl_clock();
    *pms = (int)(now - offset);

      // calculate trace time tick

    if (offset == 0)
    {
      offset = now;  sec = min = *pms = 0;
    }

    while (*pms >= 1000)
    {
      *pms -= 1000;  offset += 1000;  sec++;
    }

    while (sec >= 60)
    {
      sec -= 60;  min++;
    }

    *pmin = min;  *psec = sec;
  }

//==============================================================================
// debug tracing
//==============================================================================

   bool bl_dbg(int lev)
   {
     if (lev > debug)
       return false;

     int min, sec, ms;
     min_sec_ms(&min,&sec,&ms);

       // print header in green if in attention mode,
       // yellow if node is provisioned, otherwise white by default

     printk("%s#%d[%03d:%02d:%03d] " BL_0, color,lev, min,sec,ms);

     for (int i=0; i < lev; i++)
       printk("  ");                   // indentation

     return true;
   }

//==============================================================================
// log messages
//==============================================================================

     // basic bl_log(lev,msg) function always automatically terminates with
     // newline, except an empty string is passed. So the possibilities arw:
     //
     //   1) bl_log(lev,"here we go");  // automatically adds "\n" at the end
     //   2) bl_log(lev,"");
     //      printk("head"); ...; printk("tail\n");

   void bl_log(int lev, BL_txt msg)
   {
     if ( !bl_dbg(lev) )
       return;

     if (msg[0] != 0)
        printk("%s\n", msg);
   }

   void bl_log1(int lev, BL_txt msg, int value)
   {
     if ( !bl_dbg(lev) )
       return;

     if (msg[0] != '@')
       printk("%s: %d\n", msg,value);
     else if (value)
       printk(BL_G "%s: %d\n" BL_0, ++msg, value);
     else
       printk(BL_M "%s: %d\n" BL_0, ++msg, value);
   }

   void bl_log2(int lev, BL_txt msg, int id, int value)
   {
     if ( !bl_dbg(lev) )
       return;

     if (msg[0] != '@')
       printk("%s: @%d,%d\n", msg,id,value);
     else if (value)
       printk(BL_G "%s: @%d,%d\n" BL_0, ++msg, id,value);
     else
       printk(BL_M "%s: @%d,%d\n" BL_0, ++msg, id,value);
   }
