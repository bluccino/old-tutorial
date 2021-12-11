//==============================================================================
// timer.c - a simple timer module
//==============================================================================

#include "bluccino.h"
#include "timer.h"

  static BL_notify notify = NULL;

//==============================================================================
// mandatory init/loop functions
//==============================================================================

  static BL_ob tim = {CL_TIMER,OP_TICK,0,NULL};
  static BL_ms time = 0;

  void timer_init(BL_notify cb)
  {
    notify = cb;
  }

  void timer_loop(void)
  {
    if (bl_ms() >= time)
    {
      bl_logo(3,"loop",&tim,time);
      bl_out(&tim,time,notify);
      time += 1000;                    // next time = current time + 1000 ms
    }
  }
