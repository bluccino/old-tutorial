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
        return led_in(o,val);
    }
    return 0;
  }

//==============================================================================
// mandatory init/loop functions
//==============================================================================

  static BL_ob timer = {CL_TIMER,OP_TICK,0,NULL};
  static int cnt = 0;                  // loop counter

  static void init()
  {
  }

  static void loop()                   // app loop
  {
     cnt++;
     bl_logo(3,"loop",&timer,cnt);     // live signal
     sos_in(&timer,cnt);
     bl_sleep(1000);                   // sleep 1000 ms
  }

//==============================================================================
// Arduino style engine
//==============================================================================

  void main(void)
  {
    bl_init(when,2);                   // Bluccino init
    led_init(when);                    // init LED module
    sos_init(when);                    // init SOS module
    init();                            // app init

    for(;;)
    {
      bl_loop();                       // run Bluccino loop
      loop();                          // run app loop
    }
  }
