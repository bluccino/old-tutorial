//==============================================================================
// main.c for 02-blink (tiny LED toggle control by buttons)
//==============================================================================

  #include "bluccino.h"

  static int app(BL_ob *o, int val)
  {
    return bl_is(o,_SYS,TOCK_) ? bl_led(1,-1) : 0;    // toggle LED status
  }

  void main(void)
  {
    bl_hello(5,"02-bink (tiny blink program)");
    bl_run(app,10,1000,NULL);
  }
