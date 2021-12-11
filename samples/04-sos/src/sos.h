//==============================================================================
// sos.h - an SOS sequencer block
//==============================================================================
//
//  Interfaces: [LEVEL] = SOS(TICK)
//
//  Input Messages:
//    - [SOS:LEVEL]     SOS receives a period timer tick
//
//  Output Messages:
//    - [SOS:LEVEL]     app switches LED on or off
//
//                               [:TICK] [:LEVEL]
//                                  |      ^
//                                  v      |
//                               +-------------+
//                               |             |
//                               |     SOS     |
//                               |             |
//                               +-------------+
//
//==============================================================================

#ifndef __SOS_H__
#define __SOS_H__

  int  sos_in(BL_ob *o, int value);    // input interface
  void sos_init(BL_notify call);       // init fuction
  void sos_loop(void);                 // loop function

#endif // __SOS_H__
