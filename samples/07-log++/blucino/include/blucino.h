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

//#ifdef __cplusplus
  extern "C" {
//#endif

  #include "blapi.h"
  #include "bllog.h"

//#ifdef __cplusplus
  }
//#endif

#ifndef BL_DEBUG
   #define BL_DEBUG 1                  // debug level
#endif

//==============================================================================
// Class Blob
//==============================================================================
//  #ifdef __cplusplus


  class Blob                           // Bluccino object class
  {
    public:
      int tag;
      int id;

    public:                            // constructors
      Blob(int Tag)         { tag = Tag; id = 0;  }
      Blob(int Tag, int Id) { tag = Tag; id = Id; }

    public:                            // operations
      void init(void *data);
      void loop();
      void set(int value);
      void let(int value);
      void sleep(int ms);              // sleep some miliseconds
      void log(const char *msg, int id, int value);
      void log(const char *msg, int value);
  };

  class System : public Blob
  {
    public:
      System(int Id) : Blob(BL_SYS,Id) {}
  };

  class GooCli : public Blob
  {
    public:
      GooCli(int Id) : Blob(BL_GOOCLI,Id) {}
  };

//  #endif
#endif // __BLUCCINO_H__
