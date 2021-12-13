//==============================================================================
// led.c - an LED dummy block
//==============================================================================

#include "bluccino.h"
#include "led.h"

  static BL_ob oo = {CL_GOOSRV,OP_SET,0,NULL};

//==============================================================================
// THE led interface function
//==============================================================================

  int led(BL_ob *o, int value)
  {
    switch (o->op)
    {
      case OP_INIT:
      case OP_LOOP:
        break;

      case OP_LEVEL:
      {
        bl_logo(1,"@led",o,value);
        for (oo.id = 1; oo.id <= 4; oo.id++)
           bl_in(&oo,value);                  // send value to all 4 LEDs

        break;
      }
    }
    return 0;
  }
