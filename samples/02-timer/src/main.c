//==============================================================================
// main.c for 02-CL_TIMER demo (hello Bluccino)
//==============================================================================

#include "bluccino.h"
#include "timer.h"

//==============================================================================
// when callback
//==============================================================================

  int when(BL_ob *o, int val)
  {
    switch (o->op)
    {
      case OP_TICK:
        bl_logo(1,BL_B"when",o,val);
        return 0;
    }
    return 0;
  }

//==============================================================================
// mandatory init/loop functions for app
//==============================================================================

  static void init()
  {
  }

  static void loop()                   // app loop
  {
     bl_sleep(10);                     // sleep 10 ms
  }

//==============================================================================
// Arduino style engine
//==============================================================================

  void main(void)
  {
    bl_init(NULL,2);                   // Bluccino init
    timer_init(when);                  // init TIMER module
    init();                            // app init

    for(;;)
    {
      bl_loop();                       // run Bluccino loop
      timer_loop();                    // run Bluccino loop
      loop();                          // run app loop
    }
  }
