//==============================================================================
// main.cpp for 07-tiny++ demo
//==============================================================================

#include "bluccino.h"

  void when_button(Button &o, int value);

  System sys(1);                       // trace level 1
  GooCli cli(1);                       // GOO client in element 1
  Button button(1,when_button);        // access object for button 1

  void when_button(Button &o, int value)
  {
      sys.log("@button", o.id,value);  // log button press events
      cli.let(value);                  // unack'ed state set to value
  }

  void main(void)
  {
    Init(4);                           // Bluccino init (verbose level 4)

    for(int i=0; ;i++)
    {
      bool value = i % 2;              // toggle this value
      sys.log("alive", 0,i);
      sys.sleep(2000);                 // sleep 100 ms
    }
  }
