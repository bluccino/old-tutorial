//==============================================================================
// main.c for 02-tinyled (tiny LED toggle control by buttons)
//==============================================================================

  #include "bluccino.h"

  static int when(BL_ob *o, int val)   // callback, catching Bluccino events
  {                                    // o->id represents button @id
    if ( bl_switch(o) )                // [SWITCH:STS @id,val] message?
      bl_led(o->id,val);               // set LED(@id) on/off
    return 0;                          // OK
  }

  void main(void)
  {
    bl_hello(5,"09-tiny (click button to toggle related LED");
    bl_init(bluccino,when);            // init Bluccino - output goes to when()
  }
