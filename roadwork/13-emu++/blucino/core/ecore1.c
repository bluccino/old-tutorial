//==============================================================================
// ecore1.c - emulation core, version 1
//==============================================================================

#include "blapi.h"
#include "blcore.h"
#include "bllog.h"

#define LENGTH(s) (sizeof(s)/sizeof(s[0]))

  static bool led[]   = {0,0,0,0};
  static bool state[] = {0,0,0,0};

//==============================================================================
// GOOSRV Let
//==============================================================================

  static struct goosrv
				        {
									BL_ob o;
									int value;
									struct k_work work;
								} goosrv;

  static void goosrv_worker(struct k_work *work)
  {
	  BL_ob o = {BL_GOOSRV, goosrv.o.op, goosrv.o.id, NULL};
	  bl_event(&o,goosrv.value);
  }

  static void goosrv_let(BL_ob *o, int value)
	{
    bl_log2(3,"@goosrv.let",o->id,value);
		goosrv.o.tag = BL_LET;
		goosrv.o.op = o->op;
		goosrv.o.id = o->id;
		k_work_submit(&goosrv.work);
	}


//==============================================================================
// GOOCLI Let
//==============================================================================

  void goocli_let(BL_ob *o, int value)
	{
		if (o->id >= 1 && o->id <= LENGTH(state))          // ID out of range
		{
      bl_log2(3,"@goocli.let",o->id,value);
			goosrv_let(o,value);
		}
	}

//==============================================================================
// provisioning link open/close callbacks
//==============================================================================
/*
	static void prov_complete(uint16_t net_idx, uint16_t addr)
	{
		BL_ob o = {BL_MESH, BL_PRV, 0, NULL};
		bl_event(&o,1);
	}

	static void prov_reset(void)
	{
		BL_ob o = {BL_MESH, BL_PRV, 0, NULL};
		bl_event(&o,0);
	}

	static void link_open(bt_mesh_prov_bearer_t bearer)
	{
		BL_ob o = {BL_MESH, BL_ATT, 0, NULL};
		bl_event(&o,1);
	}

	static void link_close(bt_mesh_prov_bearer_t bearer)
	{
		BL_ob o = {BL_MESH, BL_ATT, 0, NULL};
		bl_event(&o,0);
	}
*/
//==============================================================================
// obligatory init and loop functions
//==============================================================================

	void bl_core_init(void)
	{
		printk("Initializing...\n");

  	k_work_init(&goosrv.work, goosrv_worker);

		for (int i=0; i < LENGTH(state); i++)
		   led[i] = state[i] = 0;
	}

	void bl_core_loop(void)
	{
		// nothing to do!
	}

//==============================================================================
// State change
//==============================================================================

  void bl_core_set(BL_ob *o,int value) // here's how we hook in ...
	{
    bl_log2(3,"@core.set",o->id,value);

    if (o->id >= 1 && o->id <= LENGTH(state)) // ignore IDs not in 1..4
		  state[o->id-1] = (value != 0);          // store value (for onoff state)
	}

//==============================================================================
// LED set
//==============================================================================

  void bl_core_led(BL_ob *o, int value)     // set LED on/off
  {
    bl_log2(3,"@core.led",o->id,value);

  	if (o->id >= 1 && o->id <= LENGTH(led))
		  led[o->id-1] = (value != 0);
  }

//==============================================================================
// Button press
//==============================================================================

  void bl_core_button(BL_ob *o, int value)  // button press emulation
  {
    int id = o->id;
    bl_log2(3,"@core.button",id,value);

  	if (id >= 1 && id <= 4)
    {
		  BL_ob o = {BL_BUTTON,BL_SET,id,NULL};
      bl_event(&o,value);
    }
  }
