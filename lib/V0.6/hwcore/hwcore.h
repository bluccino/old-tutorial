//==============================================================================
// hwcore.h
// Bluccino HW core supporting basic functions for button & LED
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
// SYS Interface:  [] = SYS(INIT)
// LED Interface:  [] = LED(SET)
//
//                             +-------------+
//                             |    BL_HW    |
//                             +-------------+
//                       SET ->|    LED:     |
//                             +-------------+
//                     PRESS ->|   BUTTON:   |-> PRESS
//                   RELEASE ->|             |-> RELEASE
//                             +-------------+
//  Input Messages:
//    - [SYS:INIT <cb>]                // init module, provide output callback
//    - [LED:SET @id onoff]            // set LED @id on/off (id=0..4)
//    - [BUTTON:PRESS @id,active]      // relay a button press event to output
//    - [BUTTON:RELEASE @id,active]    // relay a button release event to output
//
//  Output Messages:
//    - [BUTTON:PRESS @id 1]           // output a button press event
//    - [BUTTON:RELEASE @id 0]         // output a button release event
//
//==============================================================================
// LED interface:
// - LED messages [LED:SET @id onoff] control the onoff state of one of the four
// - LEDs @1..@4. LED @0 is the status LED which will be remapped to LED @1
//
// Button interface
// - button presses notify with [BUTTON:PRESS @id 1] with @id = 1..4
// - button releases notify with [BUTTON:RELESE @id 1] with @id = 1..4
//==============================================================================

#ifndef __HWCORE_H__
#define __HWCORE_H__

//==============================================================================
// public module interface
//==============================================================================

  int bl_hw(BL_ob *o, int val);        // HW core module interface

//==============================================================================
// syntactic sugar: HW core init
// - usage: bl_hw_init(cb)
//==============================================================================

  static inline int bl_hw_init(BL_fct cb)
  {
    return bl_init(bl_hw,cb);
  }

//==============================================================================
// syntactic sugar: set LED @id on off (@id: 0..4)
// - usage: bl_hw_led(id,onoff)
//==============================================================================

  static inline int bl_hw_led(int id, bool onoff)
  {
    BL_ob oo = {_LED,SET_,id,NULL};
    return bl_hw(&oo,onoff);
  }

#endif // __HWCORE_H__
