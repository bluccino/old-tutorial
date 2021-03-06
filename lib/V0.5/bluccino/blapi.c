//==============================================================================
// blapi.c - bluccino API layer
//==============================================================================
//
// The whole concept is based on having bluccino objects which control
// access to network interfaces and resources. Resource a grouped into the
// following categories:
//
//   1) Hardware
//        - general hardware control (e.g. low power control, watchdogs)
//        - LEDs
//        - buttons and/or switches
//        - General purpose I/O (GPIO)
//        - Timers
//        - Non-Volatile memory (NVM)
//   2) BLE System
//        - general BLE control (e.g. low power control of BLE sub units)
//        - advertiser
//        - scanner
//        - characteristics
//   3) MESH System
//        - general Mesh control (e.g. provisioning, attentioning)
//        - Mesh models (a kind of smart interfaces for mesh communication)
//          like client models (which) have no state and server models which
//          have state.
//
//   Each resource can have a state and for each resource there are certain
//   operations defined on it. Consider the following simple example.
//
//    +-----------+    +-----------+          +-----------+    +-----------+
//    |           |    |           |          |           |    |           |
//    +  SWITCH   +===>+  SENDER   +--------->+ RECEIVER  +===>+    LED    +
//    |           |    |           |          |           |    |           |
//    +-----------+    +-----------+          +-----------+    +-----------+
//
//   Resources can be treated as class instance objects of the different re-
//   source classes
//      :     :
//
//   Operations
//   ==========
//
//    Operation    SWITCH          CLIENT           SERVER         LED
//   -----------------------------------------------------------------------
//                  event        request to          event        change
//     SET        on state       change ser-       on state        LED
//                 change        ver's state        change        state
//
//==============================================================================

  #include "bluccino.h"

//==============================================================================
// API level logging shorthands
//==============================================================================

  #define LOG                     LOG_API
  #define LOGO(lvl,col,o,val)     LOGO_API(lvl,col"api:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_API(lvl,col,o,val)

//==============================================================================
// provisioned & attention state
//==============================================================================

  static bool provisioned = 0;
  static bool attention = 0;

//==============================================================================
// notification and driver callbacks
//==============================================================================

  static BL_fct output = NULL;      // notification callback

//==============================================================================
// us/ms clock
//==============================================================================

  static BL_us offset = 0;                  // offset for us clock

  static BL_us now_us()                     // system clock in us
  {
    #if __ZEPHYR__
      uint64_t cyc = k_uptime_ticks();
      uint64_t f = sys_clock_hw_cycles_per_sec();
      return (BL_us)((1000000*cyc)/f);
    #else
      return (BL_us)timer_now();
    #endif
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
// timing & sleep
//==============================================================================

  bool bl_due(BL_ms *pdue, BL_ms ms)   // check if time if due & update
  {
    BL_ms now = bl_ms();

    if (now < *pdue)                   // time for tick action is due?
      return false;                    // no, not yet due!

    *pdue = now + ms;                  // catch-up due time
    return true;                       // yes, tick time due!
  }

//void bl_sleep(int ms)                // deep sleep for given milliseconds
//{
//  nrf_delay_ms(ms);
//}

  void bl_sleep(BL_ms ms)              // deep sleep for given milliseconds
  {
    if (ms > 0)
      BL_SLEEP((int)ms);
  }

  void bl_halt(BL_txt msg, BL_ms ms)   // halt system
  {
    LOG(0,BL_R"%s: system halted", msg);
    for (;;)
      bl_sleep(ms);
  }

//==============================================================================
// output a message from Bluccino API or in general
//==============================================================================

  __weak int bl_out(BL_ob *o, int val, BL_fct call)
  {
    if (call)                          // is an app callback provided?
      return call(o,val);              // forward event message to app

    return 0;
  }

//==============================================================================
// input a message to Bluccino API
//==============================================================================

  __weak int bl_in(BL_ob *o, int val)
  {
    int level = 2;                          // default verbose level
    int pair = BL_PAIR(o->cl,o->op);

    switch (pair)                           // dispatch event
    {
      case BL_PAIR(_SET,PRV_):            // provisioned state changed
        provisioned = val;
        bl_log_color(attention,provisioned);
        LOG(2,BL_M"node %sprovisioned",val?"":"un");
        return bl_out(o,val,output);

      case BL_PAIR(_SET,ATT_):          // attention state changed
        attention = val;
        bl_log_color(attention,provisioned);
        LOG(2,BL_G"attention %s",val?"on":"off");
        return bl_out(o,val,output);

      case BL_PAIR(_SYS,TICK_):
      case BL_PAIR(_SYS,TOCK_):
      case BL_PAIR(_SCAN,ADV_):
        level = 5;
        break;

      default:
        break;
    }

    LOGO(level,"@",o,val);
    return bl_out(o,val,output);            // forward message to a
  }

//==============================================================================
// dummy interface for core module public interface (default/__weak)
//==============================================================================

  __weak int bl_core(BL_ob *o, int val)
  {
    return -1;                              // not supported by default
  }

//==============================================================================
// message downward posting to lower level / driver layer (default/__weak)
//==============================================================================

  __weak int bl_down(BL_ob *o, int val)
  {
    bl_logo(3,BL_Y "down",o,val);
    return bl_core(o,val);
  }

//==============================================================================
// message upward posting to API layer (default/__weak)
//==============================================================================

  __weak int bl_up(BL_ob *o, int val)
  {
    bl_logo(3,BL_Y "@up",o,val);
    return bl_in(o,val);
  }

//==============================================================================
// post general message [CL:OP @id,<data>,val] to module
// - usage: bl_msg(module,cl,op,id,data,val)
//==============================================================================

  int bl_msg(BL_fct module, BL_cl cl, BL_op op, int id, void* data, int val)
  {
    BL_ob oo = {cl,op,id,data};
    return module(&oo,val);            // post message to module interface
  }

//==============================================================================
// post system message to module
// - usage: bl_sys(module,op,cb,val)   // post [SYS:op @0,<cb>,val] to module
//==============================================================================

  int bl_sys(BL_fct module, BL_op op, BL_fct cb, int val)
  {
    BL_ob oo = {_SYS,op,0,cb};
    return module(&oo,val);            // post message to module interface
  }

//==============================================================================
// handle message addressed to module, characterized by opcode
// - usage: bl_hdl(module,op,id,val)   // post [HDL:op @id,val] to module
//==============================================================================

  int bl_hdl(BL_fct module, BL_op op, int id, int val)
  {
    BL_ob oo = {_HDL,op,id,NULL};
    return module(&oo,val);            // post message to module interface
  }

//==============================================================================
// emit message to be handeled to output subscriber
// - usage: bl_emit(o,cl,op,val,output)  // post [cl:op o->id,val] to output
//==============================================================================

  int bl_emit(BL_ob *o, BL_cl cl, BL_op op, int val, BL_fct output)
  {
    BL_ob oo = {cl,op,o->id,o->data};
    return bl_out(&oo,val,output);
  }

//==============================================================================
// post message (with main parameters to SYS interface of given module)
// - usage: bl_post(module,opcode,id,val)  // class=_SYS, data=NULL
//==============================================================================

  int bl_post(BL_fct module, BL_op op, int id, int val)
  {
    BL_ob oo = {_SYS,op,id,NULL};
    return module(&oo,val);            // post message to module interface
  }

//==============================================================================
// forward a received message to an interface of a given module
// - usage: bl_fwd(module,cl,o,val)    // only interface class to be changed
//==============================================================================

  int bl_fwd(BL_fct module, BL_cl cl, BL_ob *o, int val)
  {
    BL_ob oo = {cl,o->op,o->id,o->data};
    return module(&oo,val);            // forward message to module interface
  }

//==============================================================================
// subscribe to message output
// - usage: bl_when(module,cb)         // class=_SYS, val=0, data=NULL
//==============================================================================

  int bl_when(BL_fct module, BL_fct cb)
  {
    BL_ob oo = {_SYS,WHEN_,0,cb};
    return module(&oo,0);              // post [SYS:WHEN <cb>] to module
  }

//==============================================================================
// check wheter module can be used
//==============================================================================

  int bl_use(BL_fct module, BL_txt msg)
  {
    bool use = false;                       // cannot used by default
    BL_ob oo = {_SYS,USE_,0,NULL};

    if (module)
      use = module(&oo,0);

    if (!use && bl_dbg(0))
    {
      printk(BL_R "error: %s not ready for use (looping ...)\n"BL_0, msg);
      for (;;)
        bl_sleep(10);
    }

    return use;
  }

//==============================================================================
// Blucino init                             // init Blucino system
// usage:  bl_init(NULL,when,verbose)       // init Bluccino
//         bl_init(bl_gear,when_gear,0)     // init submodule
//==============================================================================

  int bl_init(BL_fct module,BL_fct cb)
  {
    return bl_sys(module,INIT_,cb,0);     // init module
  }

//==============================================================================
// Bluccino tick: send [SYS:TICK count] message to given module
// - usage: bl_tick(module,count)           // ticking a given module
//==============================================================================

  int bl_tick(BL_fct module, int id, int cnt)
  {
    return bl_post(module,TICK_,id,cnt);  // post [SYS:TICK cnt] message
  }

//==============================================================================
// Bluccino tock: send [SYS:TOCK count] message to given module
// - usage: bl_tock(module,count)           // tocking a given module
//==============================================================================

  int bl_tock(BL_fct module, int id, int cnt)
  {
    return bl_post(module,TOCK_,id,cnt);  // post [SYS:TOCK cnt] message
  }

//==============================================================================
// public module interface: supporting [SYS:INIT|WHEN|TICK|TOCK]
//==============================================================================

  int bluccino(BL_ob *o, int val)
  {
    switch (BL_PAIR(o->cl,o->op))
    {
      case (BL_PAIR(_SYS,INIT_)):
        output = o->data;
        bl_init(bl_core,bl_up);        // init core, subscribe with bl_up()
        return 0;

      case (BL_PAIR(_SYS,WHEN_)):
        output = o->data;
        return 0;

      case (BL_PAIR(_SYS,TICK_)):
      case (BL_PAIR(_SYS,TOCK_)):
        return 0;                      // nothing to tick/tock

      default:
        return -1;                     // bad command
    }
  }
