//==============================================================================
// bl_basis.c
// basis functions of a mesh node (startup, provision, attention)
//
// Created by Hugo Pristauz on 2022-Feb-21
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================

  #include "bluccino.h"
  #include "bl_basis.h"

  #define T_TICK      10               // 10 ms ticks
  #define T_ATT      750               // 750 ms attention blink period
  #define T_PRV     2000               // 2000 ms provisioned blink period
  #define T_UNP      350               // 350 ms unprovisioned blink period
  #define T_STARTUP 5000               // 5000 ms startup reset interval

  #define _INC_     BL_HASH(INC_)      // hashed #INC opcode
  #define _SET_     BL_HASH(SET_)      // hashed #SET opcode
  #define _TOGGLE_  BL_HASH(TOGGLE_)   // hashed #TOGGLE opcode

//==============================================================================
// MAIN level logging shorthands
//==============================================================================

  #define LOG                     LOG_BASIS
  #define LOGO(lvl,col,o,val)     LOGO_MAIN(lvl,col"bl_basis:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_MAIN(lvl,col,o,val)

//==============================================================================
// locals
//==============================================================================

  static volatile int id = 0;               // THE LED id

//==============================================================================
// helper: set LED @id on/off or togggle LED @id (@id: 0..4)
// - usage: led(id,val)   // val 0:off, 1:on, -1:toggle
//==============================================================================

  static int led(int id, int val)
  {
    BL_ob oo = {_LED,val<0?_TOGGLE_:_SET_,id,NULL};
    return bl_basis(&oo,val<0?0:val);
  }

//==============================================================================
//
//                          +-----------------+
//                          |     STARTUP     |
//                          +-----------------+
//                   INIT ->|      SYS:       |
//                   TICK ->|                 |
//                          +-----------------+
//                          |     RESET:      |
//                   #INC ->|                 |-> INC ->(v)
//                    DUE ->|                 |-> PRV ->(v)
//                          +-----------------+
//                   BUSY ->|      GET:       |
//                          +-----------------+
//                  PRESS ->|     BUTTON:     |
//                          +-----------------+
//                          |       LED:      |->(v) LED ->(v)
//                          +-----------------+
//
//==============================================================================
// module startup (optionally reset mesh node during startup)
// - usage: cnt = startup(o,val)
// - interfaces: []=STARTUP.SYS(INIT);[]=STARTUP.RESET(DUE);
//               []=STARTUP.GET(BUSY);[]=STARTUP.BUTTON(PRESS)
// - [SYS:INIT <out>]      // init system, store <out> callback
// - [SYS:TICK @id,cnt]    // tick system
// - [RESET:#INC due]      // provide [RESET:#INC] event with 5s <due>
// - [RESET:INC due]       // inc/get reset counter, set reset <due> timer
// - [RESET:DUE]           // receive [RESET:DUE] message (from core)
// - [GET:BUSY]            // get startup busy state
// - [BUTTON:PRESS @id]    // receive button press event during startup
//==============================================================================

  static int startup(BL_ob *o, int val)     // public module interface
  {
    static BL_fct out = NULL;               // <out> callback
    static volatile int count = 0;          // reset counter
    static int8_t map[4] = {-1,3,4,2};      // LED @id map: [-,GREEN,BLUE,RED]

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):               // [SYS:INIT <out>] init module
      {
        out = o->data;                      // store <out> callback

          // increment reset counter, start TS = 7000 ms due timer

        BL_ob oo = {_RESET,_INC_,0,NULL};
        startup(&oo,0);                     // post [HDL:#INC] to startup ifc

        if (count <= 3)                     // <= 3 times resetted?
          return led(map[count],1);      // indicate by turn on LED @count+1

        LOG(1,BL_R"unprovision node");      // let us know
        return bl_msg(bl_down,_RESET,PRV_,0,NULL,0); // unprovision node
      }

      case BL_ID(_SYS,TICK_):               // receive [RESET<DUE] event
        if (count > 0)                      // if startup in progress
        {
          static bool old;
          int rem = val % (1000/T_TICK);    // remainder modulo (ticks/period)
          bool onoff = rem < (900/T_TICK);  // 90% of time on

          if (onoff != old)                 // LED on/odd state changed?
            led(map[count],onoff);       // turn off LED @count+1

          old = onoff;                      // saveLED on/off state
        }
        return 0;                           // OK

      case BL_ID(_RESET,_INC_):             // [RESET:#INC] entry point
        count = bl_fwd(bl_down,_RESET,o,T_STARTUP); // startup reset int'val
        return 0;                           // OK

      case BL_ID(_RESET,DUE_):              // receive [RESET:DUE] event
        LOG(2,BL_B"clear reset counter");   // let us know
        led(map[count],0);               // turn off LED @count+1
        count = 0;                          // deactivate startup.busy state
        return 0;                           // OK

      case BL_ID(_GET,BUSY_):               // reset procedure busy
        return (count != 0);                // return busy state

      case BL_ID(_BUTTON,PRESS_):           // button press during startup
        if (count > 3)
        {
          LOG(1,BL_R"unprovision node");    // let us know
          return bl_msg(bl_down,_RESET,PRV_,0,NULL,0);   // unprovision node
        }

        if (count > 0)                      // if we are still in startup phase
        {
          BL_ob oo = {_RESET,_INC_,0,NULL}; // [RESET:#INC]
          startup(&oo,0);                   // post [RESET:#INC] to STARTUP mod.
        }
        return 0;                           // OK

      default:
        return 0;
    }
  }

//==============================================================================
//
//                          +-----------------+
//                          |    ATTENTION    |
//                          +-----------------+
//                   INIT ->|      SYS:       |
//                   TICK ->|                 |
//                          +-----------------+
//                    ATT ->|      SET:       |
//                          +-----------------+
//                    ATT ->|      GET:       |
//                          +-----------------+
//
//==============================================================================
// module attention (handle attention state changes and perform blinking)
// - usage: state = attention(o,val)
// - interfaces: []=SYS(INIT,TICK); []=SET(ATT); []=GET(ATT);
// - [SYS:INIT <cb>]    // init module
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [SET:ATT state]    // update attention state which is received from mesh
// - state = [GET:ATT]  // return current attention state
//==============================================================================

  static int attention(BL_ob *o, int val)   // public attention interface
  {
    static volatile bool state;             // attention state
    static BL_ms due = 0;

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):               // [SYS:INIT @id <cb>]
        return 0;                           // OK (nothing to init)

      case BL_ID(_SYS,TICK_):
        if (!state || !bl_due(&due,T_ATT))  // attention state? due?
          return 0;                         // neither attention state nor due
        return led(0,-1);                // toggle status LED @0

      case BL_ID(_SET,ATT_):
        state = val;                        // store attention state
        led(0,0);                        // turn status LED off
        led(id,0);                       // turn current LED off
        return 0;

      case BL_ID(_GET,ATT_):                // state = ATTENTION[GET:ATT]
        return state;

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
//
//                      +-----------------+
//                      |    PROVISION    |
//                      +-----------------+
//               INIT ->|      SYS:       |
//               TICK ->|                 |
//                      +-----------------+
//                PRV ->|      SET:       |
//                      +-----------------+
//                PRV ->|      GET:       |
//                      +-----------------+
//
//==============================================================================
// module provision (handle provision state changes and perform blinking)
// - usage: state = provision(o,val)
// - interfaces: []=SYS(INIT,TICK); []=SET(PRV); []=GET(PRV);
// - [SYS:INIT <cb>]    // init module
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [SET:PRV state]    // set provision state which is received from mesh
// - state = [GET:PRV]  // return current provision state
//==============================================================================

  static int provision(BL_ob *o, int val)   // public provision interface
  {
    static volatile bool state = 0;         // provision state

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):               // [SYS:INIT @id <cb>]
        return 0;                           // OK (nothing to init)

      case BL_ID(_SYS,TICK_):               // [SYS:TICK @id <val>]
      {
        int ms = (state ? T_PRV:T_UNP);     // 2000 ms versus 350 ms period
        int rem = val % (ms/T_TICK);        // remainder modulo (ticks/period)

        if (rem > 1 || bl_get(attention,ATT_) || bl_get(startup,BUSY_))
          return 0;                         // no provision blinking during att!
        else
          return led(0,rem == 0);           // update status LED @1
      }

      case BL_ID(_SET,PRV_):                // [SET:PRV val] change prov state
        state = val;                        // store attention state
        led(0,0);                           // turn status LED @1 off
        led(id,0);                          // turn current LED @id off
        return 0;

      case BL_ID(_GET,PRV_):                // state = ATTENTION[GET:ATT]
        return state;

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
// public module interface
//==============================================================================
//
// BL_BASE Interfaces:
//   SYS Interface:      [] = SYS(INIT,TICK)
//   GET Interface:      [] = GET(PRV,ATT,BUSY)
//   SET Interface:      [] = SET(PRV,ATT)
//   HDL Interface:      [] = HDL(INC)
//   BUTTON Interface:   [] = BUTTON(PRESS)
//   LED Interface:      [LED] = HDL()
//   RESET Interface:    [INC,PRV] = RESET(INC,DUE)
//
//                          +-----------------+
//                          |     BL_BASE     |
//                          +-----------------+
//                   INIT ->|      SYS:       |
//                   TICK ->|                 |
//                          +-----------------+
//                    PRV ->|      GET:       |
//                    ATT ->|                 |
//                   BUSY ->|                 |
//                          +-----------------+
//                    PRV ->|      SET:       |
//                    ATT ->|                 |
//                          +-----------------+
//                  PRESS ->|     BUTTON:     |
//                          +-----------------+
//                    #SET->|       LED:      |-> SET     ->(v)
//                 #TOGGLE->|                 |-> TOGGLE  ->(v)
//                          +-----------------+
//                    INC ->|     RESET:      |-> INC
//                    DUE ->|                 |-> PRV
//                          +-----------------+
//
//==============================================================================

  int bl_basis(BL_ob *o, int val)
  {
    static BL_fct out = NULL;          // to store <out> callback
    static BL_fct down = bl_down;      // to send messages down to driver level

    switch (bl_id(o))                  // dispatch message ID
    {
      case BL_ID(_SYS,INIT_):          // [SYS:INIT <out>] - init module
        out = o->data;                 // store <out> callback
        startup(o,val);                // forward to startup() worker
        provision(o,val);              // forward to provision() worker
        attention(o,val);              // forward to attention() worker
        return 0;                      // OK

      case BL_ID(_SYS,TICK_):          // [SYS:TICK @id,cnt] - tick module
        startup(o,val);                // forward to startup() worker
        attention(o,val);              // forward to startup() worker
        provision(o,val);              // forward to provision() worker
        return 0;                      // OK

      case BL_ID(_SET,ATT_):           // set attention blinking on/off
        return attention(o,val);       // change attention state

      case BL_ID(_SET,PRV_):           // [MESH:ATT state]
        return provision(o,val);       // change provision state

      case BL_ID(_GET,BUSY_):          // [MESH:ATT state]
        return startup(o,val);         // change provision state

      case BL_ID(_BUTTON,PRESS_):      // button press to cause LED on off
        LOGO(1,"@",o,val);
        if ( !bl_get(provision,PRV_))  // if not provisioned
        {
          led(2,0); led(3,0); led(4,0);// turn off current LED
          id = (id==0) ? 2 : (id+1)%5; // update THE LED id (=> 0 or 2,3,4)
        }
        return startup(o,val);         // fwd [BUTTON:PRESS] to startup

      case BL_ID(_RESET,DUE_):         // [RESET:DUE] - reset counter due
        return startup(o,val);         // forward to startup module

      case BL_ID(_LED,_SET_):          // [LED:#SET] - set LED @id
      case BL_ID(_LED,_TOGGLE_):       // [LED:#TOGGLE] - toggle LED @id
        return bl_out(o,val,down);     // down to driver level

      default:
        return -1;                     // bad args
    }
  }
