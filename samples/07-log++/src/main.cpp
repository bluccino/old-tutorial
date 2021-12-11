//==============================================================================
// main.cpp for 06-log demo
//==============================================================================

#include "bluccino.h"

  System sys(3);                       // trace level 3
  GooCli cli(1);                       // GOO client in element 1

  void main(void)
  {
    sys.init(NULL);

    for(int i=0; ;i++)
    {
      sys.log("alive", 0,i);
      cli.let(i%2);                    // unack'ed state set to value
      sys.sleep(1000);                 // sleep 100 ms
    }
  }
