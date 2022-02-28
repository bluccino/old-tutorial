//==============================================================================
// app_gpio.c
// app GPIO handling
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
//                                  GPIO
//                             +-------------+
//                             |             |
//                       SET ->|    LED      |
//                             |             |
//                             +-------------+
//                             |             |
//                             |   BUTTON    |-> PRESS
//                             |             |
//                             +-------------+
//  Input Messages:
//    - [LED:SET @id onoff]   // set LED @id on/off (id=1..4)
//
//  Output Messages:
//    - [BUTTON:PRESS @id]      // notify button press
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
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"gpio:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)
  #define ERR 1,BL_R

  static BL_fct notify = NULL;

//==============================================================================
// defines
//==============================================================================

  #ifdef ONE_LED_ONE_BUTTON_BOARD
    #define N    1
  #else
    #define N    4
  #endif

  #define LED0_NODE DT_ALIAS(led0)
  #define LED1_NODE DT_ALIAS(led1)
  #define LED2_NODE DT_ALIAS(led2)
  #define LED3_NODE DT_ALIAS(led3)

  #define SW0_NODE DT_ALIAS(sw0)
  #define SW1_NODE DT_ALIAS(sw1)
  #define SW2_NODE DT_ALIAS(sw2)
  #define SW3_NODE DT_ALIAS(sw3)

//==============================================================================
// locals
//==============================================================================

  static bool led_onoff[4] = {0,0,0,0};
  static const struct device *led_device[4];

//==============================================================================
// button work horse - posts [BUTTON:SET @id 1] (instead of publishing)
// - button_press_worker()                // our button work horse
// - K_WORK_DEFINE(button_work, publish); // assign bitton work with work horse
//==============================================================================

  void button_workhorse(struct k_work *work)
  {
    BL_ob oo = {_BUTTON,SET_,0,NULL};

    LOGO(1,BL_R,&oo,1);    //
    bl_core(&oo,1);        // post to core module interface in order to output
  }

//==============================================================================
// button press callback
//==============================================================================

  const struct device *button_device[4];
	static struct gpio_callback button_cb[4];

  #if MIGRATION_STEP4
    K_WORK_DEFINE(button_work, button_workhorse);    // new action
  #else
    K_WORK_DEFINE(button_work, publish);             // old action
  #endif

  typedef const struct device SH_dev;            // short hand
  typedef struct gpio_callback SH_gpiocb;           // short hand

  #if MIGRATION_STEP4
    static void button_post(int id)
    {
      BL_ob oo = {_BUTTON,PRESS_,id,NULL};
      LOGO(4,BL_Y,&oo,1);
      bl_core(&oo,1);             // post [BUTTON:PRESS @id,1] to core
    }

    static void but0_cb(SH_dev *dev, SH_gpiocb *cb, uint32_t pins)
    {
      button_post(1);
    }

    static void but1_cb(SH_dev *dev, SH_gpiocb *cb, uint32_t pins)
    {
      button_post(2);
    }

    static void but2_cb(SH_dev *dev, SH_gpiocb *cb, uint32_t pins)
    {
      button_post(3);
    }

    static void but3_cb(SH_dev *dev, SH_gpiocb *cb, uint32_t pins)
    {
      button_post(4);
    }
  #else
    static void button_pressed(const struct device *dev,
    			   struct gpio_callback *cb, uint32_t pins)
    {
      #if MIGRATION_STEP4
        LOG(4,BL_Y "button @%d pressed",1*match0+2*match1+3*match2+4*match3);
      #endif
    	k_work_submit(&button_work);     // continue at button_workhorse()
    }
  #endif

//==============================================================================
// LED set  [SET:LED @id onoff]  // @id = 1..4
//==============================================================================

  static int led_set(BL_ob *o, int onoff)
  {
    switch (o->id)     // id 1..4 maps to led_device[0..3]
    {
      case 1:
        led_onoff[0] = onoff;
        gpio_pin_set(led_device[0], DT_GPIO_PIN(DT_ALIAS(led0), gpios),onoff);
        return 0;

      case 2:
        led_onoff[1] = onoff;
        gpio_pin_set(led_device[1], DT_GPIO_PIN(DT_ALIAS(led1), gpios),onoff);
        return 0;

      case 3:
        led_onoff[2] = onoff;
        gpio_pin_set(led_device[2], DT_GPIO_PIN(DT_ALIAS(led2), gpios),onoff);
        return 0;

      case 4:
        led_onoff[3] = onoff;
        gpio_pin_set(led_device[3], DT_GPIO_PIN(DT_ALIAS(led3), gpios),onoff);
        return 0;

      default:
        return -1;                     // bad input
    }
  }

//==============================================================================
// LED toggle
//==============================================================================

  static int led_toggle(BL_ob *o,int val)
  {
    if (o->id < 1 || o->id > 4)
      return -1;                       // bad args

    val = (led_onoff[o->id-1] == 0);   // new LED value
    int ok = led_set(o,val);           // toggle LED state

    LOGO(4,BL_Y,o,val);                // log changed LED level
    return ok;
  }

//==============================================================================
// LED init
//==============================================================================

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
  LOG(3,BL_C "init %d button%s",N, N==1?"":"s");

	  // Buttons configuration & setting

  #if MIGRATION_STEP4
  	k_work_init(&button_work, button_workhorse);
  #else
  	k_work_init(&button_work, publish);
  #endif

      // button[0] ...

	button_device[0] = device_get_binding(DT_GPIO_LABEL(SW0_NODE, gpios));
	gpio_pin_configure(button_device[0], DT_GPIO_PIN(SW0_NODE, gpios),
			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
			   DT_GPIO_FLAGS(SW0_NODE, gpios));

	gpio_pin_interrupt_configure(button_device[0],
				     DT_GPIO_PIN(SW0_NODE, gpios),
				     GPIO_INT_EDGE_TO_ACTIVE);

  #if MIGRATION_STEP4
  	gpio_init_callback(&button_cb[0], but0_cb, BIT(DT_GPIO_PIN(SW0_NODE, gpios)));
  #else
  	gpio_init_callback(&button_cb[0], button_pressed,
  			   BIT(DT_GPIO_PIN(SW0_NODE, gpios)));
  #endif

	gpio_add_callback(button_device[0], &button_cb[0]);

  #ifndef ONE_LED_ONE_BUTTON_BOARD

      // button[1] ...

  	button_device[1] = device_get_binding(DT_GPIO_LABEL(SW1_NODE, gpios));

  	gpio_pin_configure(button_device[1], DT_GPIO_PIN(SW1_NODE, gpios),
  			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW1_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[1],
  				     DT_GPIO_PIN(SW1_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

    #if MIGRATION_STEP4
  	  gpio_init_callback(&button_cb[1], but1_cb, BIT(DT_GPIO_PIN(SW1_NODE, gpios)));
    #else
  	  gpio_init_callback(&button_cb[1], button_pressed,
  			   BIT(DT_GPIO_PIN(SW1_NODE, gpios)));
    #endif

  	gpio_add_callback(button_device[1], &button_cb[1]);

      // button[2] ...

  	button_device[2] = device_get_binding(DT_GPIO_LABEL(SW2_NODE, gpios));

  	gpio_pin_configure(button_device[2], DT_GPIO_PIN(SW2_NODE, gpios),
  	       GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW2_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[2],
  				     DT_GPIO_PIN(SW2_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

    #if MIGRATION_STEP4
  	  gpio_init_callback(&button_cb[2], but2_cb, BIT(DT_GPIO_PIN(SW2_NODE, gpios)));
    #else
    	gpio_init_callback(&button_cb[2], button_pressed,
  			   BIT(DT_GPIO_PIN(SW2_NODE, gpios)));
    #endif

  	gpio_add_callback(button_device[2], &button_cb[2]);

      // button[3] ...

  	button_device[3] = device_get_binding(DT_GPIO_LABEL(SW3_NODE, gpios));

  	gpio_pin_configure(button_device[3], DT_GPIO_PIN(SW3_NODE, gpios),
  			   GPIO_INPUT | GPIO_INT_DEBOUNCE |
  			   DT_GPIO_FLAGS(SW3_NODE, gpios));

  	gpio_pin_interrupt_configure(button_device[3],
  				     DT_GPIO_PIN(SW3_NODE, gpios),
  				     GPIO_INT_EDGE_TO_ACTIVE);

    #if MIGRATION_STEP4
  	  gpio_init_callback(&button_cb[3], but3_cb, BIT(DT_GPIO_PIN(SW3_NODE, gpios)));
    #else
  	  gpio_init_callback(&button_cb[3], button_pressed,
  			   BIT(DT_GPIO_PIN(SW3_NODE, gpios)));
    #endif

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
    switch (BL_ID(o->cl,o->op))
    {
      case BL_ID(_SYS,INIT_):       // [SYS:INIT]
      	return init(o,val);               // delegate to init();

      case BL_ID(_LED,SET_):        // [LED:set]
	      return led_set(o,val != 0);       // delegate to led_set();

      case BL_ID(_LED,TOGGLE_):     // [LED:toggle]
	      return led_toggle(o,val);         // delegate to led_toggle();

      default:
	      return -1;                        // bad input
    }
  }
