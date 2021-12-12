//==============================================================================
// main.cpp for 08-goosrv++ demo
//==============================================================================

#include "bluccino.h"

  Led led(1);                          // access object for LED

  void when(GooSrv &o, int value)
  {
    led.set(value);                    // unack'ed state set to value
  }

  GooSrv srv(1,when);                  // GOO server in element 1

  void main(void)
  {
    Init(2);                           // Bluccino init (verbose level 2)

    for(int i=0; ;i++)
      Loop(2000,"08-goosrv++");        // log & sleep 2000 ms
  }
