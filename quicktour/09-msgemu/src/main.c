//==============================================================================
// main.c for 09-msgstudy (Bluccino messaging study)
//==============================================================================

  #include "bluccino.h"                // 09-msgstudy

  static int when(BL_ob *o, int val)   // callback, catching Bluccino events
  {                                    // o->id represents button @id
    if ( bl_switch(o) )                // [SWITCH:STS @id,val] message?
      bl_led(o->id,val);               // set LED(@id) on/off
    return 0;                          // OK
  }

  void main(void)
  {
    bl_hello(5,"09-msgstudy (Bluccino messaging study");
    BL_ob oo = {_SWITCH,STS_,1,NULL};

    for (int v=0;;i++)
    {
      bl_up(&oo,i%2);                  // post [SWITCH,STS @1,]
      bl_sleep(5000);
    }
  }
