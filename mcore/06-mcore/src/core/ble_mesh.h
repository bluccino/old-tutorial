//==============================================================================
// mcore.h
// multi model mesh demo based mesh core
//
// Adopted to Bluccino by Hugo Pristauz on 2022-Jan-07
// Copyright © 2022 Bluccino. All rights reserved.
//==============================================================================
// SYS interface: [PRV,ATT] = SYS()
//
//                          +---------------+
//                   INIT ->|               |
//                          |      SYS      |
//                  READY ->|               |
//                          +---------------+
//                    PRV ->o               |-> PRV
//                          |      SET      |
//                    ATT ->o               |-> ATT
//                          +---------------+
//                               BLEMESH
//  Input Messages:
//    - [SYS:INIT <cb>]     init module
//    - [SYS:READY]         init BLE/Mesh when Bluetooth is ready
//
//  Output Messages:
//    - [SET:PRV val]       provisioning on/off
//    - [SET:ATT val]       attentioning on/off
//
//==============================================================================
// Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
// Copyright (c) 2018 Vikrant More
// SPDX-License-Identifier: Apache-2.0
//==============================================================================

#ifndef _BLE_MESH_H
#define _BLE_MESH_H

#include <settings/settings.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/conn.h>
#include <bluetooth/l2cap.h>
#include <bluetooth/hci.h>
#include <bluetooth/mesh.h>

#include "mcore.h"          // MIGRATION_STEP5

/* Model Operation Codes */
#define	BT_MESH_MODEL_OP_GEN_ONOFF_GET          BT_MESH_MODEL_OP_2(0x82, 0x01)
#define	BT_MESH_MODEL_OP_GEN_ONOFF_SET          BT_MESH_MODEL_OP_2(0x82, 0x02)
#define	BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK    BT_MESH_MODEL_OP_2(0x82, 0x03)
#define	BT_MESH_MODEL_OP_GEN_ONOFF_STATUS       BT_MESH_MODEL_OP_2(0x82, 0x04)

#define	BT_MESH_MODEL_OP_GEN_LEVEL_GET          BT_MESH_MODEL_OP_2(0x82, 0x05)
#define	BT_MESH_MODEL_OP_GEN_LEVEL_SET          BT_MESH_MODEL_OP_2(0x82, 0x06)
#define	BT_MESH_MODEL_OP_GEN_LEVEL_SET_UNACK    BT_MESH_MODEL_OP_2(0x82, 0x07)
#define	BT_MESH_MODEL_OP_GEN_LEVEL_STATUS       BT_MESH_MODEL_OP_2(0x82, 0x08)
#define	BT_MESH_MODEL_OP_GEN_DELTA_SET          BT_MESH_MODEL_OP_2(0x82, 0x09)
#define	BT_MESH_MODEL_OP_GEN_DELTA_SET_UNACK    BT_MESH_MODEL_OP_2(0x82, 0x0A)
#define	BT_MESH_MODEL_OP_GEN_MOVE_SET           BT_MESH_MODEL_OP_2(0x82, 0x0B)
#define	BT_MESH_MODEL_OP_GEN_MOVE_SET_UNACK     BT_MESH_MODEL_OP_2(0x82, 0x0C)

#define	BT_MESH_MODEL_GEN_DEF_TRANS_TIME_GET	BT_MESH_MODEL_OP_2(0x82, 0x0D)
#define	BT_MESH_MODEL_GEN_DEF_TRANS_TIME_SET	BT_MESH_MODEL_OP_2(0x82, 0x0E)
#define	BT_MESH_MODEL_GEN_DEF_TRANS_TIME_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x0F)
#define	BT_MESH_MODEL_GEN_DEF_TRANS_TIME_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x10)

#define	BT_MESH_MODEL_GEN_ONPOWERUP_GET		BT_MESH_MODEL_OP_2(0x82, 0x11)
#define	BT_MESH_MODEL_GEN_ONPOWERUP_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x12)
#define	BT_MESH_MODEL_GEN_ONPOWERUP_SET		BT_MESH_MODEL_OP_2(0x82, 0x13)
#define	BT_MESH_MODEL_GEN_ONPOWERUP_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x14)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_GET	BT_MESH_MODEL_OP_2(0x82, 0x4B)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_SET	BT_MESH_MODEL_OP_2(0x82, 0x4C)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x4D)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x4E)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_GET \
	BT_MESH_MODEL_OP_2(0x82, 0x4F)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET \
	BT_MESH_MODEL_OP_2(0x82, 0x50)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x51)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LINEAR_STATUS \
	BT_MESH_MODEL_OP_2(0x82, 0x52)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_GET \
	BT_MESH_MODEL_OP_2(0x82, 0x53)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_LAST_STATUS \
	BT_MESH_MODEL_OP_2(0x82, 0x54)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_GET \
	BT_MESH_MODEL_OP_2(0x82, 0x55)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_STATUS \
	BT_MESH_MODEL_OP_2(0x82, 0x56)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_GET \
	BT_MESH_MODEL_OP_2(0x82, 0x57)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_STATUS \
	BT_MESH_MODEL_OP_2(0x82, 0x58)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET \
	BT_MESH_MODEL_OP_2(0x82, 0x59)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_DEFAULT_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x5A)

#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET \
	BT_MESH_MODEL_OP_2(0x82, 0x5B)
#define	BT_MESH_MODEL_LIGHT_LIGHTNESS_RANGE_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x5C)

#define	BT_MESH_MODEL_LIGHT_CTL_GET	BT_MESH_MODEL_OP_2(0x82, 0x5D)
#define	BT_MESH_MODEL_LIGHT_CTL_SET	BT_MESH_MODEL_OP_2(0x82, 0x5E)
#define	BT_MESH_MODEL_LIGHT_CTL_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x5F)
#define	BT_MESH_MODEL_LIGHT_CTL_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x60)

#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_GET	BT_MESH_MODEL_OP_2(0x82, 0x61)

#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_RANGE_GET \
	BT_MESH_MODEL_OP_2(0x82, 0x62)
#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_RANGE_STATUS \
	BT_MESH_MODEL_OP_2(0x82, 0x63)

#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_SET	BT_MESH_MODEL_OP_2(0x82, 0x64)
#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x65)
#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x66)

#define	BT_MESH_MODEL_LIGHT_CTL_DEFAULT_GET	BT_MESH_MODEL_OP_2(0x82, 0x67)
#define	BT_MESH_MODEL_LIGHT_CTL_DEFAULT_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x68)

#define	BT_MESH_MODEL_LIGHT_CTL_DEFAULT_SET  BT_MESH_MODEL_OP_2(0x82, 0x69)
#define	BT_MESH_MODEL_LIGHT_CTL_DEFAULT_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x6A)

#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_RANGE_SET \
	BT_MESH_MODEL_OP_2(0x82, 0x6B)
#define	BT_MESH_MODEL_LIGHT_CTL_TEMP_RANGE_SET_UNACK \
	BT_MESH_MODEL_OP_2(0x82, 0x6C)

//==============================================================================
// public module interface
//==============================================================================

  int blemesh(BL_ob *o, int val);

//==============================================================================
// syntactic sugar: blemesh_ready // start BT & mesh init when Bluetooth ready)
// - usage: blemesh_ready()       // calls local bt_ready()
//==============================================================================

  static inline void blemesh_ready(void)
  {
    bl_sys(blemesh,READY_,NULL,0);
  }

#endif
