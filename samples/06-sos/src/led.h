//==============================================================================
// led.h - an LED dummy block
//==============================================================================
//
//  Interfaces: [] = LED(LEVEL)
//
//  Input Messages:
//    - [LED:LEVEL]     app switches LED on or off
//
//                                  [:LEVEL]
//                                      |
//                                      v
//                               +-------------+
//                               |             |
//                               |     LED     |
//                               |             |
//                               +-------------+
//
//==============================================================================

#ifndef __LED_H__
#define __LED_H__

  int  led_in(BL_ob *o, int value);    // input interface
  void led_init(BL_fct call);       // init fuction
  void led_loop(void);                 // loop function

#endif // __LED_H__
