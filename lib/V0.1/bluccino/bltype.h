//==============================================================================
// bltype.h - Bluccino typedefs
//==============================================================================

#ifndef __BLTYPE_H__
#define __BLTYPE_H__

  #include <stdarg.h>
  #include <stdint.h>
  #include <stdbool.h>
  #include <stdlib.h>

  #define BL_PAIR(cl,op)    (((cl)<<16) | ((op) & 0xffff))
  #define BL_MSG(o)         BL_PAIR(o->cl,o->op)

  #define BL_LENGTH(a)      (sizeof(a)/sizeof(a[0]))     // array length

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
    typedef int64_t BL_us;                  // micro seconds

//==============================================================================
// object class definitions
//==============================================================================

#ifndef BL_CL_TEXT

  #define BL_CL_TEXT {"SYS","TIMER","MESH","BLE","NGMN",   \
                      "CFGSRV","CFGCLI","HEASRV","HEACLI", \
                      "GOOCLI","GOOSRV","GLVCLI","GLVSRV", \
                      "BUTTON", "SWITCH", "LED",           \
                      "CTRL","SCAN","ADVT","SOS"}

  typedef enum BL_cl                   // class tag
          {
            CL_SYS,                    // system
            CL_TIMER,                  // timer module

            CL_MESH,                   // mesh
            CL_BLE,                    // BLE
            CL_NGMN,                   // 3G/4G/5G (next gen mobile networks)

            CL_CFGSRV,                 // Config Server
            CL_CFGCLI,                 // Config Client
            CL_HEASRV,                 // Health Server
            CL_HEACLI,                 // Health Client

            CL_GOOCLI,                 // Generic OnOff Client
            CL_GOOSRV,                 // Generic OnOff Server
            CL_GLVCLI,                 // Generic Level Client
            CL_GLVSRV,                 // Generic Level Server

            CL_BUTTON,                 // button
            CL_SWITCH,                 // switch
            CL_LED,                    // LED

            CL_CTRL,                   // Generic controller
            CL_SCAN,                   // scanning
            CL_ADVT,                   // advertizer
            CL_SOS,                    // SOS module
          } BL_cl;                     // class tag

#endif // BL_CL_TEXT

//==============================================================================
// message opcodes
//==============================================================================

#ifndef BL_OP_TEXT

  #define BL_OP_TEXT {"VOID","INIT","SET","LET","GET","STS","PRV","ATT","TRIG",\
                      "TICK","CMD","VAL","LEVEL","ONOFF", \
                      "PAY", "ADV","BEACON","SEND","PRESS","RELEASE"}

  typedef enum BL_op
          {
            OP_VOID,                   // invalid opcode (0)

            OP_INIT,                   // init function
            OP_SET,                    // acknowledged set operation
            OP_LET,                    // unacknowledged
            OP_GET,                    // ask for status
            OP_STS,                    // send status

            OP_PRV,                    // provisioning state changed
            OP_ATT,                    // attention state change

            OP_TRIG,                   // generic trigger signal
            OP_TICK,                   // timer ticks
            OP_CMD,                    // generic command
            OP_VAL,                    // generic value
            OP_LEVEL,                  // generic level
            OP_ONOFF,                  // generic onoff command

            OP_PAY,                    // generic payload
            OP_ADV,                    // LLL advertising reports
            OP_BEACON,                 // LLL beacon packet reports

            OP_SEND,                   // send a message

            OP_PRESS,                  // button press
            OP_RELEASE,                // button release
          } BL_op;

#endif // BL_OP_TEXT

//==============================================================================
// message objects
//==============================================================================

  typedef struct BL_ob
          {
            BL_cl cl;                  // class tag
            int op;                    // opcode
            int id;                    // object ID
            void *data;                // pointer to data
          } BL_ob;

  typedef int (*BL_notify)(BL_ob *o, int value);

#endif // __BLTYPE_H__
