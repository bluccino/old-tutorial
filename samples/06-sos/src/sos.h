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

  int  sos(BL_ob *o, int value);       // THE sos interface function

#endif // __SOS_H__
