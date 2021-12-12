//==============================================================================
// blapi.c - Bluccino API layer
//==============================================================================
//
// The whole concept is based that we have Bluccino objects which control
// access to network interfaces and resources. Resource a grouped into the
// following categories:
//
//   1) Hardware
//        - general hardware control (e.g. low power control, watchdogs)
//        - LEDs
//        - buttons and/or switches
//        - General purpose I/O (GPIO)
//        - Timers
//        - Non-Volatile memory (NVM)
//   2) BLE System
//        - general BLE control (e.g. low power control of BLE sub units)
//        - advertiser
//        - scanner
//        - characteristics
//   3) MESH System
//        - general Mesh control (e.g. provisioning, attentioning)
//        - Mesh models (a kind of smart interfaces for mesh communication)
//          like client models (which) have no state and server models which
//          have state.
//
//   Each resource can have a state and for each resource there are certain
//   operations defined on it. Consider the following simple example.
//
//    +-----------+    +-----------+          +-----------+    +-----------+
//    |           |    |           |          |           |    |           |
//    +  SWITCH   +===>+  SENDER   +--------->+ RECEIVER  +===>+    LED    +
//    |           |    |           |          |           |    |           |
//    +-----------+    +-----------+          +-----------+    +-----------+
//
//   Resources can be treated as class instance objects of the different re-
//   source classes
//      :     :
//
//   Operations
//   ==========
//
//    Operation    SWITCH          CLIENT           SERVER         LED
//   -----------------------------------------------------------------------
//                  event        request to          event        change
//     SET        on state       change ser-       on state        LED
//                 change        ver's state        change        state
//
//==============================================================================

#include "blapi.h"
#include "blcore.h"
#include "bllog.h"

  static BL_fct notify = NULL;

  static bool provisioned = 0;
  static bool attention = 0;

//==============================================================================
// notification entry point
//==============================================================================

  void bl_event(BL_obj *o, int value)
  {
    switch (o->op)                     // dispatch event
    {
      case BL_PRV:                     // provisioned state changed
        provisioned = value;
        bl_log_color(attention,provisioned);
        bl_log(2,"@provisioned",0,provisioned);
        break;

      case BL_ATT:                     // attention state changed
        attention = value;
        bl_log_color(attention,provisioned);
        bl_log(2,"@attention",0,attention);
        break;

      default:
        break;
    }

    if (notify)
      notify(o,value);                 // forward event to app level
  }

//==============================================================================
// set/get function (sate access)
//==============================================================================

  void bl_set(BL_obj *o, int value)
  {
    bl_log(1,"@set", o->id,value);
    bl_core_set(o,value);
  }

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_fct cb, int verbose)
  {
    notify = cb;
    bl_verbose(verbose);
    bl_core_init();
  }

  void bl_loop(void)
  {
    bl_core_loop();
  }
