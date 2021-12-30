//==============================================================================
// main.c for 04-sos demo (hello Bluccino)
//==============================================================================
//
// main taks:
// - 1) setup a main engine with 500 ms period tick generatiom
// - 2) main engine to call Bluccino and app initializers
// - 3) main engine to call Bluccino ticker and app tick() function
// - 4) app init() to init SOS and LED module
// - 5) app init() to setup message forwarding from SOS module to LED module
// - 6) app tick() to tick SOS module
//
//                    +-------------+         +-------------+
//             INIT -->             |  LEVEL  |             |
//                    |     SOS     |--------->     LED     |
//    (500 ms) TICK -->             |         |             |
//                    +-------------+         +-------------+
// LEVEL
//  ^
//  |== +== +==   +======     +======     +======   +== +== +==        +==
//  | | | | | |   |     |     |     |     |     |   | | | | | |        | |
// -+-+==-+==-+====-----+======-----+======-----+====-+==-+==-+===...===-+=...>
//
//==============================================================================

#include "bluccino.h"
#include "led.h"
#include "sos.h"

//==============================================================================
// app tick functions
//==============================================================================

  static void tick(BL_ob *o, int val)  // tick all systems
  {
     bl_logo(1,"main",o,val);          // log to see we are alife
     sos_tick(val);
  }

//==============================================================================
// app init
// - 1) init all modules of app (note: Bluccino init is done in main engine!)
// - 2) setup connections: all outputs of SOS module have to go to LED module
//==============================================================================

  static void init(void)               // init all modules
  {
    led_init(1);                       // init LED module, act on LED @1
    sos_init(led);                     // init SOS module, output to LED module
  }

//==============================================================================
// main engine
// - calling Bluccino init and app init() function
// - periodic (500ms) calls of app tick() function
//==============================================================================

  void main(void)
  {
    BL_ob oo = {CL_SOS,OP_TICK,0,NULL};
    static int count = 0;              // tick counter

    bl_verbose(2);                     // set verbose level 2
    bl_init(NULL,NULL);                // Bluccino init
    init();                            // app init

    for(;;)                            // loop generating (approx) 500ms ticks
    {
      tick(&oo,count++);               // app tick
      bl_sleep(500);                   // sleep 500 ms
    }
  }
