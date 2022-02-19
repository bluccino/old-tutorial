//==============================================================================
// main.c for 09-tiny (tiny LED toggle control by buttons)
//==============================================================================

#include "bluccino.h"
#include "blbutton.h"

//==============================================================================
// TEST level logging shorthands
//==============================================================================

  #define LOG                     LOG_TEST

  static int bl_led(int id,  bool onoff)
  {
    BL_ob oo = {_LED,SET_,id,NULL};
    return bl_down(&oo,onoff);
  }

//==============================================================================
// when callback (when a Bluccino event happens)
//==============================================================================

  static int when(BL_ob *o, int val)
  {
    if (bl_id(o) == BL_ID(_BUTTON,PRESS_))  // [BUTTON:PRESS @id]
      bl_led(o->id,val);
    return 0;                               // OK
  }

//==============================================================================
// main program
//==============================================================================

  void main(void)
  {
    bl_hello(4,"09-tiny (click button to toggle related LED");

    bl_init(bluccino,when);            // init Bluccino - output goes to when()
//  bl_init(bl_hw,when);               // init HW core, output goes to when()
    bl_init(bl_button,when);           // init button driver, output => when()
  }
