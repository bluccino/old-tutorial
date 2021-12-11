//==============================================================================
// main.cpp for 07-tiny++ demo
//==============================================================================

#include "bluccino.h"

  void when(Button &o, int value);
  void when(GooSrv &o, int value);

  System sys(1);                       // trace level 1
  GooCli cli(1);                       // GOO client in element 1
  GooSrv srv(1,when);                  // GOO server in element 1
  Button button(1,when);               // access object for button 1
  Led led(1);                          // access object for LED

  void when(Button &o, int value)
  {
    sys.log("@button", o.id,value);    // log button press events
    cli.let(value);                    // unack'ed state set to value
  }

  void when(GooSrv &o, int value)
  {
    sys.log("@srv", o.id,value);       // log button press events
    led.set(value);                    // unack'ed state set to value
  }

  void main(void)
  {
    Init(4);                           // Bluccino init (verbose level 4)

    for(int i=0; ;i++)
    {
      sys.log("alive",i);
      button.set(i%2);                 // toggle button
      sys.sleep(2000);                 // sleep 100 ms
    }
  }
