//==============================================================================
// blcore.h - core interfaces
//==============================================================================
//
//  Interfaces: [] = SYS(INIT,LOOP)
//
//  Input Messages:
//    - [SYS:INIT]      Bluccino API layer initializes core
//    - [SYS:LOOP]      Bluccino API layer calls loop function of core
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
//  Interfaces: [] = GOOSRV(SET)
//
//  Input Messages:
//    - [GOOSRV:SET]    App changes GOOSRV state
//
//                                   [:SET]
//                                      |
//                                      v
//                               +-------------+
//                               |             |
//                               |   GOOSRV    |
//                               |             |
//                               +-------------+
//
//==============================================================================

#ifndef __BLCORE_H__
#define __BLCORE_H__

  int bl_core(BL_ob *o, int val);      // THE core interface

#endif // __BLCORE_H__
