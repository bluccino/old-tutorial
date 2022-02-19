//==============================================================================
// blbutton.h
// Bluccino button driver supporting button basic functions
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
// SYS Interface:  [] = SYS(INIT)
// BUTTON Interface:  [] = BUTTON(SET)
//
//                             +-------------+
//                             |    BL_HW    |
//                             +-------------+
//                     PRESS ->|   BUTTON:   |-> PRESS
//                   RELEASE ->|             |-> RELEASE
//                             +-------------+
//  Input Messages:
//    - [SYS:INIT <cb>]                // init module, provide output callback
//    - [BUTTON:PRESS @id,active]      // relay a button press event to output
//    - [BUTTON:RELEASE @id,active]    // relay a button release event to output
//
//  Output Messages:
//    - [BUTTON:PRESS @id 1]           // output a button press event
//    - [BUTTON:RELEASE @id 0]         // output a button release event
//
//==============================================================================
// Button interface
// - button presses notify with [BUTTON:PRESS @id 1] with @id = 1..4
// - button releases notify with [BUTTON:RELESE @id 1] with @id = 1..4
//==============================================================================

#ifndef __BLBUTTON_H__
#define __BLBUTTON_H__

//==============================================================================
// public module interface
//==============================================================================

  int bl_button(BL_ob *o, int val);      // HW core module interface

//==============================================================================
// syntactic sugar: HW core init
// - usage: bl_hw_init(cb)
//==============================================================================

  static inline int bl_button_init(BL_fct cb)
  {
    return bl_init(bl_button,cb);
  }

//==============================================================================
// syntactic sugar: pseudo-invoke button press event (@id: 0..4)
// - usage: bl_button_press(id)
//==============================================================================

  static inline int bl_button_press(int id)
  {
    BL_ob oo = {_BUTTON,PRESS_,id,NULL};
    return bl_button(&oo,1);                // pass 1 to indicate 'active'
  }

//==============================================================================
// syntactic sugar: pseudo-invoke button release event (@id: 0..4)
// - usage: bl_button_release(id)
//==============================================================================

  static inline int bl_button_release(int id)
  {
    BL_ob oo = {_BUTTON,RELEASE_,id,NULL};
    return bl_button(&oo,0);                // pass 0 to indicate 'inactive'
  }

#endif // __BLBUTTON_H__
