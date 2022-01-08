//==============================================================================
// mcore.h
// multi model mesh demo based mesh core
//
// Created by Hugo Pristauz on 2022-Jan-02
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
//
// Interfaces:
//   [PRV,ATT]=MCORE.SYS(PRV,ATT);
//       [CNT]=MCORE.RESET(PRV);
//       [SET]=MCORE.BUTTON(SET);
//
//                                  INIT  TICK TOCK
//                                    |    |    |
//                                    v    v    v
//                                  +-------------+
//                            PRV ->:             |-> PRV
//                                  |   ==SYS==   |
//                            ATT ->:             |-> ATT
//                                  +-------------+
//                            INC ->|             |
//                                  |  ==RESET==  |-> DUE
//                            PRV ->|             |
//                                  +-------------+
//                            SET ->:  ==BUTTON== |-> SET
//                                  +-------------+
//                                       MCORE
//  Input Messages:
//    - [SYS<INIT <cb>]     init module
//    - [SYS<TICK @id cnt]  tick the module
//    - [SYS<TOCK @id cnt]  tock the module
//    - [RESET<INC <ms>]    inc reset counter and set due (<ms>) timer
//    - [RESET<PRV]         unprovision node
//    - [SET:PRV val]       provisioning on/off
//    - [SET:ATT val]       attentioning on/off
//    - [BUTTON:SET @id 1]  button press @ channel @id
//
//  Output Messages:
//    - [SET>PRV val]       provisioning on/off
//    - [SET>ATT val]       attentioning on/off
//    - [RESET>DUE]         reset timer due notification
//    - [BUTTON>SET @id 1]  button press @ channel @id
//
//==============================================================================
//  Reset system
//  - after system init the app calls [RESET:INC <timeout>] which increments
//    the reset counter (non-volatile memory), while a timer with <timeout>
//    is started at the same time, which emits the event [RESET:DUE] after
//    timeout
//  - The call [RESET:INC] returns the current value of the reset counter. Based
//    on this value the application may give an indication to the user (e.g.
//    different kinf of LED pattern depending on the value, or may invoke a
//    mesh node reset [RESET:PRV] to unprovision the node
//==============================================================================
//
//     INIT TICK TOCK       PRV   ATT     INC DUE PRV     SET    SET TOGGLE
//       |    |    |         ^     ^       |   ^   |       ^      |     |
//       v    v    v         |     |       v   |   v       |      v     v
//   +--------------------------------------------------------------------+
//   |  =====SYS=====        ==SET==      ===RESET===  ==BUTTON== ==LED== |
//   +-----------------------o-----o-------o---o---o-------o---------o----+
//                           ^     ^       |   ^   |       ^         |
//                           |     |      INC  |  PRV      |        LED:
//                          PRV   ATT      |  DUE  |      SET        |
//                           |     |       v   |   v       |         v
//                       +-------------+ +-----------+ +------------------+
//                       |   ==SET==   | | ==RESET== | |=BUTTON=  ==LED== |
//                       |             | |           | |                  |
//                       |   BLEMESH   | |   MCORE   | |      MGPIO       |
//                       +-------------+ +-----------+ +------------------+
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
