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

  #define T_TICK      10               // 10 ms ticks
  #define T_ATT      750               // 750 ms attention blink period
  #define T_PRV     2000               // 2000 ms provisioned blink period
  #define T_UNP      350               // 350 ms unprovisioned blink period
  #define T_BLINK   1000               // 1000 ms RGB blink period
  #define T_STARTUP 5000               // 5000 ms startup reset interval

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
// module startup (optionally reset mesh node during startup)
// - usage: cnt = startup(o,val)
// - interfaces: []=STARTUP.SYS(INIT);[]=STARTUP.RESET(DUE);
//               []=STARTUP.GET(BUSY);[]=STARTUP.BUTTON(PRESS)
// - [SYS<INIT @id,<cb>]   // init system, provide output <cb>
// - [SYS<TICK @id,ticks]  // init system, provide output <cb>
// - [VOID<INC]            // trigger to emit a [RESET:DUE] event with 5s <due>
// - [RESET!INC due]       // inc/get reset counter, set reset <due> timer
// - [BUTTON<PRESS @id]    // receive button press event during startup
// - [RESET>DUE]           // receive [RESET:DUE] message (from core)
// - [GET>BUSY]            // get startup busy state
//==============================================================================

  static int startup(BL_ob *o, int val)     // public module interface
  {
    BL_fct output = NULL;                   // output callback
    static volatile int count = 0;          // reset counter
    static int8_t map[4] = {-1,3,4,2};      // LED @id map: [-,GREEN,BLUE,RED]

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):               // [SYS:INIT <cb>] init module
        output = o->data;                   // store output callback

          // increment reset counter, start TS = 7000 ms due timer

        bl_hdl(startup,INC_,0,0);           // handle STARTUP[HDL:INC]

        if (count <= 3)                     // <= 3 times resetted?
          led(map[count],1);                // indicate by turn on LED @count+1
        else                                // more than 3-times resetted
        {
          LOG(1,"unprovision node");        // let us know
          bl_msg(bl_down,_RESET,PRV_,0,NULL,0);   // unprovision node
        }

        return 0;                           // OK

      case BL_ID(_HDL,INC_):                // handle [HDL:INC] entry point
        count = bl_fwd(bl_down,_RESET,o,T_STARTUP); // startup reset int'val
        return 0;                           // OK

      case BL_ID(_SYS,TICK_):               // receive [RESET<DUE] event
        if (count > 0)                      // if startup in progress
        {
          static bool old;
          int rem = val % (1000/T_TICK);    // remainder modulo (ticks/period)
          bool onoff = rem < (900/T_TICK);  // 90% of time on

          if (onoff != old)                 // LED on/odd state changed?
          {
            bool save = bl_verbose(0);      // verbose off
            led(map[count],onoff);          // turn off LED @count+1
            bl_verbose(save);               // restore verbose level
          }
          old = onoff;                      // saveLED on/off state
        }
        return 0;                           // OK

      case BL_ID(_RESET,DUE_):              // receive [RESET:DUE] event
        LOG(2,BL_M"clear reset counter");   // let us know
        led(map[count],0);                  // turn off LED @count+1
        count = 0;                          // deactivate startup.busy state
        return 0;                           // OK

      case BL_ID(_GET,BUSY_):               // reset procedure busy
        return (count != 0);                // return busy state

      case BL_ID(_BUTTON,PRESS_):           // button press during startup
        if (count > 0)                      // if we are still in startup phase
          bl_hdl(startup,INC_,0,0);         // handle STARTUP[HDL:INC]
        return 0;                           // OK

      default:
        return 0;
    }
  }

//==============================================================================
// module attention (handle attention state changes and perform blinking)
// - usage: state = attention(o,val)
// - interfaces: []=SYS(TICK); []=MESH(ATT); []=GET(ATT);
// - [SYS:INIT <cb>]    // init module
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [SET:ATT state]    // update attention state which is received from mesh
// - [GET:ATT state]    // return current attention state
//==============================================================================

  static int attention(BL_ob *o, int val)   // public attention interface
  {
    static volatile bool state;             // attention state
    static BL_ms due = 0;

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):               // [SYS:INIT @id <cb>]
        return 0;                           // OK (nothing to init)

      case BL_ID(CL_SYS,OP_TICK):
        if (!state || !bl_due(&due,T_ATT))  // attention state? due?
          return 0;                         // neither attention state nor due
        return led(0,-1);                   // toggle status LED @1

      case BL_ID(CL_SET,OP_ATT):
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
// - [SYS:INIT <cb>]    // init module
// - [SYS:TICK @id,cnt] // tick input for attention blinking
// - [SET:PRV state]    // set provision state which is received from mesh
// - [GET:PRV state]    // return current provision state
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
        if (rem > 1 || bl_get(attention,OP_ATT) || bl_get(startup,OP_BUSY))
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
// helper: attention blinker (let green status LED @0 attention blinking)
// - @id=0: dark, @id=1: status, @id=2: red, @id=3: green, @id=4: blue
//==============================================================================

  static int blink(BL_ob *o, int ticks)     // attention blinker to be ticked
  {
    static BL_ms due = 0;                   // need for periodic action

    if (id <= 1 || !bl_due(&due,T_BLINK))   // no blinking if @id:off or not due
      return 0;                             // bye if LED off or not due

    if ( bl_get(attention,ATT_) ||          // no blinking in attention mode
         bl_get(startup,BUSY_))             // no blinking during startup
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
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    switch (bl_id(o))
    {
      case BL_ID(CL_SET,OP_ATT):            // [MESH:ATT state]
        return attention(o,val);            // change attention state

      case BL_ID(CL_SET,OP_PRV):            // [MESH:ATT state]
        return provision(o,val);            // change provision state

      case BL_ID(CL_BUTTON,OP_PRESS):       // button press to cause LED on off
        LOGO(1,"@",o,val);

        led(2,0); led(3,0); led(4,0);       // turn off current LED
        id = (id==0) ? 2 : (id+1)%5;        // update THE LED id (=> 0 or 2,3,4)
        return startup(o,val);              // fwd [BUTTON:PRESS] to startup

      case BL_ID(_RESET,DUE_):              // [RESET:DUE] - reset counter due
        return startup(o,val);              // forward to startup module

      default:
        return -1;                          // bad args
    }
  }

//==============================================================================
// message downward posting to lower level / driver layer (default/__weak)
//==============================================================================

  int bl_down(BL_ob *o, int val)
  {
    if ( o->cl == CL_LED && !o->id)    // special logging of status LED messages
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
    startup(o,ticks);                  // tick startup module
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
    bl_init(attention,NULL);           // init atttention module
    bl_init(provision,NULL);           // init provision module
    bl_init(startup,NULL);             // init startup module
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
