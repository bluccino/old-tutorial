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
    BL_op op = val<0 ? OP_TOGGLE:OP_SET;    // SET or TOGGLE?
    BL_ob oo = {CL_LED,op,id,NULL};
    if (id)                                 // don't log status LED @0 logs
      LOGO(1,"@",&oo,val);                  // see what LEDs will do
    return bl_down(&oo,val);                // post [LED:op @id,val] to core
  }

//==============================================================================
// module attention (handle attention state changes and perform blinking)
// - usage: state = attention(o,val)
// - interfaces: []=SYS(TICK); []=MESH(ATT); []=GET(ATT);
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [MESH:ATT state]   // update attention state which is received from mesh
// - [GET:ATT state]    // return current attention state
//==============================================================================

  static int attention(BL_ob *o, int val)   // public attention interface
  {
    static volatile bool state;             // attention state
    static BL_ms due = 0;

    switch (bl_id(o))
    {
      case BL_ID(CL_SYS,OP_TICK):
        if (!state || !bl_due(&due,750))    // attention state? due?
          return 0;                         // neither attention state nor due
        return led(0,-1);                   // toggle status LED @1

      case BL_ID(CL_MESH,OP_ATT):
        state = val;                        // store attention state
        led(0,0);                           // turn status LED off
        led(id,0);                          // turn current LED off
        return 0;

      case BL_ID(CL_GET,OP_ATT):            // state = ATTENTION[GET:ATT]
        return state;

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
// module provision (handle provision state changes and perform blinking)
// - usage: state = provision(o,val)
// - interfaces: []=SYS(TICK); []=MESH(PRV); []=GET(PRV);
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [MESH:PRV state]   // update provision state which is received from mesh
// - [GET:PRV state]    // return current provision state
//==============================================================================

  static int provision(BL_ob *o, int val)   // public provision interface
  {
    static volatile bool state = 0;         // provision state

    switch (bl_id(o))
    {
      case BL_ID(CL_SYS,OP_TICK):
      {
        int rem = val % (state?200:35);     // 2000 ms versus 350 ms period
        if (rem > 1 || bl_get(attention,OP_ATT))
          return 0;                         // no provision blinking during att!

        return led(0,rem == 0);             // update status LED @1
      }
      case BL_ID(CL_MESH,OP_PRV):
        state = val;                        // store attention state
        led(0,0);                           // turn status LED @1 off
        led(id,0);                          // turn current LED @id off
        return 0;

      case BL_ID(CL_GET,OP_PRV):            // state = ATTENTION[GET:ATT]
        return state;

      default:
        return -1;                          // bad args
    }
  }
//==============================================================================
// helper: attention blinker (let green status LED @0 attention blinking)
// - @id=0: dark, @id=1: status, @id=2: red, @id=3: green, @id=4: blue
//==============================================================================

  static int blink(BL_ob *o, int ticks)     // attention blinker to be ticked
  {
    static BL_ms due = 0;

    if (!bl_due(&due,500) || !bl_get(attention,OP_ATT))
      return 0;                             // bye if neither due nor att state


    return led(1,-1);                       // toogle red LED (@id=1)
  }

//==============================================================================
// when callback - implement simple test flow
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    switch (bl_id(o))
    {
      case BL_ID(CL_MESH,OP_ATT):           // [MESH:ATT state]
        return attention(o,val);            // change attention state

      case BL_ID(CL_MESH,OP_PRV):           // [MESH:ATT state]
        return provision(o,val);            // change provision state

      case BL_ID(CL_BUTTON,OP_PRESS):       // button press to cause LED on off
        LOGO(1,"@",o,val);

        led(id,0);                          // turn off current LED
        id = (id==0) ? 2 : (id+1)%5;        // update THE LED id (=> 0 or 2,3,4)
        led(id,1);                          // turn on new selected LED
        return 0;                           // OK

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
// message downward posting to lower level / driver layer (default/__weak)
//==============================================================================

  int bl_down(BL_ob *o, int val)
  {
    #define LED_SET BL_ID(CL_LED,OP_SET)

    if ( bl_id(o)==LED_SET && !o->id)  // special logging of status LED messages
      LOGO(5,"status:down:",o,val);
    else
      LOGO(3,BL_Y "down:",o,val);

    return bl_core(o,val);
  }

//==============================================================================
// tick function (sends tick messages to all modules which have to be ticked)
//==============================================================================

  static int tick(BL_ob *o, int ticks) // system ticker: ticks all subsystems
  {
    LOGO(5,BL_Y,o,ticks);              // log to see we are alife
    blink(o,ticks);                    // tick blinker
    attention(o,ticks);                // tick attention module
    provision(o,ticks);                // tick provision module
    return 0;                          // OK
  }

//==============================================================================
// tock function (sends tock messages to all modules which have to be tocked)
//==============================================================================

  static int tock(BL_ob *o, int tocks) // system tocker: tocks all subsystems
  {
    if (tocks % 5 == 0)                // log every 5th tock
      LOGO(3,"",o,tocks);              // log to see we are alife

    if (id > 0 && !bl_get(attention,OP_ATT))
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
