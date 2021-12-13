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

  int  led(BL_ob *o, int value);       // THE led interface function

#endif // __LED_H__
