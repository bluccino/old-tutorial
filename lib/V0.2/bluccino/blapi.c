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

  #define LOG   LOG_API                // API level generic logging shorthand
  #define LOGO  LOGO_API               // API level object  logging shorthand

//==============================================================================
// provisioned & attention state
//==============================================================================

  static bool provisioned = 0;
  static bool attention = 0;

//==============================================================================
// notification and driver callbacks
//==============================================================================

  static BL_fct notify = NULL;      // notification callback

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

  int bl_out(BL_ob *o, int value, BL_fct call)
  {
    if (call)                          // is an app callback provided?
      return call(o,value);            // forward event message to app

    return 0;
  }

//==============================================================================
// input a message to Bluccino API
//==============================================================================

  int bl_in(BL_ob *o, int value)
  {
    int level = 2;                          // default verbose level
    int pair = BL_PAIR(o->cl,o->op);

    switch (pair)                           // dispatch event
    {
      case BL_PAIR(CL_MESH,OP_PRV):         // provisioned state changed
        provisioned = value;
        bl_log_color(attention,provisioned);
        bl_log1(2,"@mesh:provisioned",provisioned);
        return bl_out(o,value,notify);

      case BL_PAIR(CL_MESH,OP_ATT):         // attention state changed
        attention = value;
        bl_log_color(attention,provisioned);
        bl_log1(2,"@mesh:attention",attention);
        return bl_out(o,value,notify);

      case BL_PAIR(CL_TIMER,OP_TICK):
      case BL_PAIR(CL_SCAN,OP_ADV):
        level = 5;
        break;

      default:
        break;
    }

    LOGO(level,"@api",o,value);
    return bl_out(o,value,notify);          // forward message to a
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
bl_logo(0,BL_M,o,val);
    return bl_core(o,val);
  }

//==============================================================================
// message upward posting to API layer (default/__weak)
//==============================================================================

  __weak int bl_up(BL_ob *o, int val)
  {
    return bl_in(o,val);
  }

//==============================================================================
// run system operation on a module (syntactic sugar: id = 0, val = 0)
//==============================================================================

  int bl_sys(int op, BL_fct module, BL_fct cb)
  {
    BL_ob oo = {CL_SYS,op,0,cb};
    return module(&oo,0);
  }

  int bl_clop(int cl, int op, BL_fct module, int id, int val)
  {
    BL_ob oo = {cl,op,id,NULL};
    return module(&oo,val);
  }

//==============================================================================
// check wheter module can be used
//==============================================================================

  int bl_use(BL_fct module, BL_txt msg)
  {
    bool use = false;                       // cannot used by default
    BL_ob oo = {CL_SYS,OP_USE,0,NULL};

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
    if (module)
      return bl_sys(OP_INIT,module,cb);     // init module

    notify = cb;
//  bl_sys(OP_INIT,bl_core,when_core);      // init core
    bl_sys(OP_INIT,bl_core,NULL);           // init core
    return 0;
  }

//==============================================================================
// Bluccino loop                            // loop Bluccino system
// usage:   bl_loop(NULL)                   // Blucino API level loop
//          bl_loop(bl_gear)                // gear level loop
//==============================================================================

  int bl_loop(BL_fct module)
  {
    if (module)
      return bl_sys(OP_LOOP,module,NULL);   // run specific module loop
    else
    {
      // bl_sys(OP_LOOP,bl_core,NULL);      // run core loop
      return 0;
    }
  }
