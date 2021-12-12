//==============================================================================
// main.cpp for 09-gocli++ demo
//==============================================================================

#include "bluccino.h"

  GooCli cli(1);                       // GOO client in element 1

  void when(Button &o, int value)
  {
    cli.let(value);                    // unack'ed state set to value
  }

  Button button(1,when);               // access object for button 1

  void main(void)
  {
    Init(2);                           // Bluccino init (verbose level 4)

    for(;;)
      Loop(2000,"09-gocli++");         // log & sleep 2000 ms
  }
