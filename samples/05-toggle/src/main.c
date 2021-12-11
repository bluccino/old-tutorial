//==============================================================================
// main.c for 04-toggle demo
//==============================================================================

#include "bluccino.h"    // direct access to the core (will change this)

  BL_obj goo = {1,BL_MESH,BL_GOOLET,NULL};   // id = 1


  static void init()
  {
  }

  static void loop()                   // app loop
  {
     static int cnt = 1;               // mind: must be static!
     bool value = cnt++ % 2;           // toggle this value

     printk("[%03d] alive\n", cnt);
     bl_set(&goo,value);

     k_msleep(1000);                   // sleep 100 ms
  }

//==============================================================================
// Arduino style engine
//==============================================================================

  void main(void)
  {
    bl_init(NULL,3);                   // init mesh/HW core, verbose level 3
    init();                            // app init

    for(;;)
    {
      bl_loop();                       // run mesh/HW core loop
      loop();                          // run app loop
    }
  }
