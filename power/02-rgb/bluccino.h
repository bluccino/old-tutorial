// bluccino.h

  #include <stdio.h>

  typedef enum {CL_CTRL,CL_LED} BL_cl;
  typedef enum {OP_INIT,OP_SET} BL_op;

  typedef struct BL_ob                 // Bluccino messaging object
  {
    BL_cl cl;                          // message interface class
    BL_op op;                          // message opcode
    int id;                            // channel ID
    void *data;                        // data for extended messages
  } BL_ob;

  #define OVAL   BL_ob *o, int val
  #define CL_PAIR(cl,op) ((cl<<16) | (op)) // pair of interface class & opcode
