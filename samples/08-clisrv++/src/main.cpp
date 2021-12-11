//==============================================================================
// main.cpp for 07-clisrv++ demo
//==============================================================================

#include "bluccino.h"

  GooCli cli(1);                       // GOO client in element 1

  void when(Button &o, int value)
  {
    cli.log("@button", o.id,value);    // log button press events
    cli.let(value);                    // unack'ed state set to value
  }

  Button button(1,when);               // access object for button 1
  Led led(1);                          // access object for LED

  void when(GooSrv &o, int value)
  {
    led.log("@srv", o.id,value);       // log button press events
    led.set(value);                    // unack'ed state set to value
  }

  GooSrv srv(1,when);                  // GOO server in element 1

  void main(void)
  {
    Init(4);                           // Bluccino init (verbose level 4)

    for(;;)
      Loop(2000,"07-clisrv++");        // log & sleep 2000 ms
  }
