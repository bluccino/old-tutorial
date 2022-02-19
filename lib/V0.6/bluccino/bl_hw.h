//==============================================================================
// bl_hw.h - bluccino HW (syntactic sugar)
//==============================================================================

#ifndef __BL_HW_H__
#define __BL_HW_H__

//==============================================================================
// syntactic sugar: turn LED(@id) on/off  (send [LED:SET @id,onoff] message)
// - usage: changed = bl_switch(o)
//==============================================================================

  static inline int bl_led(int id,  bool onoff)
  {
    BL_ob oo = {_LED,SET_,id,NULL};    // [LED:SET @id,onoff]
    return bl_down(&oo,onoff);
  }

//==============================================================================
// syntactic sugar: identify a button press event ([BUTTON:PRESS @id,val])
// - usage: changed = bl_press(o)
//==============================================================================

  static inline int bl_press(BL_ob *o)
  {
    return (bl_id(o) == BL_ID(_BUTTON,PRESS_));  // [BUTTON:PRESS @id,val]
  }

//==============================================================================
// syntactic sugar: identify a switch status event ([SWITCH:STS @id,val])
// - usage: changed = bl_switch(o)
//==============================================================================

  static inline int bl_switch(BL_ob *o)
  {
    return (bl_id(o) == BL_ID(_SWITCH,STS_));    // [SWITCH:STS @id,val]
  }

#endif // __BL_HW_H__
