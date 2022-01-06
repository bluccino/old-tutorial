//==============================================================================
// app_gpio.c
// app GPIO handling
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
//                             +-------------+
//                             |             |
//                             |     LED     |
//                             |             |
//                             +-------------+
//                             |             |
//                             |   BUTTON    |
//                             |             |
//                             +-------------+
//  Input Messages:
//    - [:]
//
//  Output Messages:
//    - [:]
//    - [:]
//
//==============================================================================
// Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
// Copyright (c) 2018 Vikrant More
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

  #include <drivers/gpio.h>

  #include "bluccino.h"

  #include "app_gpio.h"
  #include "publisher.h"

//==============================================================================
// CORE level logging shorthands
//==============================================================================

  #define LOG                     LOG_CORE
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"mcore:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)
  #define ERR 1,BL_R

  static BL_fct notify = NULL;

//==============================================================================
// let's go ...
//==============================================================================

  const struct device *led_device[4];
  const struct device *button_device[4];

  K_WORK_DEFINE(button_work, publish);

  static void button_pressed(const struct device *dev,
  			   struct gpio_callback *cb, uint32_t pins)
  {
  	k_work_submit(&button_work);
  }

  #define LED0_NODE DT_ALIAS(led0)
  #define LED1_NODE DT_ALIAS(led1)
  #define LED2_NODE DT_ALIAS(led2)
  #define LED3_NODE DT_ALIAS(led3)

  #define SW0_NODE DT_ALIAS(sw0)
  #define SW1_NODE DT_ALIAS(sw1)
  #define SW2_NODE DT_ALIAS(sw2)
  #define SW3_NODE DT_ALIAS(sw3)

//==============================================================================
// LED init
//==============================================================================

#ifdef ONE_LED_ONE_BUTTON_BOARD
  #define N    1
#else
  #define N    4
#endif

  static void led_init(void)
  {
  	/* LEDs configuration & setting */

    LOG(3,BL_C "init %d LED%s",N, N==1?"":"s");

  	led_device[0] = device_get_binding(DT_GPIO_LABEL(LED0_NODE, gpios));
  	gpio_pin_configure(led_device[0], DT_GPIO_PIN(LED0_NODE, gpios),
  			   DT_GPIO_FLAGS(LED0_NODE, gpios) |
  			   GPIO_OUTPUT_INACTIVE);

    #ifndef ONE_LED_ONE_BUTTON_BOARD
    	led_device[1] = device_get_binding(DT_GPIO_LABEL(LED1_NODE, gpios));
    	gpio_pin_configure(led_device[1], DT_GPIO_PIN(LED1_NODE, gpios),
    			   DT_GPIO_FLAGS(LED1_NODE, gpios) |
    			   GPIO_OUTPUT_INACTIVE);

    	led_device[2] = device_get_binding(DT_GPIO_LABEL(LED2_NODE, gpios));
    	gpio_pin_configure(led_device[2], DT_GPIO_PIN(LED2_NODE, gpios),
    			   DT_GPIO_FLAGS(LED2_NODE, gpios) |
    			   GPIO_OUTPUT_INACTIVE);

    	led_device[3] = device_get_binding(DT_GPIO_LABEL(LED3_NODE, gpios));
    	gpio_pin_configure(led_device[3], DT_GPIO_PIN(LED3_NODE, gpios),
    			   DT_GPIO_FLAGS(LED3_NODE, gpios) |
    			   GPIO_OUTPUT_INACTIVE);
    #endif
  }

//==============================================================================
// button init
//==============================================================================

static void button_init(void)
{
	static struct gpio_callback button_cb[4];

  LOG(3,BL_C "init %d button%s",N, N==1?"":"s");

	  // Buttons configuration & setting

	k_work_init(&button_work, publish);

	button_device[0] = device_get_binding(DT_GPIO_LABEL(SW0_NODE, gpios));
	gpio_pin_configure(button_device[0], DT_GPIO_PIN(SW0_NODE, gpios),
			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
			   DT_GPIO_FLAGS(SW0_NODE, gpios));

	gpio_pin_interrupt_configure(button_device[0],
				     DT_GPIO_PIN(SW0_NODE, gpios),
				     GPIO_INT_EDGE_TO_ACTIVE);

	gpio_init_callback(&button_cb[0], button_pressed,
			   BIT(DT_GPIO_PIN(SW0_NODE, gpios)));

	gpio_add_callback(button_device[0], &button_cb[0]);

  #ifndef ONE_LED_ONE_BUTTON_BOARD
  	button_device[1] = device_get_binding(DT_GPIO_LABEL(SW1_NODE, gpios));

  	gpio_pin_configure(button_device[1], DT_GPIO_PIN(SW1_NODE, gpios),
  			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW1_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[1],
  				     DT_GPIO_PIN(SW1_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

  	gpio_init_callback(&button_cb[1], button_pressed,
  			   BIT(DT_GPIO_PIN(SW1_NODE, gpios)));

  	gpio_add_callback(button_device[1], &button_cb[1]);


  	button_device[2] = device_get_binding(DT_GPIO_LABEL(SW2_NODE, gpios));

  	gpio_pin_configure(button_device[2], DT_GPIO_PIN(SW2_NODE, gpios),
  	       GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW2_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[2],
  				     DT_GPIO_PIN(SW2_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

  	gpio_init_callback(&button_cb[2], button_pressed,
  			   BIT(DT_GPIO_PIN(SW2_NODE, gpios)));

  	gpio_add_callback(button_device[2], &button_cb[2]);

  	button_device[3] = device_get_binding(DT_GPIO_LABEL(SW3_NODE, gpios));

  	gpio_pin_configure(button_device[3], DT_GPIO_PIN(SW3_NODE, gpios),
  			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW3_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[3],
  				     DT_GPIO_PIN(SW3_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

  	gpio_init_callback(&button_cb[3], button_pressed,
  			   BIT(DT_GPIO_PIN(SW3_NODE, gpios)));

  	gpio_add_callback(button_device[3], &button_cb[3]);
  #endif
}

//==============================================================================
// init
//==============================================================================


  static int init(BL_ob *o, int val)
  {
    LOG(2,BL_B "GPIO init");

    notify = o->data;                   // store notify callback
    led_init();
    button_init();
    return 0;
  }

//==============================================================================
// public module interface
//==============================================================================

  int gpio(BL_ob *o, int val)
  {
    switch (BL_PAIR(o->cl,o->op))
    {
      case BL_PAIR(CL_SYS,OP_INIT):       // [SYS:INIT]
	      return init(o,val);               // delegate to init();

      default:
	      return -1;                        // bad input
    }
  }
