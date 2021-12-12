//==============================================================================
// bltype.h - Bluccino typedefs
//==============================================================================

#ifndef __BLTYPE_H__
#define __BLTYPE_H__

  #include <stdarg.h>
  #include <stdint.h>
  #include <stdbool.h>
  #include <stdlib.h>

  #include "blmsg.h"

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
// message objects
//==============================================================================

  typedef struct BL_ob
          {
            BL_cl cl;                  // class tag
            int op;                    // opcode
            int id;                    // object ID
            void *data;                // pointer to data
          } BL_ob;

  typedef int (*BL_fct)(BL_ob *o, int value);

#endif // __BLTYPE_H__
