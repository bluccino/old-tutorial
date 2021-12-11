//==============================================================================
//  blapi.c - Bluccino API layer
//  Bluccino debug logging
//
//  Created by Hugo Pristauz on 2021-11-19
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#include "bluccino.h"
#include "blweak.h"                    // weak function definitions

//==============================================================================
// provisioned & attention state
//==============================================================================

  static bool provisioned = 0;
  static bool attention = 0;

//==============================================================================
// notification and driver callbacks
//==============================================================================

  static BL_notify notify = NULL;      // notification callback
  static BL_notify driver = NULL;      // output driver callback

  void bl_driver(BL_notify cb)         // set output driver callback
  {
    driver = cb;                       // store callback
  }

//==============================================================================
// us/ms clock
//==============================================================================

  static BL_us now_us()                     // system clock in us
  {
    uint64_t cyc = k_uptime_ticks();
    uint64_t f = sys_clock_hw_cycles_per_sec();
    return (BL_us)((1000000*cyc)/f);
  }

  BL_us bl_us(void)                         // get current clock time in us
  {
    static BL_us offset = 0;
    BL_us us = now_us();

    if (offset == 0)                        // first call always returns 0
      offset = us;

    return us  - offset;
  }

  BL_ms bl_ms(void)                         // get current clock time in ms
  {
    BL_us us = bl_us();
    return us/1000;
  }

//==============================================================================
// timing & sleep
//==============================================================================

  void bl_sleep(int ms)                // deep sleep for given milliseconds
  {
    BL_SLEEP(ms);
  }

//==============================================================================
// output message (either handeled by driver or else by app subscription)
//==============================================================================

  int bl_out(BL_ob *o, int val, BL_notify cb)
  {
    if (driver)                        // if a driver callback has been provided
    {                                  // => call driver for message processing
      if ( driver(o,val) )             // could driver process the message?
        return 1;                      // yes? => then we are already done!
    }
        // otherwise notify application if callback has been provided

    if (cb)                            // is an app callback provided?
      return cb(o,val);                // forward event message to app

    return 0;
  }

//==============================================================================
// message input in Bluccino API layer
//==============================================================================

  int bl_in(BL_ob *o, int val)
  {
    int level = 2;                          // default verbose level
    int pair = BL_PAIR(o->cl,o->op);

    switch (pair)                           // dispatch event
    {
      case BL_PAIR(CL_MESH,OP_PRV):         // provisioned state changed
        provisioned = val;
        bl_log_color(attention,provisioned);
        bl_log1(2,"@mesh:provisioned",provisioned);
        return bl_out(o,val,notify);

      case BL_PAIR(CL_MESH,OP_ATT):         // attention state changed
        attention = val;
        bl_log_color(attention,provisioned);
        bl_log1(2,"@mesh:attention",attention);
        return bl_out(o,val,notify);

      case BL_PAIR(CL_TIMER,OP_TICK):
      case BL_PAIR(CL_SCAN,OP_ADV):
        level = 5;
        break;

      default:
        break;
    }

    bl_logo(level,"@api",o,val);
    return bl_out(o,val,notify);       // output message to out-gear or app
  }

//==============================================================================
// core message callback
//==============================================================================

  static int when_core(BL_ob *o, int val)
  {
    bl_logo(2,"@api",o,val);
    return val;
  }

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_notify cb, int verbose)
  {
    notify = cb;
    bl_verbose(verbose);
    bl_gear_init(bl_output);
    bl_core_init(when_core);
  }

  void bl_loop(void)
  {
    bl_core_loop();
  }
