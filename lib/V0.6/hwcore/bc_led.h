//==============================================================================
// bc_led.h
// basic LED functions
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
// LED interface:
// - LED messages [LED:SET @id onoff] control the onoff state of one of the four
// - LEDs @1..@4. LED @0 is the status LED which will be remapped to LED @1
//==============================================================================

#ifndef __BC_LED_H__
#define __BC_LED_H__

//==============================================================================
// public module interface
//==============================================================================
//
// SYS Interface:  [] = SYS(INIT)
// LED Interface:  [] = LED(SET)
//
//                             +-------------+
//                             |   BC_LED    |
//                             +-------------+
//                      INIT ->|    SYS:     |
//                             +-------------+
//                       SET ->|    LED:     |
//                             +-------------+
//  Input Messages:
//    - [SYS:INIT <cb>]                // init module, provide output callback
//    - [LED:SET @id onoff]            // set LED @id on/off (id=0..4)
//
//==============================================================================

  int bc_led(BL_ob *o, int val);       // HW core module interface

//==============================================================================
// syntactic sugar: LED moduler init
// - usage: bc_led_init(cb)
//==============================================================================

  static inline int bc_led_init(BL_fct cb)
  {
    return bl_init(bc_led,cb);
  }

//==============================================================================
// syntactic sugar: set LED @id on off (@id: 0..4)
// - usage: bc_led_set(id,val)
//==============================================================================

  static inline int bc_led_set(int id, int val)
  {
    BL_ob oo = {_LED,SET_,id,NULL};
    return bc_led(&oo,val);
  }

#endif // __BC_LED_H__
