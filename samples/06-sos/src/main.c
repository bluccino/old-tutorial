//==============================================================================
// main.c for 04-sos demo (hello Bluccino)
//==============================================================================
//
//                    [:TICK]           [:ONOFF]
//                       |       +---------------------+
//                       v       |                     v
//                    +-------------+           +-------------+
//                    |             |           |             |
//                    |     SOS     |           |     LED     |
//                    |             |           |             |
//                    +-------------+           +-------------+
//
//  Message posting:
//    - [LED:ONOFF]    app switches LED on or off
//
//==============================================================================

#include "bluccino.h"
#include "led.h"
#include "sos.h"

//==============================================================================
// when callback
//==============================================================================

  int when(BL_ob *o, int val)
  {
    switch (o->op)
    {
      case OP_LEVEL:
        bl_logo(1,"@when",o,val);
        return led(o,val);
    }
    return 0;
  }

//==============================================================================
// mandatory init/loop functions
//==============================================================================

  static BL_ob oo = {CL_TIMER,OP_TICK,0,NULL};
  static int cnt = 0;                  // loop counter

  static void init()
  {
  }

  static void loop()                   // app loop
  {
     cnt++;
     bl_logo(3,"loop",&oo,cnt);        // live signal
     sos(&oo,cnt);
     bl_sleep(500);                    // sleep 1000 ms
  }

//==============================================================================
// Arduino style engine
//==============================================================================

  void main(void)
  {
    bl_init(NULL,when,2);              // Bluccino init
    bl_init(led,when,0);               // init LED module
    bl_init(sos,when,0);               // init SOS module
    init();                            // app init

    for(;;)
    {
      bl_loop(NULL);                   // run Bluccino loop
      bl_loop(led);                    // run LED loop
      bl_loop(sos);                    // run SOS loop
      loop();                          // run app loop
    }
  }
