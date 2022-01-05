/* Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
 *
 * Copyright (c) 2018 Vikrant More
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "ble_mesh.h"
#include "device_composition.h"

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
	printk("OOB Number: %u\n", number);
	return 0;
}

static int output_string(const char *str)
{
	printk("OOB String: %s\n", str);
	return 0;
}

#endif

static void prov_complete(uint16_t net_idx, uint16_t addr)
{
  #if MIGRATION_STEP2
	  BL_ob o = {CL_MESH,OP_PRV,0,NULL};
	  bl_up(&o,1);
  #endif // MIGRATION_STEP2
}

static void prov_reset(void)
{
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);
  #if MIGRATION_STEP2
	  BL_ob o = {CL_MESH,OP_PRV,0,NULL};
	  bl_up(&o,0);
  #endif // MIGRATION_STEP2
}

#if MIGRATION_STEP2
//==============================================================================
// provisioning link open/close callbacks
//==============================================================================

static void link_open(bt_mesh_prov_bearer_t bearer)
{
	BL_ob o = {CL_MESH, OP_ATT, 1, NULL};
	bl_up(&o,1);
}

static void link_close(bt_mesh_prov_bearer_t bearer)
{
	BL_ob o = {CL_MESH, OP_ATT, 0, NULL};
	bl_up(&o,0);
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

void bt_ready(void)
{
	int err;
	struct bt_le_oob oob;

	printk("Bluetooth initialized\n");

	err = bt_mesh_init(&prov, &comp);
	if (err) {
		printk("Initializing mesh failed (err %d)\n", err);
		return;
	}

	if (IS_ENABLED(CONFIG_SETTINGS)) {
		settings_load();
	}

	/* Use identity address as device UUID */
	if (bt_le_oob_get_local(BT_ID_DEFAULT, &oob)) {
		printk("Identity Address unavailable\n");
	} else {
		memcpy(dev_uuid, oob.addr.a.val, 6);
	}

	bt_mesh_prov_enable(BT_MESH_PROV_GATT | BT_MESH_PROV_ADV);

	printk("Mesh initialized\n");
}
