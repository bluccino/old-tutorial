//==============================================================================
//  bluccino.h
//  Bluccino overall header
//
//  Created by Hugo Pristauz on 2021-11-06
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLUCCINO_H__
#define __BLUCCINO_H__

#include <stdint.h>
#include <stdbool.h>
#include <zephyr.h>

extern "C"
{
  #include "blapi.h"
  #include "bllog.h"
}

//==============================================================================
// Type definitions
//==============================================================================

  class Blob;                          // Bluccino object (base class)
  class Button;                        // Button access object
  class Led;                           // LED access object
  class GooCli;                        // Generic OnOff Client
  class GooSrv;                        // Generic OnOff Server

      // event notification callbacks

  typedef void (*WhenButton)(Button &o, int value);
  typedef void (*WhenLed)(Led &o, int value);
  typedef void (*WhenGooSrv)(GooSrv &o, int value);
  typedef void (*WhenGooCli)(GooCli &o, int value);

//==============================================================================
// Class Blob (Bluccino Object)
//==============================================================================

  class Blob                                // Bluccino object class
  {
    public:
      int tag;
      int op;                               // op code
      int id;

    public:                                 // constructors
      Blob(int Tag)         { tag = Tag; id = 0;  op = 0; }
      Blob(int Tag, int id_=0, int op_=0) { tag = Tag; id = id_; op = op_; }

    public:                                 // operations
//    void loop();
      void set(int value);
      void let(int value);
      void sleep(int ms);                   // sleep some miliseconds
  };

//==============================================================================
// Class System (Bluccino System Object)
//==============================================================================

  class System : public Blob
  {
    public:
      int verbose;                          // verbose (debug) level
    public:
      System(int debug) : Blob(BL_SYS,0) { verbose = debug; }
      void init(int debug);                 // init verbose (debug) level

    public:                                  // logging methods
      void log(const char *msg, int id, int value);
      void log(const char *msg, int value);
      void log(const char *msg);
  };

//==============================================================================
// Class Button (Bluccino Button Object)
//==============================================================================

  class Button : public Blob
  {
    public:
      Button(int id, WhenButton cb=0);
      Button(int id, int op): Blob(BL_BUTTON,id,op) {}
      void set(int value);
//    void when_set(int value);             // 'button set' event handler
  };

//==============================================================================
// Class Led (Bluccino Led Object)
//==============================================================================

  class Led : public Blob
  {
    public:
      Led(int id, WhenLed cb=0);
      Led(int id, int op): Blob(BL_LED,id,op) {}
      void set(int value);
//    void when_set(int value);             // 'button set' event handler
  };

//==============================================================================
// Class GooCli (Generic OnOff Client Object)
//==============================================================================

  class GooCli : public Blob
  {
    public:
      GooCli(int Id) : Blob(BL_GOOCLI,Id) {}
  };

//==============================================================================
// Class GooSrv (Generic OnOff Server Object)
//==============================================================================

  class GooSrv : public Blob
  {
    public:
      GooSrv(int id, WhenGooSrv cb=0);
      GooSrv(int id, int op): Blob(BL_GOOSRV,id,op) {}
//    void when_set(int value);        // 'button set' event handler
  };

//==============================================================================
// Bluccino Init() and Loop functions
//==============================================================================

  void Init(int verbose=1);            // Blucion init function
  void Loop();                         // Bluccino loop function

#endif // __BLUCCINO_H__
