//==============================================================================
// blmsg.h - Bluccino message definitions
//==============================================================================

#ifndef __BLMSG_H__
#define __BLMSG_H__

//==============================================================================
// message class definitions
//==============================================================================

  #ifndef BL_CL_TEXT

    #define BL_CL_TEXT {"VOID","SYS","HDL","RESET","TIMER","TEST","SET","GET",\
                        "MESH","BLE","NGMN","CFGSRV","CFGCLI",\
                        "HEASRV","HEACLI","GOOCLI","GOOSRV","GLVCLI","GLVSRV",\
                        "BUTTON", "SWITCH", "LED",           \
                        "CTRL","SCAN","ADVT","SOS"}

    typedef enum BL_cl                 // class tag
            {
              _VOID,                   // invalid
              _SYS,                    // system
              _HDL,                    // handle a message
              _RESET,                  // reset
              _TIMER,                  // timer module
              _TEST,                   // test class

              _SET,                    // set property
              _GET,                    // get property

              _MESH,                   // mesh
              _BLE,                    // BLE
              _NGMN,                   // 3G/4G/5G (next gen mobile networks)

              _CFGSRV,                 // Config Server
              _CFGCLI,                 // Config Client
              _HEASRV,                 // Health Server
              _HEACLI,                 // Health Client

              _GOOCLI,                 // Generic OnOff Client
              _GOOSRV,                 // Generic OnOff Server
              _GLVCLI,                 // Generic Level Client
              _GLVSRV,                 // Generic Level Server

              _BUTTON,                 // button
              _SWITCH,                 // switch
              _LED,                    // LED

              _CTRL,                   // Generic controller
              _SCAN,                   // scanning
              _ADVT,                   // advertizer
              _SOS,                    // SOS module
            } BL_cl;                     // class tag

  #endif // BL_CL_TEXT

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
              VOID_,                   // invalid opcode (0)
              INIT_,                   // init function
              WHEN_,                   // provide module handling callback
              USE_,                    // can module be used

              READY_,                  // ready state
              BUSY_,                   // busy state
              PRV_,                    // provisioning state changed
              ATT_,                    // attention state change
              DUE_,                    // timer is due

              SET_,                    // acknowledged set operation
              LET_,                    // unacknowledged
              GET_,                    // ask for status
              STS_,                    // send status

              TRIG_,                   // generic trigger signal
              TICK_,                   // timer ticks
              TOCK_,                   // timer tocks
              CMD_,                    // generic command
              VAL_,                    // generic value
              LEVEL_,                  // generic level
              ONOFF_,                  // generic onoff command
              COUNT_,                  // counter
              TOGGLE_,                 // toggle state

              INC_,                    // increment
              DEC_,                    // decrement

              PAY_,                    // generic payload
              ADV_,                    // LLL advertising reports
              BEACON_,                 // LLL beacon packet reports

              SEND_,                   // send a message

              PRESS_,                  // button press
              RELEASE_,                // button release
            } BL_op;

  #endif // BL_OP_TEXT

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
