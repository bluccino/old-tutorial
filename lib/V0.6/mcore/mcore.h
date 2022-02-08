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
//                                +----------------+
//                                |     MCORE      |
//                                +----------------+
//                         INIT ->|SYS:            |
//                         TICK ->|SYS:            |
//                         TOCK ->|SYS:            |
//                                +----------------+
//                          PRV ->|SET:        SET:|-> PRV
//                          ATT ->|SET:        SET:|-> ATT
//                                +----------------+
//                          INC ->|RESET:          |
//                                |          RESET:|-> DUE
//                          PRV ->|RESET:          |
//                                +----------------+
//                        PRESS ->|BUTTON:  BUTTON:|-> PRESS
//                      RELEASE ->|BUTTON:  BUTTON:|-> RELEASE
//                                +----------------+
//                          SET ->|LED:            |
//                                +----------------+
//
//  Input Messages:
//    - [SYS:<INIT <cb>]          init module
//    - [SYS:<TICK @id cnt]       tick the module
//    - [SYS:<TOCK @id cnt]       tock the module
//    - [RESET:<INC <ms>]         inc reset counter and set due (<ms>) timer
//    - [RESET:<PRV]              unprovision node
//    - [SET:$PRV val]            provision on/off
//    - [SET:$ATT val]            attention on/off
//    - [BUTTON:<PRESS @id 1]     button press @ channel @id
//    - [BUTTON:<RELEASE @id 1]   button release @ channel @id
//    - [LED:SET @id val]         set LED
//
//  Output Messages:
//    - [SET:>PRV val]            provision on/off
//    - [SET:>ATT val]            attention on/off
//    - [RESET:>DUE]              reset timer due notification
//    - [BUTTON:>PRESS @id 1]     button press @ channel @id
//    - [BUTTON:>RELEASE @id 1]   button release @ channel @id
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
//     INIT TICK TOCK       PRV   ATT     DUE INC PRV     SET    SET TOGGLE
//       |    |    |         ^     ^       ^   |   |       ^      |     |
//       v    v    v         |     |       |   v   v       |      v     v
//   +--------------------------------------------------------------------+
//   |  =====SYS=====        ==SET==      ===RESET===  ==BUTTON== ==LED== |
//   |                                                                    |
//   |  =====SYS=====        ======HDL======  =RESET=  ==BUTTON== ==LED== |
//   +-------------------------------------o---------------o---------o----+
//                           ^     ^       ^   |   |       ^         |
//                           |     |       |  INC PRV      |        LED:
//                          PRV   ATT     DUE  |   |      SET        |
//                           |     |       |   v   v       |         v
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
    #define MIGRATION_STEP1         1
  #endif
  #ifndef MIGRATION_STEP2
    #define MIGRATION_STEP2         1
  #endif
  #ifndef MIGRATION_STEP3
    #define MIGRATION_STEP3         1
  #endif
  #ifndef MIGRATION_STEP4
    #define MIGRATION_STEP4         1
  #endif
  #ifndef MIGRATION_STEP5
    #define MIGRATION_STEP5         1
  #endif

#endif // __MCORE_H__
