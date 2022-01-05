//==============================================================================
// main.c for 01-hello demo (hello Bluccino)
//==============================================================================

#include <zephyr.h>
#define bl_prt printk

//#include "bluccino.h"

  void main(void)
  {
    bl_prt("hello, Bluccino!\n");
  }
