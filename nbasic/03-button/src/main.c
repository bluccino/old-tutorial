//==============================================================================
// 03-button.c
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//
// Created by Hugo Pristauz on 2022-Jan-09
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================
// Copyright (c) 2016 Open-RnD Sp. z o.o.
// Copyright (c) 2020 Nordic Semiconductor ASA
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

  #include <zephyr.h>
  #include <device.h>
  #include <drivers/gpio.h>
  #include <sys/util.h>
  #include <sys/printk.h>
  #include <inttypes.h>

  #define SLEEP_TIME_MS	1

//==============================================================================
// some shorthands
//==============================================================================

  #define GP_SPEC GPIO_DT_SPEC_GET_OR

  typedef struct gpio_dt_spec GP_io;             // GPIO device tree spec
  typedef struct gpio_callback GP_ctx;           // GPIO callback context
  typedef gpio_flags_t GP_flags;                 // GPIO flags
  typedef struct device GP_device;               // GPIO device
  typedef uint32_t GP_pins;                      // PINs qualifier in irs

  typedef void (*GP_irs)(const GP_device *dev, GP_ctx *ctx, GP_pins pins);

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

  #define SW0_NODE	DT_ALIAS(sw0)

    // very good: check status whether board supports the alias :-)))

  #if !DT_NODE_HAS_STATUS(SW0_NODE, okay)
    #error "Unsupported board: sw0 devicetree alias is not defined"
  #endif

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

  static const GP_io button = GP_SPEC(SW0_NODE, gpios,{0});
  static GP_ctx  butctx;                         // button context

//==============================================================================
// The led0 device tree alias is optional. If present, we'll use it
// to turn on the LED whenever the button is pressed.
//==============================================================================

  static GP_io led = GP_SPEC(DT_ALIAS(led0), gpios, {0});

//==============================================================================
// GPIO pin configuration (helper)
// - usage: gp_pin_cfg(io,flags)
// -        gp_pin_cfg(io,GPIO_INPUT)
//==============================================================================

  static int gp_pin_cfg(const GP_io *io, GP_flags flags)
  {
    int err = gpio_pin_configure_dt(io,flags);
    if (err)
      printk("Error %d: failed to configure %s pin %d\n",
  		       err, io->port->name, io->pin);

    return err;
  }

//==============================================================================
// GPIO pin interrupt configuration (helper)
// - usage: gp_int_cfg(io,flags)
// -        gp_int_cfg(io,GPIO_INT_EDGE_TO_ACTIVE)
// -        gp_int_cfg(io,GPIO_INT_EDGE_BOTH)
//==============================================================================

  static int gp_int_cfg(const GP_io *io, GP_flags flags)
  {
    int err = gpio_pin_interrupt_configure_dt(io,flags);
    if (err)
      printk("Error %d: failed to config %s pin %d interrupt\n",
  			     err, io->port->name, io->pin);

    return err;
  }

//==============================================================================
// GPIO pin add interrupt callback (helper)
// - usage: gp_add_cb(io,ctx,irs)
// -        gp_add_cb(&but,&but_ctx,but_irs)
//==============================================================================

  static void gp_add_cb(const GP_io *io, GP_ctx *ctx, GP_irs cb)
  {
    gpio_init_callback(ctx, cb, BIT(io->pin));
    gpio_add_callback(io->port, ctx);
  }

//==============================================================================
// provide button IRS callback (button pressed)
//==============================================================================

  static void irs_button(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    printk("Button pressed at %" PRIu32 "\n", k_cycle_get_32());
  }

//==============================================================================
// configure button
// - check whether device is ready
// - configure GPIO
//==============================================================================

  static void config_button(void)
  {
    if (!device_is_ready(button.port))
    {
      printk("Error: button device %s is not ready\n", button.port->name);
      return;
    }

    gp_pin_cfg(&button, GPIO_INPUT);
    gp_int_cfg(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gp_add_cb(&button, &butctx, irs_button);

    //gpio_init_callback(&butctx, irs_button, BIT(button.pin));
    //gpio_add_callback(button.port, &butctx);

    printk("set up button at %s pin %d\n", button.port->name, button.pin);
  }

//==============================================================================
// configure LED
// - check whether device is ready
// - configure GPIO
//==============================================================================

  static void config_led(void)
  {
    if (led.port && !device_is_ready(led.port))
    {
      printk("Error: LED device %s is not ready; ignoring it\n",
  		       led.port->name);
      led.port = NULL;
    }

    if (led.port)
    {
      int err = gpio_pin_configure_dt(&led, GPIO_OUTPUT);
      if (err != 0)
      {
  	    printk("Error %d: failed to configure LED device %s pin %d\n",
  	       err, led.port->name, led.pin);
  	    led.port = NULL;
      }
      else
  	    printk("Set up LED at %s pin %d\n", led.port->name, led.pin);
    }
  }

//==============================================================================
// main program
//==============================================================================

  void main(void)
  {
    config_button();
    config_led();

    printk("Press the button\n");

    if (led.port)
    {
      while (1)
      {
  	    // if we have an LED, match its state to the button's

        int val = gpio_pin_get_dt(&button);

  	    if (val >= 0)
        {
  	      gpio_pin_set_dt(&led, val);
  	    }

        k_msleep(SLEEP_TIME_MS);
      }
    }
  }
