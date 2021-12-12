//==============================================================================
// led.c - an LED dummy block
//==============================================================================

#include "bluccino.h"
#include "led.h"

  static BL_fct notify = NULL;

//==============================================================================
// input interface
//==============================================================================

  int led_in(BL_ob *o, int value)
  {
    switch (BL_PAIR(o->cl,o->op))
    {
      case BL_PAIR(CL_LED,OP_ONOFF):
      {
        bl_logo(1,"@led",o,value);
        break;
      }
    }
    return 0;
  }

//==============================================================================
// mandatory init/loop functions
//==============================================================================

  void led_init(BL_fct cb)
  {
    notify = cb;
  }

  void led_loop(void)
  {
    // nothing to do
  }
