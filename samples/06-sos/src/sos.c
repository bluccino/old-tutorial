//==============================================================================
// sos.c - an SOS sequencer block
//==============================================================================

#include "bluccino.h"
#include "sos.h"

  static BL_fct notify = NULL;

//==============================================================================
// timer tick: output sequenced [SOS:ONOFF] messages
//==============================================================================

  #define PATTERN "* * *  *** *** ***  * * *   ";

  static int tick(BL_ob *o, int val)
  {
    static BL_txt p = NULL;
    static bool last = 0;

    if (!p || *p == 0)
      p = PATTERN;                     // reset pointer to pattern begin

    val = (*p++ != ' ');               // next value to display
    if (val != last)                   // only if tick changes
    {
      BL_ob oo = {CL_SOS,OP_LEVEL,0,NULL};
      bl_out(&oo,val,notify);
      last = val;
    }

    return val;
  }

//==============================================================================
// input interface
//==============================================================================

  int sos_in(BL_ob *o, int val)
  {
    switch (o->op)
    {
      case OP_TICK:
      {
        bl_logo(2,"sos",o,val);
        return tick(o,val);
      }
    }
    return val;
  }

//==============================================================================
// mandatory init/loop functions
//==============================================================================

  void sos_init(BL_fct cb)
  {
    notify = cb;
  }

  void sos_loop(void)
  {
    // nothing to do
  }
