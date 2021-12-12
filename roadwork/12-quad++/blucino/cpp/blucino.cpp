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
//
//                                Blob Class
//
//==============================================================================

//==============================================================================
// init and loop operation
//==============================================================================
/*
  void Blob::init(void *data)
  {
    bl_log(2,"@blu.init",0,id);
    if (tag == BL_SYS)                 // only system init supported
       bl_init((BL_fct)data,id);    // init bluccino kernel
  }
*/
  void Blob::loop()                    // kernel & core loop
  {
    assert(tag == BL_SYS);             // only system init supported
    bl_loop();                         // drive kernel and core loops
  }

//==============================================================================
// logging
//==============================================================================

  void System::log(const char *msg, int Id, int value)
  {
    bl_log2(verbose,msg,Id,value);
  }

  void System::log(const char *msg, int value)
  {
    bl_log1(verbose,msg,value);
  }

  void System::log(const char *msg)
  {
    bl_log(verbose,msg);
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
    bl_log2(2,"@blu.send",id,value);

    BL_ob o = {tag,BL_SET,id,NULL};
    bl_set(&o,value);
  }

  void Blob::let(int value)
  {
    bl_log2(2,"@blu.send",id,value);

    BL_ob o = {tag,BL_LET,id,NULL};
    bl_set(&o,value);
  }

//==============================================================================
//
//                    Bluccino System Class
//
//==============================================================================

  void System::init(int verbose)            // init verbose (debug) level
  {
    id = verbose;                           // store verbose level in id
    log("verbose",verbose);
  }

//==============================================================================
//
//                    Bluccino Button Class
//
//==============================================================================

  static WhenButton when_button = 0;

  Button::Button(int id, WhenButton cb) : Blob(BL_SYS,id)
  {
    when_button = cb;
  }

//==============================================================================
//
//                      Bluccino Global Functionality
//
//==============================================================================

//==============================================================================
// event notification
//==============================================================================

  static void notify(BL_ob *o, int value)
  {
    bl_log2(2,"@notify",o->id,value);

    switch (o->tag)
    {
      case BL_BUTTON:
        if (when_button)
        {
          Button obj(o->id,o->op);
          when_button(obj,value);
          return;
        }
      default:
        return;
    }
  }

//==============================================================================
// Bluccino Init() and Loop functions
//==============================================================================

  void Init(int verbose)                    // Blucion init function
  {
    bl_log1(1,"@blu.init",verbose);
    bl_init(notify,verbose);                // init bluccino kernel
  }

  void Loop()                               // Bluccino loop function
  {
    // nothing to do ...
  }
