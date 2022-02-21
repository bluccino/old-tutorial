//==============================================================================
// main.c
// main program for testing wearable's hwcore
//
// Created by Hugo Pristauz on 2022-Jan-04
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================

  #include "bluccino.h"
  #include "bl_basis.h"

  #define VERSION  CFG_APP_VERSION
  #define VERBOSE  CFG_APP_VERBOSE     // verbose level for application

  #define T_BLINK   1000               // 1000 ms RGB blink period

//==============================================================================
// MAIN level logging shorthands
//==============================================================================

  #define LOG                     LOG_MAIN
  #define LOGO(lvl,col,o,val)     LOGO_MAIN(lvl,col"main:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_MAIN(lvl,col,o,val)

//==============================================================================
// locals
//==============================================================================

  static volatile int id = 0;               // THE LED id

//==============================================================================
// helper: LED control   // set or toggle on/off status of LED @id
// - usage: led(id,val)  // val=-1: toggle, val=0: off, val=1: on
// -        @id=0: dark, @id=1: status, @id=2: red, @id=3: green, @id=4: blue
//==============================================================================

  static int led(int id, int val)           // control LED
  {
    BL_op op = val<0 ? TOGGLE_:SET_;    // SET or TOGGLE?
    BL_ob oo = {_LED,op,id,NULL};
    if (id)                                 // don't log status LED @0 logs
      LOGO(1,"@",&oo,val);                  // see what LEDs will do
    return bl_down(&oo,val);                // post [LED:op @id,val] to core
  }

//==============================================================================
// helper: attention blinker (let green status LED @0 attention blinking)
// - @id=0: dark, @id=1: status, @id=2: red, @id=3: green, @id=4: blue
//==============================================================================

  static int blink(BL_ob *o, int ticks)     // attention blinker to be ticked
  {
    static BL_ms due = 0;                   // need for periodic action

    if (id <= 1 || !bl_due(&due,T_BLINK))   // no blinking if @id:off or not due
      return 0;                             // bye if LED off or not due

    if ( bl_get(bl_basis,ATT_) ||           // no blinking in attention mode
         bl_get(bl_basis,BUSY_))            // no blinking during startup
      return 0;                             // bye if attention state

    static bool toggle;
    toggle = !toggle;

    if (toggle)
      return (led(id,1), led(2+id%3,0));    // flip LED pair
    else
      return (led(id,0), led(2+id%3,1));    // flip back LED pair
  }

//==============================================================================
// when callback - implement simple test flow
// - [SWITCH:STS @id,val] events are forwarded to [GOOCLI:SET @1,val] calls
// - [GOOSRV:STS @id,val] events are forwarded to [LED:SET @2,val] calls
// - led() is a helper routine 
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    switch (bl_id(o))
    {
      case BL_ID(_SWITCH,STS_):        // button press to cause LED on off
        LOGO(1,"@",o,val);
        if ( bl_get(bl_basis,PRV_))   // only if provisioned
        {
          BL_ob oo = {_GOOCLI,SET_,1,NULL};
          bl_down(&oo,val);            // post via generic on/off client
        }
        return 0;                      // OK

      case BL_ID(_GOOSRV,STS_):        // [GOOSRV:STS] status update
        LOGO(1,BL_R,o,val);
        led(2,val);                    // switch LED @2
        return 0;                      // OK

      default:
        return bl_basis(o,val);         // else forward event to BL_BASE module
    }
  }

//==============================================================================
// tick function (sends tick messages to all modules which have to be ticked)
//==============================================================================

  static int tick(BL_ob *o, int ticks) // system ticker: ticks all subsystems
  {
    blink(o,ticks);                    // tick blinker
    bl_basis(o,ticks);
    return 0;                          // OK
  }

//==============================================================================
// tock function (sends tock messages to all modules which have to be tocked)
//==============================================================================

  static int tock(BL_ob *o, int tocks) // system tocker: tocks all subsystems
  {
    if (tocks % 5 == 0)                // log every 5th tock
      LOGO(3,"",o,tocks);              // log to see we are alife

    return 0;                          // OK
  }

//==============================================================================
// app init
// - 1) init all modules of app (note: Bluccino init is done in main engine!)
// - 2) setup connections: all outputs of SOS module have to go to LED module
//==============================================================================

  static int init(BL_ob *o, int val)   // init all modules
  {
    bl_init(bl_basis,bl_down);         // init BL_BASIS module, output goes down
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

      // 10 ms tick loop

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
