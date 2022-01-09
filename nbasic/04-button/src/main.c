//==============================================================================
// 04-button.c
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//
// Created by Hugo Pristauz on 2022-Jan-09
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================
//
// - led0 device tree alias is optional 
// - if present, we'll use it to turn on the LED whenever button is pressed
//
//==============================================================================

  #define CFG_LOG_GPIO    1           // GPIO logging by default
  #define LOG             LOG_GPIO    // short hand

  #include "bluccino.h"
  #include "blgpio.h"                 // stuff to init and access Zephyr GPIO

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

  #define SW0      DT_ALIAS(sw0)
  #define LED0     DT_ALIAS(led0)

    // very good: check status whether board supports the alias :-)))
    // switch sw0 alias must be available, but we tolerate missing led0

  #if !DT_NODE_HAS_STATUS(SW0, okay)
    #error "Unsupported board: sw0 devicetree alias is not defined"
  #endif

  static BL_pin button = BL_PIN(SW0);
  static BL_pin led = BL_PIN(LED0);

//==============================================================================
// provide button IRS callback (button pressed)
//==============================================================================

  static void irs_button(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    LOG(1,BL_M"button pressed");
  }

//==============================================================================
// main program
//==============================================================================

  void main(void)
  {
    bl_pin_cfg(&button, GPIO_INPUT | GPIO_INT_DEBOUNCE);
    bl_pin_attach(&button, GPIO_INT_EDGE_TO_ACTIVE, irs_button);
    bl_pin_cfg(&led, GPIO_OUTPUT);

    LOG(1,"press button");

    if (led.io.port)    // if LED is supported
    {
      for (;;)          // if we have an LED, match its state to the button's
      {
        int val = bl_pin_get(&button); // negative value means invalid

        if (val >= 0)
          bl_pin_set(&led, val);

        bl_sleep(1);                   // sleep 1 ms
      }
    }
  }
