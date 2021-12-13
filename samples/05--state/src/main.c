//==============================================================================
// main.c for 04-attention demo
//==============================================================================

#include "bluccino.h"

//==============================================================================
// when a message is emitted
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    switch (BL_PAIR(o->cl,o->op))
    {
      case BL_PAIR(CL_TIMER,OP_TICK):
      {
        static bool toggle = 0;
        toggle = !toggle;

        BL_ob oo = {CL_GOOSRV,OP_SET,0,NULL};
        bl_logo(1,"when",&oo,toggle);  // log message
        bl_in(&oo,toggle);             // send message to core
        break;
      }
    }
    return 0;
  }

//==============================================================================
// app init and app loop
//==============================================================================

  static void init(void)               // app init
  {
  }

  static void loop(void)               // app loop
  {
    static BL_ms tick = 0;             // next time stamp for tick emission
    static BL_ob oo = {CL_TIMER,OP_TICK,0,NULL};

tick=2000;
    bl_wait(tick);                     // sleep 1000 ms
bl_logo(1,BL_R"loop",&oo,(int)tick);
for(;;);
    bl_out(&oo,(int)tick,when);        // emit message subscribed by when-cb

    tick += 1000;                      // next time stamp to emit tick event
  }

//==============================================================================
// Arduino style engine
//==============================================================================

  void main(void)
  {
    bl_init(NULL,when,4);              // Bluccino init (when-callback passing)
    init();                            // app init

    for(;;)
    {
      bl_loop(NULL);                   // run Bluccino loop
      loop();                          // run app loop
    }
  }
