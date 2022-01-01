//==============================================================================
// bltype.h - bluccino typedefs
//==============================================================================

#ifndef __BLTYPE_H__
#define __BLTYPE_H__

  #include <stdint.h>
  #include <stdbool.h>
  #include <stdlib.h>

  #define BL_PAIR(tag,op)   (((tag)<<16) | ((op)<0?-(op):(op)))
  #define BL_LENGTH(a)      (sizeof(a)/sizeof(a[0]))     // array length

     // create from given object o either OUT type init aggregate (op > 0)
     // or IN type init aggregate (op < 0)

  #define BL_OP(o)          (o->op >= 0 ? o->op : -(o->op))  // always positive
  #define BL_OUT(o)         {o->cl,+BL_OP(o),o->id,o->data}  // opcode > 0
  #define BL_IN(o)          {o->cl,-BL_OP(o),o->id,o->data}  // opcode < 0

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

#endif // __BLTYPE_H__
