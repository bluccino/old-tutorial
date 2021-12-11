//==============================================================================
// timer.h - a simple timer module
//==============================================================================
//
//  Interfaces: [TICK] = TIMER(TRIG)
//
//  Input Messages:
//    - [TIMER:TRIG]    main loop driving timer modules with trigger messages
//
//  Output Messages:
//    - [TIMER:TICK]    timer module outputs tick messages
//
//                               [:TRIG] [:TICK]
//                                  |      ^
//                                  v      |
//                               +-------------+
//                               |             |
//                               |    TIMER    |
//                               |             |
//                               +-------------+
//
//==============================================================================

#ifndef __TIMER_H__
#define __TIMER_H__

  int  timer_in(BL_ob *o, int value);  // input interface
  void timer_init(BL_notify call);     // init fuction
  void timer_loop(void);               // loop function

#endif // __TIMER_H__
