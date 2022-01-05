//=== rgb.c ====================================================================
//                                                +----------+
// - LED interface: [] = LED(INIT,SET)     INIT ->|          |
//                                                |   LED    |
//                                          SET ->|          |
// - [LED:SET @id onoff] turn LED on/off          +----------+
//==============================================================================

  #include "bluccino.h"           // use Bluccino framework

//=== LED module ===============================================================

  int led(BL_ob *o, int val)
  {
    static const char *color = "on";

    switch (BL_PAIR(o->cl,o->op))
    {
      case BL_PAIR(CL_LED,OP_INIT):
        color = o->data;
        return 0;
      case BL_PAIR(CL_LED,OP_SET):
        printf("LED %d is %s\n", o->id, val?color:"off");
        return 0;
      default:
        return -1;                // bad input
    }
  }

//=== main program =============================================================

  int main()
  {
    BL_ob oi = {CL_LED,OP_INIT,0,"red"}
    led(&oi,0);                   // send [LED:INIT <"red">] to LED module
    BL_ob oo = {CL_LED,OP_SET,3,NULL}; // control LED @3

    led(&oo,1);                   // send [LED:SET @3 1] to LED module
    led(&oo,0);                   // send [LED:SET @3 0] to LED module
    return 0;
  }
