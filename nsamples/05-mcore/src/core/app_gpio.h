/* Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
 *
 * Copyright (c) 2018 Vikrant More
 *
 * SPDX-License-Identifier: Apache-2.0
 */

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

  int gpio(BL_ob *o, int val);         // public module interface

//==============================================================================
// syntactic sugar: app GPIO init /init GPIO driver)
// - usage: app_gpio_init()
//==============================================================================

  static inline int app_gpio_init(void)
  {
    return bl_init(gpio,NULL);
  }

#endif // _APP_GPIO_H
