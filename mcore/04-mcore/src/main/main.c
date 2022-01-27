//==============================================================================
// main.c
// main program for testing wearable's hwcore
//
// Created by Hugo Pristauz on 2022-Jan-04
// Copyright © 2022 fuseAware. All rights reserved.
//==============================================================================

  #include "bluccino.h"

  #define VERSION  CFG_APP_VERSION
  #define VERBOSE  CFG_APP_VERBOSE     // verbose level for application

//==============================================================================
// MAIN level logging shorthands
//==============================================================================

  #define LOG                     LOG_MAIN
  #define LOGO(lvl,col,o,val)     LOGO_MAIN(lvl,col"main:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_MAIN(lvl,col,o,val)

//==============================================================================
// helper: LED control   // set or toggle on/off status of LED @id
// - usage: led(id,val)  // val=-1: toggle, val=0: off, val=1: on
//==============================================================================

  static int id = 0;                        // THE LED id

  static int led(int id, int val)           // control LED
  {
    BL_op op = val<0 ? TOGGLE_:SET_;    // SET or TOGGLE?
    BL_ob oo = {_LED,op,id,NULL};
    return bl_down(&oo,val);                // post [LED:op @id,val] to core
  }

//==============================================================================
// when callback - implement simple test flow
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    switch (BL_ID(o->cl,o->op))
    {
      case BL_ID(_BUTTON,PRESS_):     // button press to cause LED on off
        LOGO(1,"@",o,val);

        led(id,0);                          // turn off current LED

        if (o->id == id)                    // same button pressed again?
          id = 0;                           // unselect current ID
        else
          id = o->id;                       // update THE LED id

        return 0;                           // OK

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
// tick function (sends tick messages to all modules which have to be ticked)
//==============================================================================

  static int tick(BL_ob *o, int val)   // system ticker: ticks all subsystems
  {
    LOGO(5,BL_Y,o,val);                // log to see we are alife
    return 0;                          // OK
  }

//==============================================================================
// tock function (sends tock messages to all modules which have to be tocked)
//==============================================================================

  static int tock(BL_ob *o, int val)   // system tocker: tocks all subsystems
  {
    LOGO(3,"",o,val);                  // log to see we are alife
    if (id > 0)
      led(id,-1);                      // toggle LED
    return 0;                          // OK
  }

//==============================================================================
// app init
// - 1) init all modules of app (note: Bluccino init is done in main engine!)
// - 2) setup connections: all outputs of SOS module have to go to LED module
//==============================================================================

  static int init(BL_ob *o, int val)   // init all modules
  {
    return 0;                          // OK
  }

//==============================================================================
// main engine
// - calling Bluccino init and app init() function
// - periodic (10ms) tick() message emission
// - periodic (2s)   tock() message emission
//==============================================================================

  void main(void)
  {
    bl_hello(VERBOSE,VERSION);

    bl_init(bluccino,when);            // Bluccino init, subscribe with when()
    bl_init(init,NULL);                // app init

    int tocks = 0;                     // tock counter
    for(int ticks=0;;ticks++)          // loop generating (approx) 10ms ticks
    {
      bl_tick(tick,0,ticks);           // app ticking

      if (ticks % 100 == 0)            // app tock is 100 times slower
      {
        bl_tock(tock,1,tocks);         // app tocking
        tocks++;
      }

      bl_sleep(10);                    // sleep 10 ms
    }
  }
