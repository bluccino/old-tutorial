// bccore.h - mesh/HW core interface

#ifndef __BLCORE_H__
#define __BLCORE_H__

#include <zephyr.h>

  void bl_core_set(BL_obj *o, int value);

  void bl_core_init(void);
  void bl_core_loop(void);

#endif // __BLCORE_H__
