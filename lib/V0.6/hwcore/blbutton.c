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
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"blbutton:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)
  #define ERR 1,BL_R

//==============================================================================
// defines
//==============================================================================

  #define NBUTS  CFG_NUMBER_OF_BUTS

  #define SW0_NODE DT_ALIAS(sw0)
  #define SW1_NODE DT_ALIAS(sw1)
  #define SW2_NODE DT_ALIAS(sw2)
  #define SW3_NODE DT_ALIAS(sw3)

//==============================================================================
// Get button configuration from the devicetree sw0 alias. This is mandatory.
//==============================================================================

//#define BL_PIN(alias)      {.io = GP_IO(alias, gpios,{0}) }

  static const GP_io button[4] =
               {
                 GP_IO(SW0_NODE, gpios,{0}),
                 GP_IO(SW1_NODE, gpios,{1}),
                 GP_IO(SW2_NODE, gpios,{2}),
                 GP_IO(SW3_NODE, gpios,{3}),
               };

  static GP_ctx context[4];              // button context
  static int id = 0;                  // button ID

//==============================================================================
// button work horse - posts [BUTTON:PRESS @id 1] or [BUTTON:RELEASE @id 0]
// - IRS routine sets id (button ID) and submits (button) work, which invokes
// - workhorse to process [BUTTON:PRESS @id 1] or [BUTTON:RELEASE @id,0]
//==============================================================================

  void workhorse(struct k_work *work)
  {
    if (id < 1 || id > 4)
      return;                          // ignore out of range ID values

    int val = 1;
    BL_ob oo = {_BUTTON,val?PRESS_:RELEASE_,id,NULL};

    LOGO(1,BL_R,&oo,val);
    bl_button(&oo,val);                // post to module interface for output
  }

  K_WORK_DEFINE(work, workhorse);      // assign work with workhorse

//==============================================================================
// provide button IRS callback (button pressed)
//==============================================================================

  static void irs_button(const GP_device *dev, GP_ctx *ctx, GP_pins pins)
  {
    int id1 = 1 * ((pins & 0x02000) != 0);
    int id2 = 2 * ((pins & 0x04000) != 0);
    int id3 = 3 * ((pins & 0x08000) != 0);
    int id4 = 4 * ((pins & 0x10000) != 0);

    id = id1 + id2 + id3 + id4;
    LOG(4,BL_Y "Button @%d pressed (pins:%05X)", id,pins);

  	k_work_submit(&work);
  }

//==============================================================================
// configure button
// - check whether device is ready
// - configure GPIO
//==============================================================================

  static void config(int id)
  {
    if (id < 0 || id > 3)
       return;                         // bad arg

    if (!device_is_ready(button[id].port))
    {
      LOG(1,BL_R"error: button device %s not ready\n", button[0].port->name);
      return;
    }

    gp_pin_cfg(&button[id], GPIO_INPUT | GPIO_INT_DEBOUNCE);
    gp_int_cfg(&button[id], GPIO_INT_EDGE_BOTH);
    gp_add_cb(&button[id], &context[id], irs_button);

    //gpio_init_callback(&context, irs_button, BIT(button.pin));
    //gpio_add_callback(button.port, &context);

    LOG(4,"set up button[@%d] @ %s pin %d",
           id, button[id].port->name, button[id].pin);
  }

//==============================================================================
// init
//==============================================================================

  static int init(BL_ob *o, int val)
  {
    LOG(2,BL_B "button init");

  	k_work_init(&work, workhorse);

    for (int i=0; i < 4; i++)
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
