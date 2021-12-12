//==============================================================================
// bluccino.c - Bluccino API layer
//==============================================================================

#include "bluccino.h"
#include "blcore.h"

  static BL_fct notification = NULL;

//==============================================================================
// set/get function (sate access)
//==============================================================================

  void bl_set(BL_obj *o, int value)
  {
    printk("  set(%d,%d)\n", o->id,value);
    bl_core_set(o,value);
  }

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_fct notify, int verbose)
  {
    notification = notify;
    bl_core_init();
  }

  void bl_loop(void)
  {
    bl_core_loop();
  }
