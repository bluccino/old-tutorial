//=== simple.c =================================================================
//                                                 +-------+
// - LED interface: [] = LED(SET)            SET ->|  LED  |
// - [LED:SET @id onoff] turn LED on/off           +-------+
//==============================================================================

  #include "bluccino.h"           // use Bluccino framework

//=== LED module ===============================================================

  int led(BL_ob *o, int val)      // or: int led(OVAL)
  {
    if (o->cl == CL_LED && o->op == OP_SET)
      printf("LED %d is %s\n", o->id, val?"on":"off");
    return 0;                     // all went OK
  }

//=== main program =============================================================

  int main()
  {
    BL_ob oo = {CL_LED,OP_SET,3,NULL}; // control LED @3

    led(&oo,1);                   // send [LED:SET @3 1] to LED module
    led(&oo,0);                   // send [LED:SET @3 0] to LED module
    return 0;
  }
