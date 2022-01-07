//==============================================================================
// app_gpio.h
// app GPIO handling
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
// LED messages [LED:SET @id onoff] control the onoff state of one of the four
// LEDs @1..@4. LED @0 is the status LED which will be remapped to LED @1
//
// Button presses notify with [BUTTON:PRESS @id] whith @id = 1..4
//
//                                  MGPIO
//                             +-------------+
//                             |             |
//                       SET ->|    LED      |
//                             |             |
//                             +-------------+
//                             |             |
//                     PRESS ->o   BUTTON    |-> PRESS
//                             |             |
//                             +-------------+
//  Input Messages:
//    - [LED:SET @id onoff]   // set LED @id on/off (id=0..4)
//    - [BUTTON:PRESS @id]    // relay module internal message to output
//
//  Output Messages:
//    - [BUTTON:PRESS @id]    // notify button press
//
//==============================================================================
// Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
// Copyright (c) 2018 Vikrant More
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

#ifndef _APP_GPIO_H
#define _APP_GPIO_H

  #include "mcore.h"

  #if !MIGRATION_STEP4
    extern const struct device *button_device[4];
    extern const struct device *led_device[4];
  #endif

//==============================================================================
// public module interface
//==============================================================================

  int mgpio(BL_ob *o, int val);         // public module interface

//==============================================================================
// syntactic sugar: app GPIO init /init GPIO driver)
// - usage: app_gpio_init()
//==============================================================================

  static inline int app_gpio_init(void)
  {
    return bl_init(mgpio,NULL);
  }

#endif // _APP_GPIO_H
