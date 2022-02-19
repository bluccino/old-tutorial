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

    #include "bl_type.h"

    #ifdef BL_CONFIG
      #include "config.h"
    #endif

    #include "bl_msg.h"

    #ifdef BL_LOGGING
      #include "logging.h"
    #endif

    #include "bl_api.h"
    #include "bl_log.h"
    #include "bl_hw.h"

  #endif

#endif // __BLUCCINO_H__
