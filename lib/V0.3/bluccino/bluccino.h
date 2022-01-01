//==============================================================================
//  bluccino.h
//  Bluccino overall header
//
//  Created by Hugo Pristauz on 2021-11-06
//  Copyright © 2021 Bluenetics GmbH. All rights reserved.
//==============================================================================

#ifndef __BLUCCINO_H__
#define __BLUCCINO_H__

  #ifdef __cplusplus
    #include "blclass.h"               // include Bluccino C++ class definitions
  #else

    #include "bltype.h"

    #ifdef BL_CONFIG
      #include "config.h"
    #endif

    #include "blmsg.h"

    #ifdef BL_LOGGING
      #include "logging.h"
    #endif

    #include "blapi.h"
    #include "bllog.h"

  #endif

#endif // __BLUCCINO_H__
