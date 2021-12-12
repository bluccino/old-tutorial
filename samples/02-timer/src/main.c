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
        bl_logo(1,BL_G"when",o,val);
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
// framework engine
//==============================================================================

  void main(void)
  {
    bl_init(NULL,NULL,2);              // Bluccino init (no callback passing)
    bl_init(timer,when,0);             // TIMER init (pass 'when' callback)
    init();                            // app init

    for(;;)
    {
      bl_loop(NULL);                   // run Bluccino loop
      bl_loop(timer);                  // run TIMER loop
      loop();                          // run app loop
    }
  }
