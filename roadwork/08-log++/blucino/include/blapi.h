//==============================================================================
// bluccino.h - Bluccino API
//==============================================================================

#ifndef __BLAPI_H__
#define __BLAPI_H__

#include <zephyr.h>

   extern bool bl_attention;           // attention mode
   extern bool bl_provisioned;         // provisioned mode

//==============================================================================
// mesh opcodes
//==============================================================================

  #define BL_GOOLET 0x8203

//==============================================================================
// typedefs
//==============================================================================

    typedef uint8_t BL_byte;

    typedef const char *BL_txt;

      // we define ZL_ms to represent miliseconds since system start or clock
      // restart in 64-bit signed integer representation. This allows
      //
      //     a) negative time stamps to indicate invalid time stamps
      //     b) sufficient long time () before overrun
      //        (1 year .= 2^35 ms, 2^28 years .= 268 million years = 2^63 ms)

    typedef int64_t BL_ms;                  // mili seconds

//==============================================================================
// object and event definitions
//==============================================================================

  typedef enum BL_tag                  // class tag
          {
            BL_SYS,                    // system

            BL_MESH,                   // mesh
            BL_GOOCLI,                 // GOO Client
            BL_GOOSRV,                 // GOO Server

            BL_BUTTON,                 // button event
            BL_TIMER,                  // timer event

            BL_ADV,                    // advertising
            BL_SCAN,                   // scanning

            BL_USER1,                  // user 1
            BL_USER2,                  // user 2
            BL_USER3,                  // user 3
            BL_USER4,                  // user 4
          } BL_tag;

  typedef enum BL_op
          {
            BL_INIT,                   // init function
            BL_SET,                    // acknowledged set operation
            BL_LET,                    // unacknowledged
            BL_GET,                    // ask for status
            BL_STS,                    // send status
            BL_PRV,                    // provisioning state changed
            BL_ATT,                    // attention state change
          } BL_op;

  typedef struct BL_ob
          {
            int tag;                   // class tag
            int op;                    // opcode
            int id;                    // object ID
            void *data;                // pointer to data
          } BL_obj;

  typedef void (*BL_fct)(BL_obj *o, int value);

//==============================================================================
// issuing an event
//==============================================================================

  void bl_event(BL_obj *o, int value); // issue an event

//==============================================================================
// state access
//==============================================================================

  void bl_set(BL_obj *o, int value);

//==============================================================================
// obligatory init and loop functions
//==============================================================================

  void bl_init(BL_fct cb, int verbose);
  void bl_loop(void);

#endif // __BLAPI_H__
