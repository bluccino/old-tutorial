//==============================================================================
// 04-button.c
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//
// Created by Hugo Pristauz on 2022-Jan-09
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================

  #define CFG_LOG_GPIO    1           // GPIO logging by default

  #include "bluccino.h"
  #include "blgpio.h"

  #define LOG  LOG_GPIO

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

  #define SW0	DT_ALIAS(sw0)

    // very good: check status whether board supports the alias :-)))

  #if !DT_NODE_HAS_STATUS(SW0, okay)
    #error "Unsupported board: sw0 devicetree alias is not defined"
  #endif

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

//static const GP_io button = GP_IO(SW0, gpios,{0});
//  static GP_ctx  butctx;                         // button context

  typedef struct BL_pin
  {
    GP_io io;                          // input I/O pin
    GP_ctx ctx;                        // context
  } BL_pin;

  #define BL_PIN(alias)      {.io = GP_IO(alias, gpios,{0}) }

//BL_button mybut = {.io = GP_IO(SW0, gpios,{0}) };
  static BL_pin button = BL_PIN(SW0);

//static GP_io led = GP_IO(DT_ALIAS(led0), gpios, {0});
  static BL_pin led = BL_PIN(DT_ALIAS(led0));

//==============================================================================
// config I/O pin
// - usage: bl_pin_cfg(pin,flags)
// -        bl_ipin(&button, GPIO_INPUT)
//==============================================================================

  static inline void bl_pin_cfg(BL_pin *pin, GP_flags flags)
  {
    if (!device_is_ready(pin->io.port))
    {
      LOG_GPIO(1,BL_R"error: pin %s not ready", pin->io.port->name);
      return;
    }

    gp_pin_cfg(&pin->io, flags);
  }

//==============================================================================
// attach interrupt handler to I/O pin
// - usage: bl_pin_attach(pin,flags,cb)
// -        bl_pin_attach(&button, GPIO_INT_EDGE_TO_ACTIVE, but_cb)
// -        bl_pin_attach(&button, GPIO_INT_EDGE_BOTH, but_cb)
//==============================================================================

  static inline void bl_pin_attach(BL_pin *pin, GP_flags flags, GP_irs cb)
  {
    if (!device_is_ready(pin->io.port))
    {
      LOG_GPIO(1,BL_R"error: pin %s not ready", pin->io.port->name);
      return;
    }

    if (flags)
      gp_int_cfg(&pin->io, flags);

    if (cb)
      gp_add_cb(&pin->io, &pin->ctx, cb);
  }

//==============================================================================
// config I/O
// - usage: bl_pin(p,flags,iflags,cb)
// -        bl_pin(&button, GPIO_INPUT, GPIO_INT_EDGE_TO_ACTIVE, but_cb)
// -        bl_pin(&button, GPIO_INPUT, GPIO_INT_EDGE_BOTH, but_cb)
// -        bl_pin(&button, GPIO_INPUT, 0, NULL)  // no interrupt
//==============================================================================

  static inline void bl_pin(BL_pin *p,GP_flags flags,GP_flags iflags,GP_irs cb)
  {
    bl_pin_cfg(p, flags);

    if (iflags && cb)
      bl_pin_attach(p, iflags, cb);
  }

//==============================================================================
// The led0 device tree alias is optional. If present, we'll use it
// to turn on the LED whenever the button is pressed.
//==============================================================================


//==============================================================================
// provide button IRS callback (button pressed)
//==============================================================================

  static void irs_button(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    LOG(1,BL_M"button pressed");
  }

//==============================================================================
// configure button
// - check whether device is ready
// - configure GPIO
//==============================================================================
/*
  static inline void config_button(void)
  {
    if (!device_is_ready(button.port))
    {
      printk("Error: button device %s is not ready\n", button.port->name);
      return;
    }

    gp_pin_cfg(&button, GPIO_INPUT);
    gp_int_cfg(&button, GPIO_INT_EDGE_TO_ACTIVE);
    gp_add_cb(&button, &butctx, irs_button);

    printk("set up button at %s pin %d\n", button.port->name, button.pin);
  }
*/
//==============================================================================
// configure LED
// - check whether device is ready
// - configure GPIO
//==============================================================================
/*
  static void config_led(BL_pin *pin)
  {
    if (pin->io.port && !device_is_ready(pin->io.port))
    {
      printk("Error: LED device %s is not ready; ignoring it\n",
  		       pin->io.port->name);
      pin->io.port = NULL;
    }

    if (pin->io.port)
    {
      int err = gpio_pin_configure_dt(&pin->io, GPIO_OUTPUT);
      if (err != 0)
      {
  	    printk("Error %d: failed to configure LED device %s pin %d\n",
  	       err, pin->io.port->name, pin->io.pin);
  	    pin->io.port = NULL;
      }
      else
  	    printk("Set up LED at %s pin %d\n", pin->io.port->name, pin->io.pin);
    }
  }
*/
//==============================================================================
// The led0 device tree alias is optional. If present, we'll use it
// to turn on the LED whenever the button is pressed.
//==============================================================================

//  static GP_io led = GP_IO(DT_ALIAS(led0), gpios, {0});

//==============================================================================
// configure LED
// - check whether device is ready
// - configure GPIO
//==============================================================================

  static void bl_pin_cfgo(BL_pin *pin)
  {
    if (pin->io.port && !device_is_ready(pin->io.port))
    {
      LOG(1,BL_R"error: output pin %s not ready; ignore!", pin->io.port->name);
      pin->io.port = NULL;
    }

    if (pin->io.port)
    {
      int err = gpio_pin_configure_dt(&pin->io, GPIO_OUTPUT);
      if (err != 0)
      {
  	    LOG(1,BL_R"error %d: failed to config %s pin %d",
  	       err, pin->io.port->name, pin->io.pin);
  	    pin->io.port = NULL;
      }
      else
  	    LOG(1,BL_R"set pin %d @ port %s", pin->io.pin, pin->io.port->name);
    }
  }

//==============================================================================
// main program
//==============================================================================

  static inline int bl_pin_get(BL_pin *pin)
  {
    return gpio_pin_get_dt(&pin->io);
  }

  static inline int bl_pin_set(BL_pin *pin, int value)
  {
    return gpio_pin_set_dt(&pin->io,value);
  }

  void main(void)
  {
    bl_pin_cfg(&button, GPIO_INPUT);
    bl_pin_attach(&button, GPIO_INT_EDGE_TO_ACTIVE, irs_button);
    bl_pin_cfg(&led, GPIO_OUTPUT);

//    bl_pin_cfgo(&led);

    printk("Press the button\n");

    if (led.io.port)
    {
      for (;;)          // if we have an LED, match its state to the button's
      {
        int val = bl_pin_get(&button);

        if (val >= 0)
//        bl_pin_set(&led, val);
  	      gpio_pin_set_dt(&led.io, val);

        bl_sleep(1);                   // sleep 1 ms
      }
    }
  }
