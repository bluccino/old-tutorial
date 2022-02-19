//==============================================================================
// bc_hw.h
// Bluccino HW core supporting basic functions for button & LED
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
// LED interface:
// - LED messages [LED:SET @id onoff] control the onoff state of one of the four
// - LEDs @1..@4. LED @0 is the status LED which will be remapped to LED @1
//
// BUTTON interface
// - button presses notify with [BUTTON:PRESS @id 1]   (@id = 1..4)
// - button releases notify with [BUTTON:RELESE @id 1] (@id = 1..4)
//
// SWITCH interface
// - each button (1..4) is assigned with a logical switch which is toggled
//   on [BUTTON:PRESS @id,val] events
// - each change of the logical switch state is notified by a
//   [SWITCH:SET @id,onoff] event message
//
//==============================================================================

#ifndef __BC_HW_H__
#define __BC_HW_H__

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
//    - [BUTTON:PRESS @id,1]           // output a button press event
//    - [BUTTON:RELEASE @id,0]         // output a button release event
//    - [SWITCH:STS @id,onoff]         // output switch status update
//
//==============================================================================

  int bc_hw(BL_ob *o, int val);        // HW core module interface

//==============================================================================
// syntactic sugar: HW core init
// - usage: bc_hw_init(cb)
//==============================================================================

  static inline int bc_hw_init(BL_fct cb)
  {
    return bl_init(bc_hw,cb);
  }

//==============================================================================
// syntactic sugar: set LED @id on off (@id: 0..4)
// - usage: bc_hw_led(id,onoff)
//==============================================================================

  static inline int bc_hw_led(int id, bool onoff)
  {
    BL_ob oo = {_LED,SET_,id,NULL};
    return bc_hw(&oo,onoff);
  }

#endif // __BC_HW_H__
