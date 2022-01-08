//==============================================================================
// blmsg.h - Bluccino message definitions
//==============================================================================

#ifndef __BLMSG_H__
#define __BLMSG_H__

//==============================================================================
// message class definitions
//==============================================================================

  #ifndef BL_CL_TEXT

    #define BL_CL_TEXT {"VOID","SYS","RESET","TIMER","TEST","SET","GET","MESH",\
                        "BLE","NGMN","CFGSRV","CFGCLI","HEASRV","HEACLI", \
                        "GOOCLI","GOOSRV","GLVCLI","GLVSRV", \
                        "BUTTON", "SWITCH", "LED",           \
                        "CTRL","SCAN","ADVT","SOS"}

    typedef enum BL_cl                   // class tag
            {
              CL_VOID,                   // invalid
              CL_SYS,                    // system
              CL_RESET,                  // reset
              CL_TIMER,                  // timer module
              CL_TEST,                   // test class

              CL_SET,                    // set property
              CL_GET,                    // get property

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

  #define _VOID    CL_VOID               // void
  #define _SYS     CL_SYS                // system
  #define _RESET   CL_RESET              // reset
  #define _TIMER   CL_TIMER              // timer module
  #define _TEST    CL_TEST               // test class

  #define _SET     CL_SET                // set property
  #define _GET     CL_GET                // get property

  #define _MESH    CL_MESH               // mesh
  #define _BLE     CL_BLE                // BLE
  #define _NGMN    CL_NGMN               // 3G/4G/5G (next gen mobile networks)

  #define _CFGSRV  CL_CFGSRV             // Config Server
  #define _CFGCLI  CL_CFGCLI             // Config Client
  #define _HEASRV  CL_HEASRV             // Health Server
  #define _HEACLI  CL_HEACLI             // Health Client

  #define _GOOCLI  CL_GOOCLI             // Generic OnOff Client
  #define _GOOSRV  CL_GOOSRV             // Generic OnOff Server
  #define _GLVCLI  CL_GLVCLI             // Generic Level Client
  #define _GLVSRV  CL_GLVSRV             // Generic Level Server

  #define _BUTTON  CL_BUTTON             // button
  #define _SWITCH  CL_SWITCH             // switch
  #define _LED     CL_LED                // LED

  #define _CTRL    CL_CTRL               // Generic controller
  #define _SCAN    CL_SCAN               // scanning
  #define _ADVT    CL_ADVT               // advertizer
  #define _SOS     CL_SOS                // SOS module

//==============================================================================
// message opcodes
//==============================================================================

  #ifndef BL_OP_TEXT

    #define BL_OP_TEXT {"VOID","INIT","WHEN","USE","READY","BUSY", \
                        "PRV","ATT","DUE","SET","LET",   \
                        "GET","STS","TRIG","TICK","TOCK","CMD","VAL","LEVEL", \
                        "ONOFF","COUNT","TOGGLE","INC","DEC", \
                        "PAY", "ADV","BEACON","SEND","PRESS","RELEASE"}

    typedef enum BL_op
            {
              OP_VOID,                   // invalid opcode (0)
              OP_INIT,                   // init function
              OP_WHEN,                   // provide module handling callback
              OP_USE,                    // can module be used

              OP_READY,                  // ready state
              OP_BUSY,                   // busy state
              OP_PRV,                    // provisioning state changed
              OP_ATT,                    // attention state change
              OP_DUE,                    // timer is due

              OP_SET,                    // acknowledged set operation
              OP_LET,                    // unacknowledged
              OP_GET,                    // ask for status
              OP_STS,                    // send status

              OP_TRIG,                   // generic trigger signal
              OP_TICK,                   // timer ticks
              OP_TOCK,                   // timer tocks
              OP_CMD,                    // generic command
              OP_VAL,                    // generic value
              OP_LEVEL,                  // generic level
              OP_ONOFF,                  // generic onoff command
              OP_COUNT,                  // counter
              OP_TOGGLE,                 // toggle state

              OP_INC,                    // increment
              OP_DEC,                    // decrement

              OP_PAY,                    // generic payload
              OP_ADV,                    // LLL advertising reports
              OP_BEACON,                 // LLL beacon packet reports

              OP_SEND,                   // send a message

              OP_PRESS,                  // button press
              OP_RELEASE,                // button release
            } BL_op;

  #endif // BL_OP_TEXT

  #define VOID_    OP_VOID               // invalid opcode (0)
  #define INIT_    OP_INIT               // init function
  #define WHEN_    OP_WHEN               // provide module handling callback
  #define USE_     OP_USE                // can module be used

  #define READY_   OP_READY              // continue when Bluetooth ready
  #define BUSY_    OP_BUSY               // continue when Bluetooth ready
  #define PRV_     OP_PRV                // provisioning state changed
  #define ATT_     OP_ATT                // attention state change
  #define DUE_     OP_DUE                // timer is due

  #define SET_     OP_SET                // acknowledged set operation
  #define LET_     OP_LET                // unacknowledged
  #define GET_     OP_GET                // ask for status
  #define STS_     OP_STS                // send status

  #define TRIG_    OP_TRIG               // generic trigger signal
  #define TICK_    OP_TICK               // timer ticks
  #define TOCK_    OP_TOCK               // timer tocks
  #define CMD_     OP_CMD                // generic command
  #define VAL_     OP_VAL                // generic value
  #define LEVEL_   OP_LEVEL              // generic level
  #define ONOFF_   OP_ONOFF              // generic onoff command
  #define COUNT_   OP_COUNT              // counter
  #define TOGGLE_  OP_TOGGLE             // toggle state

  #define INC_     OP_INC                // increment
  #define DEC_     OP_DEC                // decrement

  #define PAY_     OP_PAY                // generic payload
  #define ADV_     OP_ADV                // LLL advertising reports
  #define BEACON_  OP_BEACON             // LLL beacon packet reports

  #define SEND_    OP_SEND               // send a message

  #define PRESS_   OP_PRESS              // button press
  #define RELEASE_ OP_RELEASE            // button release

//==============================================================================
// message object & message callback definition
//==============================================================================

  typedef struct BL_ob
          {
            BL_cl cl;                  // class tag
            BL_op op;                  // opcode
            int id;                    // object ID
            const void *data;          // pointer to data
          } BL_ob;

  typedef int (*BL_fct)(BL_ob *o, int value);

#endif // __BLMSG_H__
