//==============================================================================
//  blweak.h
//  weak function definitions for Bluccino
//
//  Created by Hugo Pristauz on 2021-12-11
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLWEAK_H__
#define __BLWEAK_H__

  #ifndef __WEAK__
    #define __WEAK__ 1
  #endif

  #if __WEAK__
      __weak int  bl_core(BL_ob *o, int val) { return 0; }      // dummy
      __weak void bl_verbose(int verbose)    {}                 // dummy
      __weak void bl_logo(int l,BL_txt m,BL_ob *o,int v) {}     // dummy
  #endif

  __weak int  bl_up(BL_ob *o, int val) { return 0; }            // dummy
  __weak int  bl_down(BL_ob *o, int val) { return 0; }          // dummy
  __weak int  bl_gear(BL_ob *o, int val) { return 0; }          // dummy

#endif // __BLWEAK_H__
