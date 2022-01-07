//==============================================================================
// mcore.h
// multi model mesh demo based mesh core
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
//                                  INIT  TICK TOCK
//                                    |    |    |
//                                    v    v    v
//                                  +-------------+
//                            PRV ->|             |-> PRV
//                                  |     SYS     |
//                            ATT ->|             |-> ATT
//                                  +-------------+
//                                  |             |
//                            SET ->|   BUTTON    |-> SET
//                                  |             |
//                                  +-------------+
//
//  Input Messages:
//    - [SYS:INIT <cb>]     init module
//    - [SYS:TICK @id cnt]  tick the module
//    - [SYS:TOCK @id cnt]  tock the module
//    - [SYS:PRV val]       provisioning on/off
//    - [SYS:ATT val]       attentioning on/off
//
//  Output Messages:
//    - [SYS:PRV val]       provisioning on/off
//    - [SYS:ATT val]       attentioning on/off
//    - [BUTTON:SET @id 1]  button press @ channel @id
//
//==============================================================================
// mcore derived from:
// Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
// Copyright (c) 2018 Vikrant More
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

#ifndef __MCORE_H__
#define __MCORE_H__

  #include "bluccino.h"

//==============================================================================
// migration defaults
//==============================================================================

  #ifndef MIGRATION_STEP1
    #define MIGRATION_STEP1         0
  #endif
  #ifndef MIGRATION_STEP2
    #define MIGRATION_STEP2         0
  #endif
  #ifndef MIGRATION_STEP3
    #define MIGRATION_STEP3         0
  #endif
  #ifndef MIGRATION_STEP4
    #define MIGRATION_STEP4         0
  #endif
  #ifndef MIGRATION_STEP5
    #define MIGRATION_STEP5         0
  #endif

#endif // __MCORE_H__
