//==============================================================================
// bluccino.h - Bluccino API
//==============================================================================

#ifndef __BLUCCINO_H__
#define __BLUCCINO_H__

#include <zephyr.h>

//==============================================================================
// mesh opcodes
//==============================================================================

  #define BL_GOOLET 0x8203

//==============================================================================
// object and event definitions
//==============================================================================

  typedef enum BL_event                // event type
          {
            BL_MESH,                   // mesh event
            BL_BUTTON,                 // button event
            BL_TIMER,                  // timer event
            BL_USER,                   // user event
          } BL_event;

  typedef struct BL_obj
          {
            int id;                    // object ID
            BL_event evt;              // event type
            int op;                    // opcode
            void *data;
          } BL_obj;

  typedef void (*BL_notify)(BL_obj *o, int value);

//==============================================================================
// state access
//==============================================================================

  void bl_set(BL_obj *o, int value);

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_notify notify, int verbose);
  void bl_loop(void);

#endif // __BLUCCINO_H__
