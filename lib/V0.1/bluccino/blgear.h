//==============================================================================
// blgear.h - core interfaces
//==============================================================================
//
//  Interfaces: [] = SYS(INIT,LOOP)
//
//  Input Messages:
//    - [SYS:INIT]     Bluccino API layer initializes core
//    - [SYS:LOOP]     Bluccino API layer calls loop function of core
//
//                              [:INIT]  [:LOOP]
//                                  |       |
//                                  v       v
//                               +-------------+
//                               |             |
//                               |     SYS     |
//                               |             |
//                               +-------------+
//
//==============================================================================

#ifndef __BLGEAR_H__
#define __BLGEAR_H__

  int bl_gear(BL_ob *o, int val);      // gear system interface
  int bl_up(BL_ob *o, int val);        // gear upward interface
  int bl_down(BL_ob *o, int val);      // gear downward interface

#endif // __BLGEAR_H__
