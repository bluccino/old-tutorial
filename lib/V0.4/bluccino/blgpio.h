//==============================================================================
// blgpio.h
// shorthannd typedefs and helpers for GPIO setup
//
// Created by Hugo Pristauz on 2022-Jan-09
// Copyright © 2022 Bluenetics. All rights reserved.
//==============================================================================

#ifndef __BLGPIO_H__
#define __BLGPIO_H__

  #include <zephyr.h>
  #include <device.h>
  #include <drivers/gpio.h>
  #include <sys/util.h>
  #include <sys/printk.h>
  #include <inttypes.h>

//==============================================================================
// some shorthands
//==============================================================================

  #define GP_IO                GPIO_DT_SPEC_GET_OR

  typedef uint32_t             GP_pins;          // PINs qualifier in irs
  typedef gpio_flags_t         GP_flags;         // GPIO flags

  typedef struct device        GP_device;        // GPIO device
  typedef struct gpio_dt_spec  GP_io;            // GPIO device tree spec
  typedef struct gpio_callback GP_ctx;           // GPIO callback context

  typedef void (*GP_irs)(const GP_device *dev, GP_ctx *ctx, GP_pins pins);

//==============================================================================
// GPIO pin configuration (helper)
// - usage: gp_pin_cfg(io,flags)
// -        gp_pin_cfg(io,GPIO_INPUT)
//==============================================================================

  static inline int gp_pin_cfg(const GP_io *io, GP_flags flags)
  {
    int err = gpio_pin_configure_dt(io,flags);
    if (err)
      printk("Error %d: failed to configure %s @ pin %d\n",
  		       err, io->port->name, io->pin);

    return err;
  }

//==============================================================================
// GPIO pin interrupt configuration (helper)
// - usage: gp_int_cfg(io,flags)
// -        gp_int_cfg(io,GPIO_INT_EDGE_TO_ACTIVE)
// -        gp_int_cfg(io,GPIO_INT_EDGE_BOTH)
//==============================================================================

  static inline int gp_int_cfg(const GP_io *io, GP_flags flags)
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

  static inline void gp_add_cb(const GP_io *io, GP_ctx *ctx, GP_irs cb)
  {
    gpio_init_callback(ctx, cb, BIT(io->pin));
    gpio_add_callback(io->port, ctx);
  }



/*
__syscall int gpio_pin_configure(const struct device *port,
				 gpio_pin_t pin,
				 gpio_flags_t flags);
*/

#endif // __BLGPIO_H__
