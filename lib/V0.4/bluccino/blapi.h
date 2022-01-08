//==============================================================================
// blapi.h - bluccino API
//==============================================================================

#ifndef __BLAPI_H__
#define __BLAPI_H__

#include "bllog.h"
#include "bltype.h"
#include "blmsg.h"

  extern bool bl_attention;            // attention mode
  extern bool bl_provisioned;          // provisioned mode

//==============================================================================
// mesh opcodes
//==============================================================================

  #define BL_GOOLET 0x8203

//==============================================================================
// syntactic sugar: compound message identifier
// - usage: bl_id(o)                   // same as BL_PAIR(o->cl,o->op)
//==============================================================================

  static inline int bl_id(BL_ob *o)
  {
    return BL_PAIR(o->cl,o->op);
  }

//==============================================================================
// us/ms clock
//==============================================================================

  BL_us bl_zero(void);                 // reset clock
  BL_us bl_us(void);                   // get current clock time in us
  BL_ms bl_ms(void);                   // get current clock time in ms

//==============================================================================
// timing & sleep, system halt
//==============================================================================

  bool bl_due(BL_ms *pdue, BL_ms ms);  // check if time if due & update
  void bl_sleep(BL_ms ms);             // deep sleep for given milliseconds
  void bl_halt(BL_txt msg, BL_ms ms);  // halt system

//==============================================================================
// event message emission, posting and notification
//==============================================================================

  int bl_out(BL_ob *o, int value, BL_fct call);
  int bl_in(BL_ob *o, int value);

//==============================================================================
// gear upward/downward and THE gear interface
//==============================================================================

  int bl_up(BL_ob *o, int value);      // gear input function
  int bl_down(BL_ob *o, int value);    // gear output function
  int bl_gear(BL_ob *o, int val);      // gear module interface
  int bl_core(BL_ob *o, int val);      // core module interface

//==============================================================================
// post general message [CL:OP @id,<data>,val] to module
// - usage: bl_msg(module,cl,op,id,data,val)
//==============================================================================

  int bl_msg(BL_fct module, BL_cl cl, BL_op op, int id, void* data, int val);

//==============================================================================
// post message (with main parameters to POST interface ofa given module)
// - usage: bl_post(module,opcode,id,val) // class=CL_SYS, data=NULL
//==============================================================================

  int bl_post(BL_fct module, BL_op op, int id, int val);

//==============================================================================
// forward a received message to an interface of a given module
// - usage: bl_fwd(module,cl,o,val)    // only interface class to be changed
//==============================================================================

  int bl_fwd(BL_fct module, BL_cl cl, BL_ob *o, int val);

//==============================================================================
// post system message to module
// - usage: bl_sys(module,op,cb,val)   // post [SYS:op @0,<cb>,val] to module
//==============================================================================

  int bl_sys(BL_fct module, BL_op op, BL_fct cb, int val);

//==============================================================================
// handle message addressed to module, characterized by opcode
// - usage: bl_hdl(module,op,id,val)   // post [HDL:op @id,val] to module
//==============================================================================

  int bl_hdl(BL_fct module, BL_op op, int id, int val);

//==============================================================================
// subscribe to a module's message output
// - usage: bl_sub(module,cb)          // class=CL_SYS, id=0, val=0
//==============================================================================

  int bl_when(BL_fct module, BL_fct cb);

//==============================================================================
// invoke a test message (sends to upward gear, where it needs to be forwarded)
//==============================================================================

  static inline int bl_test(int op, int id, int mode)
  {
    BL_ob oo = {CL_TEST,op,id,NULL};
    return bl_up(&oo,mode);            // post to test module via upward gear
  }

//==============================================================================
// get module property
// - usage: val = bl_get(module,op)    // use opcodes for property names
//==============================================================================

  static inline int bl_get(BL_fct module, BL_op op)
  {
    BL_ob oo = {CL_GET,op,0,NULL};
    return module(&oo,0);              // post to test module via upward gear
  }

//==============================================================================
// set module property
// - usage: val = bl_set(module,op,val) // use opcodes for property names
//==============================================================================

  static inline int bl_set(BL_fct module, BL_op op, int val)
  {
    BL_ob oo = {CL_SET,op,0,NULL};
    return module(&oo,val);            // post to test module via upward gear
  }

//==============================================================================
// ticking/tocking a module (syntactic sugar: id = 0, val = 0)
// - usage: bl_tick(module,id,cnt)     // post [SYS:TICK @id,cnt] to a module
// -        bl_tock(module,id,cnt)     // post [SYS:TOCK @id,cnt] to a module
//==============================================================================

  int bl_tick(BL_fct module, int id, int cnt);  // ticking a module
  int bl_tock(BL_fct module, int id, int cnt);  // tocking a module

//==============================================================================
// bl_hello (syntactic sugar to set verbose level and print a hello message)
// - usage: bl_hello(verbose,msg)
//==============================================================================

  static inline void bl_hello(int verbose, BL_txt msg)
  {
    bl_verbose(verbose);               // set verbose level
    if (bl_dbg(0))                     // always - start print with time stamp
      bl_prt(BL_R "%s\n" BL_0,msg);    // print hello message in red
  }

//==============================================================================
// bl_data (syntactic sugar to cast const void* data to non-const)
// - usage: MY_data *p = bl_data(o)
//==============================================================================

  static inline void *bl_data(BL_ob *o)
  {
    return (void*)o->data;
  }

//==============================================================================
// bl_init (syntactic sugar to initialize a given module)
// - usage: bl_init(module,cb)  // post [SYS:INIT <cb>] to a module
//==============================================================================

  int bl_init(BL_fct module,BL_fct cb);

//==============================================================================
// public module interface: supporting [SYS:INIT|WHEN|TICK|TOCK]
//==============================================================================

  int bluccino(BL_ob *o, int val);

#endif // __BLAPI_H__
