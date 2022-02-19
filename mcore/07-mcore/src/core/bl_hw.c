//==============================================================================
// bl_hw.c
// Bluccino HW core supporting basic functions for button & LED
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================

  #define init  init_button
  #include "bl_hwbut.c"                // button core driver

  #undef LOG
  #undef LOGO
  #undef LOG0
  #undef init

  #define init  init_led
  #include "bl_hwled.c"                // LED core driver

//==============================================================================
// public module interface
//==============================================================================
//
// BC_HW Interfaces:
//   SYS Interface:     [] = SYS(INIT)
//   LED Interface:     [] = LED(SET)
//   BUTTON Interface:  [PRESS,RELEASE] = BUTTON(PRESS,RELEASE)
//   SWITCH Interface:  [STS] = SWITCH(STS)
//
//                             +-------------+
//                             |    BC_HW    |
//                             +-------------+
//                      INIT ->|    SYS:     |
//                             +-------------+
//                       SET ->|    LED:     |
//                    TOGGLE ->|             |
//                             +-------------+
//                     PRESS ->|   BUTTON:   |-> PRESS
//                   RELEASE ->|             |-> RELEASE
//                             +-------------+
//                       STS ->|   SWITCH:   |-> STS
//                             +-------------+
//  Input Messages:
//    - [SYS:INIT <cb>]                // init module, provide output callback
//    - [LED:SET @id onoff]            // set LED @id on/off (id=0..4)
//    - [LED:TOGGLE @id]               // toggle LED(@id), (id: 0..4)
//    - [BUTTON:PRESS @id,active]      // forward button press event to output
//    - [BUTTON:RELEASE @id,active]    // forward button release event to output
//    - [SWITCH:STS @id,onoff]         // forward switch status update to output
//
//  Output Messages:
//    - [BUTTON:PRESS @id,1]           // output button press event
//    - [BUTTON:RELEASE @id,0]         // output button release event
//    - [SWITCH:STS @id,onoff]         // output switch status update
//
//==============================================================================

  int bl_hw(BL_ob *o, int val)         // HW core module interface
  {
    static BL_fct output = NULL;       // to store output callback

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):          // [SYS:INIT <cb>]
      {
        output = o->data;              // store output callback
        BL_ob oo = {_SYS,INIT_,o->id,bl_hw};

          // for BC_BUTTON and BC_LED output needs to go to BC_HW

        bl_hwbut(&oo,val);             // init BUTTON module
        bl_hwled(&oo,val);               // init LED module
      	return 0;                      // OK
      }

      case BL_ID(_LED,SET_):           // [LED:set @id,val]
      case BL_ID(_LED,TOGGLE_):        // [LED:toggle @id]
        return bl_hwled(o,val);

      case BL_ID(_BUTTON,PRESS_):      // [BUTTON:PRESS @id,val]
      case BL_ID(_BUTTON,RELEASE_):    // [BUTTON:RELEASE @id,val]
      case BL_ID(_SWITCH,STS_):        // [SWITCH:STS @id]
	      return bl_out(o,val,output);   // output message

      default:
	      return -1;                     // bad input
    }
  }
