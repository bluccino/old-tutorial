/* Bluetooth: Mesh Generic OnOff, Generic Level, Lighting & Vendor Models
 *
 * Copyright (c) 2018 Vikrant More
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include <drivers/gpio.h>
#include <bluetooth/bluetooth.h>

#include "app_gpio.h"
#include "ble_mesh.h"
#include "device_composition.h"
#include "no_transition_work_handler.h"
#include "state_binding.h"
#include "storage.h"
#include "transition.h"

#if defined(CONFIG_MCUMGR)
#include <mgmt/mcumgr/smp_bt.h>
#include "smp_svr.h"
#endif

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#include <random/rand32.h>
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

static bool reset;

static void light_default_var_init(void)
{
	ctl->tt = 0x00;

	ctl->onpowerup = STATE_DEFAULT;

	ctl->light->range_min = LIGHTNESS_MIN;
	ctl->light->range_max = LIGHTNESS_MAX;
	ctl->light->last = LIGHTNESS_MAX;
	ctl->light->def = LIGHTNESS_MAX;
	ctl->light->target = ctl->light->def;

	ctl->temp->range_min = TEMP_MIN;
	ctl->temp->range_max = TEMP_MAX;
	ctl->temp->def = TEMP_MAX;
	ctl->temp->target = ctl->temp->def;

	ctl->duv->def = DELTA_UV_DEF;
	ctl->duv->target = ctl->duv->def;
}

/* This function should only get call after execution of settings_load() */
static void light_default_status_init(void)
{
	/* Retrieve Range of Lightness */
	if (ctl->light->range) {
		ctl->light->range_max = (uint16_t) (ctl->light->range >> 16);
		ctl->light->range_min = (uint16_t) ctl->light->range;
	}

	/* Retrieve Range of Temperature */
	if (ctl->temp->range) {
		ctl->temp->range_max = (uint16_t) (ctl->temp->range >> 16);
		ctl->temp->range_min = (uint16_t) ctl->temp->range;
	}

	ctl->light->last = constrain_lightness(ctl->light->last);
	ctl->light->def = constrain_lightness(ctl->light->def);
	ctl->light->target = constrain_lightness(ctl->light->target);

	ctl->temp->def = constrain_temperature(ctl->temp->def);
	ctl->temp->target = constrain_temperature(ctl->temp->target);

	ctl->temp->current = ctl->temp->def;
	ctl->duv->current = ctl->duv->def;

	switch (ctl->onpowerup) {
	case STATE_OFF:
		ctl->light->current = 0U;
		break;
	case STATE_DEFAULT:
		if (ctl->light->def == 0U) {
			ctl->light->current = ctl->light->last;
		} else {
			ctl->light->current = ctl->light->def;
		}
		break;
	case STATE_RESTORE:
		ctl->light->current = ctl->light->target;
		ctl->temp->current = ctl->temp->target;
		ctl->duv->current = ctl->duv->target;
		break;
	}

	ctl->light->target = ctl->light->current;
	ctl->temp->target = ctl->temp->current;
	ctl->duv->target = ctl->duv->current;
}

void update_vnd_led_gpio(void)
{
#ifndef ONE_LED_ONE_BUTTON_BOARD
	gpio_pin_set(led_device[1], DT_GPIO_PIN(DT_ALIAS(led1), gpios),
		     vnd_user_data.current == STATE_ON);
#endif
}

void update_led_gpio(void)
{
	uint8_t power, color;

	power = 100 * ((float) ctl->light->current / 65535);
	color = 100 * ((float) (ctl->temp->current - ctl->temp->range_min) /
		       (ctl->temp->range_max - ctl->temp->range_min));

	printk("power-> %d, color-> %d\n", power, color);

	gpio_pin_set(led_device[0], DT_GPIO_PIN(DT_ALIAS(led0), gpios),
		     ctl->light->current);
#ifndef ONE_LED_ONE_BUTTON_BOARD
	gpio_pin_set(led_device[2], DT_GPIO_PIN(DT_ALIAS(led2), gpios),
		     power < 50);
	gpio_pin_set(led_device[3], DT_GPIO_PIN(DT_ALIAS(led3), gpios),
		     color < 50);
#endif
}

void update_light_state(void)
{
	update_led_gpio();

	if (ctl->transition->counter == 0 || reset == false) {
		reset = true;
		k_work_submit(&no_transition_work);
	}
}

static void short_time_multireset_bt_mesh_unprovisioning(void)
{
	if (reset_counter >= 4U) {
		reset_counter = 0U;
		printk("BT Mesh reset\n");
		bt_mesh_reset();
	} else {
		printk("Reset Counter -> %d\n", reset_counter);
		reset_counter++;
	}

	save_on_flash(RESET_COUNTER);
}

static void reset_counter_timer_handler(struct k_timer *dummy)
{
	reset_counter = 0U;
	save_on_flash(RESET_COUNTER);
	printk("Reset Counter set to Zero\n");
}

K_TIMER_DEFINE(reset_counter_timer, reset_counter_timer_handler, NULL);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
#define DEVICE_NAME CONFIG_BT_DEVICE_NAME
#define DEVICE_NAME_LEN (sizeof(DEVICE_NAME) - 1)


static struct bt_le_ext_adv *adv;
/*
 * Set Advertisement data. Based on the Eddystone specification:
 * https://github.com/google/eddystone/blob/master/protocol-specification.md
 * https://github.com/google/eddystone/tree/master/eddystone-url
 */
static const struct bt_data ad[] = {
	BT_DATA_BYTES(BT_DATA_FLAGS, BT_LE_AD_NO_BREDR),
	BT_DATA(BT_DATA_NAME_COMPLETE, DEVICE_NAME, DEVICE_NAME_LEN),
};

static void create_advertising_set(void)
{
	int err;
	struct bt_le_adv_param param =
		BT_LE_ADV_PARAM_INIT(BT_LE_ADV_OPT_CONNECTABLE |
				     BT_LE_ADV_OPT_EXT_ADV,
				     BT_GAP_ADV_FAST_INT_MIN_2,
				     BT_GAP_ADV_FAST_INT_MAX_2,
				     NULL);

	printk("Starting BLE advertisements\n");

	err = bt_le_ext_adv_create(&param, NULL, &adv);
	if (err) {
		printk("Failed to create advertiser set (%d)\n", err);
		return;
	}

	printk("Created adv: %p\n", adv);

	err = bt_le_ext_adv_set_data(adv, ad, ARRAY_SIZE(ad), NULL, 0);
	if (err) {
		printk("Failed to set advertising data (%d)\n", err);
	}

}

static void start_advertising(void)
{
	int err;
	err = bt_le_ext_adv_start(adv, BT_LE_EXT_ADV_START_DEFAULT);
	if (err) {
		printk("Failed to start advertising set (%d)\n", err);
		return;
	}

	printk("Advertiser %p set started\n", adv);
}

static void stop_advertising(void)
{
	bt_le_ext_adv_stop(adv);
}

#define STACKSIZE 1024
#define PRIORITY 7

void threadMesh(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	int err;

	while(1) 
	{
		printk("going to suspend mesh\n");
		err = bt_mesh_suspend();
		if (err && err != -EALREADY) {
			printk("failed to suspend mesh (err %d)\n", err);
		}
		k_sleep(K_MSEC(sys_rand32_get() % 500));

		printk("going to resume mesh\n");
		err = bt_mesh_resume();
		if (err && err != -EALREADY) {
			printk("failed to resume mesh (err %d)\n", err);
		}
		k_sleep(K_MSEC(sys_rand32_get() % 500));
	}
}

void threadAdv(void *dummy1, void *dummy2, void *dummy3)
{
	ARG_UNUSED(dummy1);
	ARG_UNUSED(dummy2);
	ARG_UNUSED(dummy3);

	create_advertising_set();

	while(1) 
	{
		printk("starting advertising\n");
		start_advertising();
		k_sleep(K_MSEC(sys_rand32_get() % 500));

		printk("stopping advertising\n");
		stop_advertising();
		k_sleep(K_MSEC(sys_rand32_get() % 500));
	}
}

K_THREAD_STACK_DEFINE(threadMesh_stack_area, STACKSIZE);
static struct k_thread threadMesh_data;

K_THREAD_STACK_DEFINE(threadAdv_stack_area, STACKSIZE);
static struct k_thread threadAdv_data;
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<


void main(void)
{
	int err;

	light_default_var_init();

	app_gpio_init();

#if defined(CONFIG_MCUMGR)
	smp_svr_init();
#endif

	printk("Initializing...\n");

	ps_settings_init();

	/* Initialize the Bluetooth Subsystem */
	err = bt_enable(NULL);
	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

	bt_ready();

	light_default_status_init();

	update_light_state();

	short_time_multireset_bt_mesh_unprovisioning();
	k_timer_start(&reset_counter_timer, K_MSEC(7000), K_NO_WAIT);

#if defined(CONFIG_MCUMGR)
	/* Initialize the Bluetooth mcumgr transport. */
	smp_bt_register();

	k_timer_start(&smp_svr_timer, K_NO_WAIT, K_MSEC(1000));
#endif
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
	k_thread_create(&threadMesh_data, threadMesh_stack_area,
			K_THREAD_STACK_SIZEOF(threadMesh_stack_area),
			threadMesh, NULL, NULL, NULL,
			PRIORITY, 0, K_FOREVER);

	k_thread_create(&threadAdv_data, threadAdv_stack_area,
			K_THREAD_STACK_SIZEOF(threadAdv_stack_area),
			threadAdv, NULL, NULL, NULL,
			PRIORITY, 0, K_FOREVER);

	k_thread_start(&threadMesh_data);
	k_thread_start(&threadAdv_data);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}
