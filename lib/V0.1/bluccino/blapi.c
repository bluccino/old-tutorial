//==============================================================================
//  blapi.c - Bluccino API layer
//  Bluccino debug logging
//
//  Created by Hugo Pristauz on 2021-11-19
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#include "bluccino.h"
#include "blweak.h"                    // weak function definitions
#include "blcore.h"
#include "blgear.h"

//==============================================================================
// provisioned & attention state
//==============================================================================

  bool bl_provisioned = 0;
  bool bl_attention = 0;

//==============================================================================
// notification and driver callbacks
//==============================================================================

  static BL_fct notify = NULL;      // notification callback
  static BL_fct driver = NULL;      // output driver callback

  void bl_driver(BL_fct cb)         // set output driver callback
  {
    driver = cb;                       // store callback
  }

//==============================================================================
// us/ms clock
//==============================================================================

  static volatile BL_us offset = 0;

  static BL_us now_us()                     // system clock in us
  {
    uint64_t cyc = k_uptime_ticks();
    uint64_t f = sys_clock_hw_cycles_per_sec();
    return (BL_us)((1000000*cyc)/f);
  }

  BL_us bl_zero(void)                       // reset clock
  {
    return offset = now_us();
  }

  BL_us bl_us(void)                         // get current clock time in us
  {
    BL_us us = now_us();

    if (offset == 0)                        // first call always returns 0
      us = bl_zero();                       // reset clock

    return us  - offset;
  }

  BL_ms bl_ms(void)                         // get current clock time in ms
  {
    BL_us us = bl_us();
    return us/1000;
  }

//==============================================================================
// sleep & wait
//==============================================================================

  void bl_sleep(BL_ms ms)              // deep sleep for given milliseconds
  {
    if (ms > 0)
      BL_SLEEP((int)ms);
  }

  void bl_wait(BL_ms tick)             // wait until time tick
  {
    BL_ms ms = tick - bl_ms();
    bl_sleep(ms);
  }

//==============================================================================
// output message (either handeled by driver or else by app subscription)
//==============================================================================

  int bl_out(BL_ob *o, int val, BL_fct cb)
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
        bl_provisioned = val;
        bl_log_color(bl_attention,bl_provisioned);
        bl_log1(2,"@mesh:provisioned",bl_provisioned);
        return bl_out(o,val,notify);

      case BL_PAIR(CL_MESH,OP_ATT):         // attention state changed
        bl_attention = val;
        bl_log_color(bl_attention,bl_provisioned);
        bl_log1(2,"@mesh:attention",bl_attention);
        return bl_out(o,val,notify);

      case BL_PAIR(CL_GOOSRV,OP_SET):       // GOOSRV set
      case BL_PAIR(CL_GOOSRV,OP_LET):       // GOOSRV let (unack'ed set)
        bl_logo(2,"@api",o,val);
        return bl_core(o,val);

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
// run system operation on a module (syntactic sugar: id = 0, val = 0)
//==============================================================================

  int bl_sys(int op, BL_fct module, BL_fct cb)
  {
    BL_ob o = {CL_SYS,op,0,cb};
    return module(&o,0);
  }

//==============================================================================
// obligatory init and loop functions
//==============================================================================

    // bl_init(NULL,when,verbose)           // Bluccino API level loop
    // bl_init(bl_gear,when_core,0)         // initialize gear

  void bl_init(BL_fct module,BL_fct cb, int val)
  {
    if (module)
      bl_sys(OP_INIT,module,cb);            // init module
    else
    {
      notify = cb;
      bl_verbose(val);

      //bl_sys(OP_INIT,bl_gear,bl_down);      // init gear
bl_log(2,BL_B"api:init"BL_0);
      bl_sys(OP_INIT,bl_core,when_core);    // init core
bl_log(2,BL_B"api:done"BL_0);
    }
  }

    // bl_loop(NULL)                        // Blucino API level loop
    // bl_loop(bl_core)                     // core level loop

  void bl_loop(BL_fct module)
  {
    if (module)
      bl_sys(OP_LOOP,module,NULL);          // run specific module loop
    else
    {
      bl_sys(OP_LOOP,bl_gear,NULL);         // run gear loop
      bl_sys(OP_LOOP,bl_core,NULL);         // run core loop
    }
  }
