//==============================================================================
// blmsg.h - Bluccino message definitions
//==============================================================================

#ifndef __BLMSG_H__
#define __BLMSG_H__

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

    #define BL_OP_TEXT {"VOID","INIT","LOOP","PRV","ATT","SET","LET","GET", \
                        "STS","TRIG","TICK","CMD","VAL","LEVEL","ONOFF",    \
                        "PAY", "ADV","BEACON","SEND","PRESS","RELEASE"}

    typedef enum BL_op
            {
              OP_VOID,                   // invalid opcode (0)
              OP_INIT,                   // init function
              OP_LOOP,                   // loop function

              OP_PRV,                    // provisioning state changed
              OP_ATT,                    // attention state change

              OP_SET,                    // acknowledged set operation
              OP_LET,                    // unacknowledged
              OP_GET,                    // ask for status
              OP_STS,                    // send status

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

#endif // __BLMSG_H__
