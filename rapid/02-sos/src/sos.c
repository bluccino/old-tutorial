//==============================================================================
// sos.c (SOS module)
//==============================================================================

  #include "bluccino.h"
  #include "sos.h"

//==============================================================================
// notify callback
//==============================================================================

  static BL_fct notify = NULL;
  BL_ob oo = {CL_SOS,OP_LEVEL,0,NULL}; // output messaging object

//==============================================================================
// public module interface
//==============================================================================

  int sos(BL_ob *o, int val)           // SOS dummy interface
  {
    bl_logo(2,BL_M "sos",o,val);

    switch (o->op)
    {
      case OP_INIT:
        oo.id = o->id;                 // store LED id in messaging object
        notify = o->data;              // output message subscriber callback
        return 0;

      case OP_TICK:
      {
        int level = val % 2;           // binary level output sequence (toggling)
        bl_out(&oo,level,notify);      // output [SOS:LEVEL @id,level] to subscriber
      }
      default:
        return -1;                     // bad command
    }
  }
