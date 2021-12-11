//==============================================================================
//  bluccino.cpp
//  Bluccino class implementation
//
//  Created by Hugo Pristauz on 2021-11-21
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#include <assert.h>
#include "bluccino.h"

//==============================================================================
// init and loop operation
//==============================================================================

  void Blob::init(void *data)
  {
    bl_log(2,"@blu.init",0,id);
    assert(tag == BL_SYS);             // only system init supported
    bl_init((BL_notify)data,id);       // init bluccino kernel
  }

  void Blob::loop()                    // kernel & core loop
  {
    assert(tag == BL_SYS);             // only system init supported
    bl_loop();                         // drive kernel and core loops
  }

//==============================================================================
// logging
//==============================================================================

  void Blob::log(const char *msg, int Id, int value)
  {
    bl_log(id,msg,Id,value);
  }

  void Blob::log(const char *msg, int value)
  {
    bl_log(id,msg,0,value);
  }

//==============================================================================
// set and let operation
//==============================================================================

  void Blob::sleep(int ms)             // sleep some miliseconds
  {
    k_msleep(ms);                      // good night ...
  }

//==============================================================================
// set and let operation
//==============================================================================

  void Blob::set(int value)
  {
    bl_log(2,"@blu.send",id,value);

    BL_ob o = {tag,BL_SET,id,NULL};
    bl_set(&o,value);
  }

  void Blob::let(int value)
  {
    bl_log(2,"@blu.send",id,value);

    BL_ob o = {tag,BL_LET,id,NULL};
    bl_set(&o,value);
  }
