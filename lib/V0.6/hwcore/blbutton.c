//==============================================================================
// blbutton.c
// Bluccino HW core supporting basic functions for button & LED
//
// Created by Hugo Pristauz on 2022-Feb-18
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================

  #include "bluccino.h"
  #include "blbutton.h"
  #include "blgpio.h"

//==============================================================================
// CORE level logging shorthands
//==============================================================================

  #define LOG                     LOG_CORE
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"button:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)
  #define ERR 1,BL_R

//==============================================================================
// defines
//==============================================================================

//#define NBUTS  CFG_NUMBER_OF_BUTS

  #define N             4              // number of buttons

  #define SW0_NODE      DT_ALIAS(sw0)
  #define SW1_NODE      DT_ALIAS(sw1)
  #define SW2_NODE      DT_ALIAS(sw2)
  #define SW3_NODE      DT_ALIAS(sw3)

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

  static int id = 0;                   // button ID
  static GP_ctx context[N];            // button context

  static const GP_io button[N] =
               {
                 GP_IO(SW0_NODE, gpios,{0}),
                 GP_IO(SW1_NODE, gpios,{0}),
                 GP_IO(SW2_NODE, gpios,{0}),
                 GP_IO(SW3_NODE, gpios,{0}),
               };

//==============================================================================
// button work horse - posts [BUTTON:PRESS @id 1] or [BUTTON:RELEASE @id 0]
// - IRS routine sets id (button ID) and submits (button) work, which invokes
// - workhorse to process [BUTTON:PRESS @id 1] or [BUTTON:RELEASE @id,0]
//==============================================================================

  void workhorse(struct k_work *work)
  {
    if (id < 1 || id > N)
      return;                          // ignore out of range ID values

    int idx = id-1;
    int val = gp_pin_get(button+idx);  // read I/O pin input value
    BL_ob oo = {_BUTTON, val?PRESS_:RELEASE_, id,NULL};

    LOGO(4,BL_Y,&oo,val);
    bl_button(&oo,val);                // post to module interface for output
  }

  K_WORK_DEFINE(work, workhorse);      // assign work with workhorse

//==============================================================================
// submit button work for button @id
//==============================================================================

  static void submit(int bid)
  {
    id = bid;                          // store global button ID
    k_work_submit(&work);              // invoke workhorse(), which picks id
  }

//==============================================================================
// provide button IRS callback (button pressed/released)
//==============================================================================

  static void irs1(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    submit(1);
  }

  static void irs2(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    submit(2);
  }

  static void irs3(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    submit(3);
  }

  static void irs4(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    submit(4);
  }

  static GP_irs irs[N] = {irs1,irs2,irs3,irs4};

//==============================================================================
// configure button
// - check whether device is ready
// - configure GPIO
//==============================================================================

  static void config(int id)
  {
    int idx = id - 1;                  // button index (0..3)
    if (idx < 0 || idx > 3)
       return;                         // bad arg

    if (!device_is_ready(button[idx].port))
    {
      LOG(1,BL_R"error: button device %s not ready\n", button[idx].port->name);
      return;
    }

    gp_pin_cfg(&button[idx], GPIO_INPUT | GPIO_INT_DEBOUNCE);
    gp_int_cfg(&button[idx], GPIO_INT_EDGE_BOTH);
    gp_add_cb(&button[idx], &context[idx], irs[idx]);

    //gpio_init_callback(&context, irs_button, BIT(button.pin));
    //gpio_add_callback(button.port, &context);

    LOG(4,"set up button[@%d] @ %s pin %d",
           id, button[idx].port->name, button[idx].pin);
  }

//==============================================================================
// init
//==============================================================================

  static int init(BL_ob *o, int val)
  {
    LOG(2,BL_B "button init");

  	k_work_init(&work, workhorse);

    for (int i=1; i <= N; i++)
      config(i);
    return 0;
  }

//==============================================================================
// public module interface
//==============================================================================

  int bl_button(BL_ob *o, int val)        // BUTTON core module interface
  {
    static BL_fct output = NULL;          // to store output callback

    switch (bl_id(o))
    {
      case BL_ID(_SYS,INIT_):             // [SYS:INIT <cb>]
        output = o->data;                 // store output callback
      	return init(o,val);               // delegate to init() worker

      case BL_ID(_BUTTON,PRESS_):         // [BUTTON:PRESS @id]
      case BL_ID(_BUTTON,RELEASE_):       // [BUTTON:RELEASE @id]
	      return bl_out(o,val,output);      // output message

      default:
	      return -1;                        // bad input
    }
  }
