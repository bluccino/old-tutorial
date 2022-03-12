//==============================================================================
// ocore4.c - onoff-app based mesh/HW core, version 4                 //@@@1.1
//==============================================================================
//
// OCORE Versions 1 (ocore1)
//   - filename and heading changed                                     @@@1.1
//   - #include "bccore.h" added                                        @@@1.2
//   - rename main() to bl_core_init()                                  @@@1.3
//   - add a dummy bl_core_loop() function                              @@@1.4
//
// OCORE Versions 2 (ocore2)
//   - include "bluccino.h"                                             @@@2.1
//   - provide bl_core_set() function to change state                   @@@2.2
//
// OCORE Versions 3 (ocore3)
//   - add link_open in provisioning table                              @@@3.1
//   - add link_close in provisioning table                             @@@3.2
//   - provisioning link open/close callbacks                           @@@3.3
//   - notify blapi.c in prov_complete                                  @@@3.4
//   - notify blapi.c in prov_reset                                     @@@3.5
//   - include <stdlib.h>                                               @@@3.6
//   - use bl_log() for logging                                         @@@3.7
//   - #include "bllog.h" added                                         @@@3.8
//
// OCORE Versions 4 (ocore4)
//   - lower BUTTON_DEBOUNCE_DELAY_MS from 250 to 120ms                 @@@4.1
//	 - lower sw.button_timer interval from 1.0 to 0.6s                  @@@4.2
//   - define notify_work, provide code for notify_worker               @@@4.3
//   - in button_cnt_timer submit a 'notify_work'                       @@@4.4
//   - assign (init) notify_work with notify_worker                     @@@4.5
//   - dont directly set LED                                            @@@4.6
//   - provide basis function to set LED                                @@@4.7
//   - change name 'onoff_state' to 'p'                                 @@@4.8
//   - invoke receive callback                                          @@@4.9
//
//==============================================================================

	#include <sys/printk.h>
	#include <settings/settings.h>
	#include <sys/byteorder.h>
	#include <device.h>
	#include <drivers/gpio.h>
	#include <bluetooth/bluetooth.h>
	#include <bluetooth/conn.h>
	#include <bluetooth/l2cap.h>
	#include <bluetooth/hci.h>
	#include <bluetooth/mesh.h>
	#include <stdio.h>
	#include <stdlib.h>

	#include "bluccino.h"

  #define _PRESS_    BL_HASH(PRESS_)   // hashed symbol #PRESS
  #define _RELEASE_  BL_HASH(RELEASE_) // hashed symbol #RELEASE
  #define _STS_      BL_HASH(STS_)     // hashed symbol #STS
  #define _PRV_      BL_HASH(PRV_)     // hashed symbol #PRV
  #define _ATT_      BL_HASH(ATT_)     // hashed symbol #ATT

//==============================================================================
// CORE level logging shorthands
//==============================================================================

  #define LOG                     LOG_CORE
  #define LOGO(lvl,col,o,val)     LOGO_CORE(lvl,col"ocore4:",o,val)
  #define LOG0(lvl,col,o,val)     LOGO_CORE(lvl,col,o,val)

//==============================================================================
// let's go ...
//==============================================================================

/* Model Operation Codes */
#define BT_MESH_MODEL_OP_GEN_ONOFF_GET		BT_MESH_MODEL_OP_2(0x82, 0x01)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET		BT_MESH_MODEL_OP_2(0x82, 0x02)
#define BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK	BT_MESH_MODEL_OP_2(0x82, 0x03)
#define BT_MESH_MODEL_OP_GEN_ONOFF_STATUS	BT_MESH_MODEL_OP_2(0x82, 0x04)

static int gen_onoff_set(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf);

static int gen_onoff_set_unack(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf);

static int gen_onoff_get(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf);

static int gen_onoff_status(struct bt_mesh_model *model,
			    struct bt_mesh_msg_ctx *ctx,
			    struct net_buf_simple *buf);

/*
 * Client Configuration Declaration
 */

static struct bt_mesh_cfg_cli cfg_cli = {
};

/*
 * Health Server Declaration
 */

static struct bt_mesh_health_srv health_srv = {
};

/*
 * Publication Declarations
 *
 * The publication messages are initialized to the
 * the size of the opcode + content
 *
 * For publication, the message must be in static or global as
 * it is re-transmitted several times. This occurs
 * after the function that called bt_mesh_model_publish() has
 * exited and the stack is no longer valid.
 *
 * Note that the additional 4 bytes for the AppMIC is not needed
 * because it is added to a stack variable at the time a
 * transmission occurs.
 *
 */

BT_MESH_HEALTH_PUB_DEFINE(health_pub, 0);

BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_cli, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv_s_0, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_cli_s_0, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv_s_1, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_cli_s_1, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_srv_s_2, NULL, 2 + 2);
BT_MESH_MODEL_PUB_DEFINE(gen_onoff_pub_cli_s_2, NULL, 2 + 2);

/*
 * Models in an element must have unique op codes.
 *
 * The mesh stack dispatches a message to the first model in an element
 * that is also bound to an app key and supports the op code in the
 * received message.
 *
 */

/*
 * OnOff Model Server Op Dispatch Table
 *
 */

static const struct bt_mesh_model_op gen_onoff_srv_op[] = {
	{ BT_MESH_MODEL_OP_GEN_ONOFF_GET,       BT_MESH_LEN_EXACT(0), gen_onoff_get },
	{ BT_MESH_MODEL_OP_GEN_ONOFF_SET,       BT_MESH_LEN_EXACT(2), gen_onoff_set },
	{ BT_MESH_MODEL_OP_GEN_ONOFF_SET_UNACK, BT_MESH_LEN_EXACT(2), gen_onoff_set_unack },
	BT_MESH_MODEL_OP_END,
};

/*
 * OnOff Model Client Op Dispatch Table
 */

static const struct bt_mesh_model_op gen_onoff_cli_op[] = {
	{ BT_MESH_MODEL_OP_GEN_ONOFF_STATUS, BT_MESH_LEN_EXACT(1), gen_onoff_status },
	BT_MESH_MODEL_OP_END,
};

struct onoff_state {
	uint8_t current;
	uint8_t previous;
	uint8_t led_gpio_pin;
	const struct device *led_device;
};

/*
 * Declare and Initialize Element Contexts
 * Change to select different GPIO output pins
 */

static struct onoff_state onoff_state[] = {
	{ .led_gpio_pin = DT_GPIO_PIN(DT_ALIAS(led0), gpios) },
	{ .led_gpio_pin = DT_GPIO_PIN(DT_ALIAS(led1), gpios) },
	{ .led_gpio_pin = DT_GPIO_PIN(DT_ALIAS(led2), gpios) },
	{ .led_gpio_pin = DT_GPIO_PIN(DT_ALIAS(led3), gpios) },
};

/*
 *
 * Element Model Declarations
 *
 * Element 0 Root Models
 */

static struct bt_mesh_model root_models[] = {
	BT_MESH_MODEL_CFG_SRV,
	BT_MESH_MODEL_CFG_CLI(&cfg_cli),
	BT_MESH_MODEL_HEALTH_SRV(&health_srv, &health_pub),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
		      &gen_onoff_pub_srv, &onoff_state[0]),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
		      &gen_onoff_pub_cli, &onoff_state[0]),
};

/*
 * Element 1 Models
 */

static struct bt_mesh_model secondary_0_models[] = {
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
		      &gen_onoff_pub_srv_s_0, &onoff_state[1]),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
		      &gen_onoff_pub_cli_s_0, &onoff_state[1]),
};

/*
 * Element 2 Models
 */

static struct bt_mesh_model secondary_1_models[] = {
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
		      &gen_onoff_pub_srv_s_1, &onoff_state[2]),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
		      &gen_onoff_pub_cli_s_1, &onoff_state[2]),
};

/*
 * Element 3 Models
 */

static struct bt_mesh_model secondary_2_models[] = {
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_SRV, gen_onoff_srv_op,
		      &gen_onoff_pub_srv_s_2, &onoff_state[3]),
	BT_MESH_MODEL(BT_MESH_MODEL_ID_GEN_ONOFF_CLI, gen_onoff_cli_op,
		      &gen_onoff_pub_cli_s_2, &onoff_state[3]),
};

/*
 * Button to Client Model Assignments
 */

struct bt_mesh_model *mod_cli_sw[] = {
		&root_models[4],
		&secondary_0_models[1],
		&secondary_1_models[1],
		&secondary_2_models[1],
};

/*
 * LED to Server Model Assigmnents
 */

struct bt_mesh_model *mod_srv_sw[] = {
		&root_models[3],
		&secondary_0_models[0],
		&secondary_1_models[0],
		&secondary_2_models[0],
};

/*
 * Root and Secondary Element Declarations
 */

static struct bt_mesh_elem elements[] = {
	BT_MESH_ELEM(0, root_models, BT_MESH_MODEL_NONE),
	BT_MESH_ELEM(0, secondary_0_models, BT_MESH_MODEL_NONE),
	BT_MESH_ELEM(0, secondary_1_models, BT_MESH_MODEL_NONE),
	BT_MESH_ELEM(0, secondary_2_models, BT_MESH_MODEL_NONE),
};

static const struct bt_mesh_comp comp = {
	.cid = BT_COMP_ID_LF,
	.elem = elements,
	.elem_count = ARRAY_SIZE(elements),
};

const struct device *sw_device;

struct sw {
	uint8_t sw_num;
	uint8_t onoff_state;
	struct k_work button_work;
	struct k_timer button_timer;
};

static uint8_t button_press_cnt;
static struct sw sw;

static struct gpio_callback button_cb;

static uint8_t trans_id;
static uint32_t time, last_time;
static uint16_t primary_addr;
static uint16_t primary_net_idx;

/*
 * Generic OnOff Model Server Message Handlers
 *
 * Mesh Model Specification 3.1.1
 *
 */

static int gen_onoff_get(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf)
{
	NET_BUF_SIMPLE_DEFINE(msg, 2 + 1 + 4);
	struct onoff_state *onoff_state = model->user_data;

  if (bl_dbg(4))
	  printk(BL_Y"addr 0x%04x onoff 0x%02x\n"BL_0,
	       bt_mesh_model_elem(model)->addr, onoff_state->current);

	bt_mesh_model_msg_init(&msg, BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
	net_buf_simple_add_u8(&msg, onoff_state->current);

	if (bt_mesh_model_send(model, ctx, &msg, NULL, NULL)) {
		printk("Unable to send On Off Status response\n");
	}

	return 0;
}

static int gen_onoff_set_unack(struct bt_mesh_model *model,
			       struct bt_mesh_msg_ctx *ctx,
			       struct net_buf_simple *buf)
{
	struct net_buf_simple *msg = model->pub->msg;
	struct onoff_state *p = model->user_data;                           //@@@4.8
	int err;

	p->current = net_buf_simple_pull_u8(buf);                           //@@@4.8

	if (bl_dbg(4))
  	printk(BL_Y"addr 0x%02x state 0x%02x\n"BL_0,
	       bt_mesh_model_elem(model)->addr, p->current);                //@@@4.8


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//   - dont directly set LED                                          //@@@4.6
//	gpio_pin_set(p->led_device, p->led_gpio_pin,
//		     p->current);
  int id = p->led_gpio_pin - 16;
  BL_ob oo = {_GOOSRV,LET_,id,NULL};
  bl_core(&oo,p->current);                                            //@@@4.9
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

	/*
	 * If a server has a publish address, it is required to
	 * publish status on a state change
	 *
	 * See Mesh Profile Specification 3.7.6.1.2
	 *
	 * Only publish if there is an assigned address
	 */

	if (p->previous != p->current &&                                    //@@@4.8
	    model->pub->addr != BT_MESH_ADDR_UNASSIGNED) {
		printk("publish last 0x%02x cur 0x%02x\n",
		       p->previous, p->current);                                  //@@@4.8
		p->previous = p->current;
		bt_mesh_model_msg_init(msg,
				       BT_MESH_MODEL_OP_GEN_ONOFF_STATUS);
		net_buf_simple_add_u8(msg, p->current);                           //@@@4.8
		err = bt_mesh_model_publish(model);
		if (err) {
			printk("bt_mesh_model_publish err %d\n", err);
		}
	}

	return 0;
}

static int gen_onoff_set(struct bt_mesh_model *model,
			 struct bt_mesh_msg_ctx *ctx,
			 struct net_buf_simple *buf)
{
	if (bl_dbg(4))
	   printk(BL_Y"gen_onoff_set\n"BL_0);

	(void)gen_onoff_set_unack(model, ctx, buf);
	(void)gen_onoff_get(model, ctx, buf);

	return 0;
}

static int gen_onoff_status(struct bt_mesh_model *model,
			    struct bt_mesh_msg_ctx *ctx,
			    struct net_buf_simple *buf)
{
	uint8_t	state;

	state = net_buf_simple_pull_u8(buf);

	if (bl_dbg(4))
  	printk(BL_Y"Node 0x%04x OnOff status from 0x%04x with state 0x%02x\n"BL_0,
	       bt_mesh_model_elem(model)->addr, ctx->addr, state);

	return 0;
}

static int output_number(bt_mesh_output_action_t action, uint32_t number)
{
	printk("OOB Number %06u\n", number);
	return 0;
}

static int output_string(const char *str)
{
	printk("OOB String %s\n", str);
	return 0;
}

static void prov_complete(uint16_t net_idx, uint16_t addr)
{
	printk("provisioning complete for net_idx 0x%04x addr 0x%04x\n",
	       net_idx, addr);
	primary_addr = addr;
	primary_net_idx = net_idx;

	BL_ob oo = {_SET, PRV_, 0, NULL};                               //@@@3.4
	bl_core(&oo,1);
}

static void prov_reset(void)
{
	bt_mesh_prov_enable(BT_MESH_PROV_ADV | BT_MESH_PROV_GATT);

	BL_ob oo = {_SET, PRV_, 0, NULL};                               //@@@3.4
	bl_core(&oo,0);
}

static uint8_t dev_uuid[16] = { 0xdd, 0xdd };


//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//==============================================================================
// button notification
//==============================================================================

static struct k_work notify_work;

static void notify_worker(struct k_work *work)
{
	BL_ob oo = {_BUTTON, _PRESS_, sw.sw_num+1, NULL};   // button ID = sw_num+1
	bl_core(&oo,sw.onoff_state);
}
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//#define BUTTON_DEBOUNCE_DELAY_MS 250
#define BUTTON_DEBOUNCE_DELAY_MS 120                                  //@@@.4
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

/*
 * Map GPIO pins to button number
 * Change to select different GPIO input pins
 */

static uint8_t pin_to_sw(uint32_t pin_pos)
{
	switch (pin_pos) {
	case BIT(DT_GPIO_PIN(DT_ALIAS(sw0), gpios)): return 0;
	case BIT(DT_GPIO_PIN(DT_ALIAS(sw1), gpios)): return 1;
	case BIT(DT_GPIO_PIN(DT_ALIAS(sw2), gpios)): return 2;
	case BIT(DT_GPIO_PIN(DT_ALIAS(sw3), gpios)): return 3;
	}

	printk("No match for GPIO pin 0x%08x\n", pin_pos);
	return 0;
}

static void button_pressed(const struct device *dev, struct gpio_callback *cb,
			   uint32_t pin_pos)
{
	/*
	 * One button press within a 1 second interval sends an on message
	 * More than one button press sends an off message
	 */

	time = k_uptime_get_32();

	/* debounce the switch */
	if (time < last_time + BUTTON_DEBOUNCE_DELAY_MS) {
		last_time = time;
		return;
	}

	if (button_press_cnt == 0U) {
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//	k_timer_start(&sw.button_timer, K_SECONDS(1), K_NO_WAIT);
		k_timer_start(&sw.button_timer, K_SECONDS(0.6), K_NO_WAIT);        //@@@4.2
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	}

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (bl_dbg(4))
  	printk(BL_Y "button_press_cnt 0x%02x\n" BL_0, button_press_cnt);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	button_press_cnt++;

	/* The variable pin_pos is the pin position in the GPIO register,
	 * not the pin number. It's assumed that only one bit is set.
	 */

	sw.sw_num = pin_to_sw(pin_pos);
	last_time = time;
}

/*
 * Button Count Timer Worker
 */

static void button_cnt_timer(struct k_timer *work)
{
	struct sw *button_sw = CONTAINER_OF(work, struct sw, button_timer);

	button_sw->onoff_state = button_press_cnt == 1U ? 1 : 0;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
  if (bl_dbg(4))
	  printk(BL_Y "button_press_cnt 0x%02x onoff_state 0x%02x\n" BL_0,
	       button_press_cnt, button_sw->onoff_state);
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	button_press_cnt = 0U;
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//k_work_submit(&sw.button_work);
	k_work_submit(&notify_work);                                         //@@@4.4
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
}

/*
 * Button Pressed Worker Task
 */

static void button_pressed_worker(struct k_work *work)
{
	struct bt_mesh_model *mod_cli, *mod_srv;
	struct bt_mesh_model_pub *pub_cli, *pub_srv;
	struct sw *sw = CONTAINER_OF(work, struct sw, button_work);
	int err;
	uint8_t sw_idx = sw->sw_num;

	mod_cli = mod_cli_sw[sw_idx];
	pub_cli = mod_cli->pub;

	mod_srv = mod_srv_sw[sw_idx];
	pub_srv = mod_srv->pub;

	/* If unprovisioned, just call the set function.
	 * The intent is to have switch-like behavior
	 * prior to provisioning. Once provisioned,
	 * the button and its corresponding led are no longer
	 * associated and act independently. So, if a button is to
	 * control its associated led after provisioning, the button
	 * must be configured to either publish to the led's unicast
	 * address or a group to which the led is subscribed.
	 */

	if (primary_addr == BT_MESH_ADDR_UNASSIGNED) {
		NET_BUF_SIMPLE_DEFINE(msg, 1);
		struct bt_mesh_msg_ctx ctx = {
			.addr = sw_idx + primary_addr,
		};

		/* This is a dummy message sufficient
		 * for the led server
		 */

		net_buf_simple_add_u8(&msg, sw->onoff_state);
		(void)gen_onoff_set_unack(mod_srv, &ctx, &msg);
		return;
	}

	if (pub_cli->addr == BT_MESH_ADDR_UNASSIGNED) {
		return;
	}

	if (bl_dbg(4))
  	printk(BL_Y"publish to 0x%04x onoff 0x%04x sw_idx 0x%04x\n"BL_0,
	       pub_cli->addr, sw->onoff_state, sw_idx);
	bt_mesh_model_msg_init(pub_cli->msg,
			       BT_MESH_MODEL_OP_GEN_ONOFF_SET);
	net_buf_simple_add_u8(pub_cli->msg, sw->onoff_state);
	net_buf_simple_add_u8(pub_cli->msg, trans_id++);
	err = bt_mesh_model_publish(mod_cli);
	if (err) {
		printk("bt_mesh_model_publish err %d\n", err);
	}
}

//==============================================================================
// provisioning link open/close callbacks                             //@@@3.3
//==============================================================================

static void link_open(bt_mesh_prov_bearer_t bearer)
{
	BL_ob oo = {_SET, _ATT_, 0, NULL};
	bl_core(&oo,1);
}

static void link_close(bt_mesh_prov_bearer_t bearer)
{
	BL_ob oo = {_SET,_ATT_, 0, NULL};
	bl_core(&oo,0);
}

//==============================================================================
// provisioning table
//==============================================================================

/* Disable OOB security for SILabs Android app */

static const struct bt_mesh_prov prov = {
	.uuid = dev_uuid,
#if 1
	.output_size = 6,
	.output_actions = (BT_MESH_DISPLAY_NUMBER | BT_MESH_DISPLAY_STRING),
	.output_number = output_number,
	.output_string = output_string,
#else
	.output_size = 0,
	.output_actions = 0,
	.output_number = 0,
#endif
  .link_open = link_open,                                             //@@@3.1
  .link_close = link_close,                                           //@@@3.2
	.complete = prov_complete,
	.reset = prov_reset,
};

/*
 * Bluetooth Ready Callback
 */

static void bt_ready(int err)
{
	struct bt_le_oob oob;

	if (err) {
		printk("Bluetooth init failed (err %d)\n", err);
		return;
	}

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

void init_led(uint8_t dev, const char *port, uint32_t pin_num, gpio_flags_t flags)
{
	onoff_state[dev].led_device = device_get_binding(port);
	gpio_pin_configure(onoff_state[dev].led_device, pin_num,
			   flags | GPIO_OUTPUT_INACTIVE);
}

  static int init(BL_ob *o, int val)
  {
    LOGO(3,BL_B"init ",o,val);
    int err;

    printk("Initializing...\n");

    /* Initialize the button debouncer */
    last_time = k_uptime_get_32();

    /* Initialize button worker task*/
    k_work_init(&sw.button_work, button_pressed_worker);

//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
    /* Initialize button worker task*/
    k_work_init(&notify_work, notify_worker);                            //@@@4.5
//<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

    /* Initialize button count timer */
    k_timer_init(&sw.button_timer, button_cnt_timer, NULL);

    sw_device = device_get_binding(DT_GPIO_LABEL(DT_ALIAS(sw0), gpios));
    gpio_pin_configure(sw_device, DT_GPIO_PIN(DT_ALIAS(sw0), gpios),
                       GPIO_INPUT |
                       DT_GPIO_FLAGS(DT_ALIAS(sw0), gpios));
    gpio_pin_configure(sw_device, DT_GPIO_PIN(DT_ALIAS(sw1), gpios),
                       GPIO_INPUT |
                       DT_GPIO_FLAGS(DT_ALIAS(sw1), gpios));
    gpio_pin_configure(sw_device, DT_GPIO_PIN(DT_ALIAS(sw2), gpios),
                       GPIO_INPUT |
                       DT_GPIO_FLAGS(DT_ALIAS(sw2), gpios));
    gpio_pin_configure(sw_device, DT_GPIO_PIN(DT_ALIAS(sw3), gpios),
                       GPIO_INPUT |
                       DT_GPIO_FLAGS(DT_ALIAS(sw3), gpios));
    gpio_pin_interrupt_configure(sw_device,
                                 DT_GPIO_PIN(DT_ALIAS(sw0), gpios),
                                 GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(sw_device,
                                 DT_GPIO_PIN(DT_ALIAS(sw1), gpios),
                                 GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(sw_device,
                                 DT_GPIO_PIN(DT_ALIAS(sw2), gpios),
                                 GPIO_INT_EDGE_TO_ACTIVE);
    gpio_pin_interrupt_configure(sw_device,
                                 DT_GPIO_PIN(DT_ALIAS(sw3), gpios),
                                 GPIO_INT_EDGE_TO_ACTIVE);
    gpio_init_callback(&button_cb, button_pressed,
                       BIT(DT_GPIO_PIN(DT_ALIAS(sw0), gpios)) |
                       BIT(DT_GPIO_PIN(DT_ALIAS(sw1), gpios)) |
                       BIT(DT_GPIO_PIN(DT_ALIAS(sw2), gpios)) |
                       BIT(DT_GPIO_PIN(DT_ALIAS(sw3), gpios)));
    gpio_add_callback(sw_device, &button_cb);

    /* Initialize LED's */
    init_led(0, DT_GPIO_LABEL(DT_ALIAS(led0), gpios),
             DT_GPIO_PIN(DT_ALIAS(led0), gpios),
             DT_GPIO_FLAGS(DT_ALIAS(led0), gpios));
    init_led(1, DT_GPIO_LABEL(DT_ALIAS(led1), gpios),
             DT_GPIO_PIN(DT_ALIAS(led1), gpios),
             DT_GPIO_FLAGS(DT_ALIAS(led1), gpios));
    init_led(2, DT_GPIO_LABEL(DT_ALIAS(led2), gpios),
             DT_GPIO_PIN(DT_ALIAS(led2), gpios),
             DT_GPIO_FLAGS(DT_ALIAS(led2), gpios));
    init_led(3, DT_GPIO_LABEL(DT_ALIAS(led3), gpios),
             DT_GPIO_PIN(DT_ALIAS(led3), gpios),
             DT_GPIO_FLAGS(DT_ALIAS(led3), gpios));

    /* Initialize the Bluetooth Subsystem */
    err = bt_enable(bt_ready);
    if (err) {
            printk("Bluetooth init failed (err %d)\n", err);
    }

    return 0;
  }

//==============================================================================
// dummy Arduino style loop                                              @@@1.4
//==============================================================================

void bl_core_loop(void)
{
	// nothing to do!
}

//==============================================================================
// State change                                                          @@@2.2
//==============================================================================

  void bl_core_set(BL_ob *o,int val)        // here's how we hook in ...
  {
    // id = 1..4, since it adresses elements 1..4
    // button indices are 0..3, thus swnum = o->id - 1

    if (o->id < 1 || o->id > 4)             // ignore IDs not in 1..4
      return;

    sw.sw_num = (uint8_t)(o->id-1);         // store ID (switch number)
    sw.onoff_state = (uint8_t)val;          // store value (for onoff state)

		  // finally we submit a button press work, which refreshes the
			// onoff state with the new value and publishes the state

    LOGO(3,"@core.set",o,val);              // @@@3.7
    k_work_submit(&sw.button_work);         // submit button work (that's all)
  }

//==============================================================================
// LED set                                                               @@@4.7
//==============================================================================

  void bl_core_led(BL_ob *o, int val)       // set LED on/off
  {
    LOGO(3,"@bl_core.led",o,val);                                       //@@@3.7
    if (o->id >= 1 && o->id <= 4)
    {
      struct onoff_state *p = onoff_state + (o->id-1);
      //p->current = value;
      gpio_pin_set(p->led_device, p->led_gpio_pin, val);
    }
  }

//==============================================================================
// public module interface
//==============================================================================
//
//                   +--------------------+
//                   |      BL_CORE       |
//                   +--------------------+
//                   |        SYS:        |  SYS interface
//  (v)->     INIT ->|       <out>        |  init module, store <out> callback
//  (v)->     TICK ->|      @id,cnt       |  tick module
//  (v)->     TOCK ->|      @id,cnt       |  tock module
//                   +--------------------+
//                   |       BUTTON:      |
//  (#)->   #PRESS ->|       @id,1        |  button @id press (rising edge)
//  (#)-> #RELEASE ->|       @id,0        |  button @id release (falling edge)
//  (^)<-    PRESS <-|       @id,1        |  button @id press (rising edge)
//  (^)<-  RELEASE <-|       @id,0        |  button @id release (falling edge)
//                   +--------------------+
//                   |        LED:        |  LED interface
//  (v)->      SET ->|     @id,onoff      |  set LED on/off
//  (v)->   TOGGLE ->|        @id         |  toggle LED
//                   +--------------------+
//                   |      SWITCH:       |  SWITCH interface
//  (#)->      STS ->|     @id,onoff      |  provide switch status for output
//  (^)<-      STS <-|     @id,onoff      |  output switch status
//                   +--------------------+
//                   |        SET:        |  SET interface
//  (#)->     #PRV ->|       enable       |  node provisioned
//  (#)->     #ATT ->|       enable       |  attention mode enable/disable
//  (^)<-      PRV <-|       enable       |  node provisioned
//  (^)<-      ATT <-|       enable       |  attention mode enable/disable
//                   +--------------------+
//                   |      GOOSRV:       |  GOOSRV interface
//  (#)->      STS ->|     @id,onoff      |  provide generic on/off status
//  (^)<-      SET <-|     @id,onoff      |  output generic on/off status
//                   +--------------------+
//                   |      GOOCLI:       |  GOOSRV interface
//  (#)->      SET ->|     @id,onoff      |  provide ack'ed generic on/off SET
//  (#)->      LET ->|     @id,onoff      |  provide unack'ed generic on/off SET
//  (^)<-      SET <-|     @id,onoff      |  output ack'ed generic on/off SET
//  (^)<-      LET <-|     @id,onoff      |  output unack'ed generic on/off SET
//                   +--------------------+
//
//==============================================================================

  int bl_core(BL_ob *o, int val)
  {
    static BL_fct out = NULL;

    switch (BL_ID(o->cl,o->op))
    {
      case BL_ID(_SYS,INIT_):        // [SYS:INIT <out>]
        out = o->data;               // store output callback
        return init(o,val);          // forward to init() worker

      case BL_ID(_SYS,TICK_):        // [SYS:TICK @id,cnt]
      case BL_ID(_SYS,TOCK_):        // [SYS:TOCK @id,cnt]
        return 0;                    // OK - nothing to tick/tock

      case BL_ID(_BUTTON,_PRESS_):   // [BUTTON:#PRESS @id 1] (button pressed)
      case BL_ID(_BUTTON,_RELEASE_): // [BUTTON:#RELEASE @id 0] (button release)
      case BL_ID(_SWITCH,_STS_):     // [BUTTON:#SWITCH @id,sts] (switch status)
        return bl_out(o,val,out);    // output to subscriber

      case BL_ID(_LED,SET_):         // [LED:SET @id,onoff]
      case BL_ID(_LED,TOGGLE_):      // [LED:SET @id,onoff]
//      return led(o,val);           // delegate to led() worker
				return -1;

      case BL_ID(_SET,_PRV_):        // [SET:PRV val]  (provision)
      case BL_ID(_SET,_ATT_):        // [SET:ATT val]  (attention)
        LOGO(3,"",o,val);
        return bl_out(o,val,out);    // output to subscriber

      case BL_ID(_GOOCLI,LET_):      // [GOOCLI:LET] publish unack'ed GOO msg
      case BL_ID(_GOOCLI,SET_):      // [GOOCLI:SET] publish ack'ed GOO msg
//      return bl_pub(o,val);        // publish [GOOCLI:LET] or [GOOCLI:SET] msg
        return 0;

      case BL_ID(_GOOSRV,_STS_):     // [GOOSRV:STS @id,sts] GOO status ->upward
        return bl_out(o,val,out);    // publish [GOOSRV:STS] upward

      default:
        return -1;                   // bad input
    }
  }
