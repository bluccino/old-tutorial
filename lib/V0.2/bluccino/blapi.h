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
// run system operation on a module (syntactic sugar: id = 0, val = 0)
//==============================================================================

  int bl_sys(int op, BL_fct module, BL_fct cb);
///int bl_clop(int cl, int op, BL_fct module, int id, int val);

//==============================================================================
// invoke a test message (sends to upward gear, where it needs to be forwarded)
//==============================================================================

  static inline int bl_test(int op, int id, int mode)
  {
    BL_ob oo = {CL_TEST,op,id,NULL};
    return bl_up(&oo,mode);            // post to test module via upward gear
  }

//==============================================================================
// ticking/tocking a module (syntactic sugar: id = 0, val = 0)
//==============================================================================

  static inline int bl_tick(BL_fct module, int val)  // ticking a module
  {
    BL_ob oo = {CL_SYS,OP_TICK,0,NULL};
    return module(&oo,val);
  }

  static inline int bl_tock(BL_fct module, int val)  // tocking a module
  {
    BL_ob oo = {CL_SYS,OP_TOCK,0,NULL};
    return module(&oo,val);
  }

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  int bl_loop(BL_fct module);
  int bl_init(BL_fct module,BL_fct cb);
  int bl_use(BL_fct module, BL_txt msg);

#endif // __BLAPI_H__
