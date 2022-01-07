/* Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
 *
 * Copyright (c) 2018 Vikrant More
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ble_mesh.h"
#include "device_composition.h"

#if MIGRATION_STEP5
  BL_fct output = NULL;
#endif

//==============================================================================
// CORE level logging shorthands
//==============================================================================

  #define LOG                     LOG_CORE
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"mcore:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)
  #define ERR 1,BL_R

//==============================================================================
// overview code migration from original onoff_app sample to a bluccino core
//==============================================================================

#include "bluccino.h"

#ifndef MIGRATION_STEP1
  #define MIGRATION_STEP1         0    // TODO introduce bl_core()
#endif
#ifndef MIGRATION_STEP2
  #define MIGRATION_STEP2         0    // TODO introduce bl_core()
#endif

//==============================================================================
// let's go ...
//==============================================================================

#ifdef OOB_AUTH_ENABLE

static int output_number(bt_mesh_output_action_t action, uint32_t number)
{
  #if MIGRATION_STEP2
   	LOG(1,BL_M "OOB Number: %u", number);
	#else
   	printk("OOB Number: %u\n", number);
	#endif
	return 0;
}

static int output_string(const char *str)
{
  #if MIGRATION_STEP2
  	LOG(1,BL_M "OOB String: %s", str);
	#else
  	printk("OOB String: %s\n", str);
	#endif
	return 0;
}

#endif

static void prov_complete(uint16_t net_idx, uint16_t addr)
{
  #if MIGRATION_STEP2   // pimped in MIGRATION_STEP5
	  BL_ob o = {CL_SYS,OP_PRV,0,NULL};
	  bl_out(&o,1,output);         // post [MESH:PRV 1] to core, which posts it up
  #endif // MIGRATION_STEP2
}

static void prov_reset(void)
{
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
  #if MIGRATION_STEP2  // pimped in MIGRATION_STEP5
	  BL_ob o = {CL_SYS,OP_PRV,0,NULL};
	  bl_out(&o,0,output);         // post [MESH:PRV 0] to core, which posts it up
  #endif // MIGRATION_STEP2
}

#if MIGRATION_STEP2  // pimped in MIGRATION_STEP5
//==============================================================================
// provisioning link open/close callbacks
//==============================================================================

static void link_open(bt_mesh_prov_bearer_t bearer)
{
	BL_ob o = {CL_SYS, OP_ATT, 1, NULL};
	bl_out(&o,1,output);        // post [MESH:ATT 1] to core, which posts it up
}

static void link_close(bt_mesh_prov_bearer_t bearer)
{
	BL_ob o = {CL_SYS, OP_ATT, 0, NULL};
	bl_out(&o,0,output);        // post [MESH:ATT 0] to core, which posts it up
}

//==============================================================================
// provisioning table
//==============================================================================
#endif // MIGRATION_STEP2

static uint8_t dev_uuid[16] = { 0xdd, 0xdd };

static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,

#ifdef OOB_AUTH_ENABLE

	.output_size = 6,
	.output_actions = BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING,
	.output_number = output_number,
	.output_string = output_string,

#endif
#if MIGRATION_STEP2
  .link_open = link_open,              // to activate attention mode
  .link_close = link_close,            // to deactivate attention mode
#endif // MIGRATION_STEP2
	.complete = prov_complete,
	.reset = prov_reset,
};

#if MIGRATION_STEP5
static void bt_ready(void)
#else
void bt_ready(void)
#endif
{
	int err;
	struct bt_le_oob oob;

  #if MIGRATION_STEP2
  	LOG(2,BL_B "Bluetooth initialized");
	#else
  	printk("Bluetooth initialized\n");
  #endif

	err = bt_mesh_init(&prov, &comp);
	if (err)
	{
    #if MIGRATION_STEP2
	  	LOG(ERR "Initializing mesh failed (err %d)", err);
		#else
	  	printk("Initializing mesh failed (err %d)\n", err);
		#endif
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	/* Use identity address as device UUID */
	if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob))
	{
          #if MIGRATION_STEP2
  		LOG(1,BL_R "Identity Address unavailable");
	  #else
  		printk("Identity Address unavailable\n");
	  #endif
	} else {
		memcpy(dev_uuid, oob.addr.a.val, 6);
	}

	bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

	#if MIGRATION_STEP2
    LOG(2,BL_B"Mesh initialized");
	#else
    printk("Mesh initialized\n");
	#endif
}

//==============================================================================
// public module interface
//==============================================================================

  int blemesh(BL_ob *o, int val)
  {
    switch (bl_id(o))
    {
      case BL_ID(CL_SYS,OP_INIT):      // [SYS:INIT <cb>] init sub module
        output = o->data;              // store output cb
        return 0;                      // OK

      case BL_ID(CL_SYS,OP_READY):     // [SYS:READY] BLE/MESH init
        bt_ready();                    // init BLE/Mesh when Bluetooth is ready
        return 0;                      // OK

      default:
        return -1;                     // bad args
    }
  }
