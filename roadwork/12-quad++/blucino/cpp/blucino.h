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

  class Blob;                                         // forward declaration
  class Button;                                       //forward declaration

  typedef void (*WhenButton)(Button &o, int value);   // event notify callback

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
      void loop();
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
      Button(int id, int op): Blob(BL_SYS,id,op) {}
      void when_set(int value);             // 'button set' event handler
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
// Bluccino Init() and Loop functions
//==============================================================================

  void Init(int verbose=1);            // Blucion init function
  void Loop();                         // Bluccino loop function

#endif // __BLUCCINO_H__
