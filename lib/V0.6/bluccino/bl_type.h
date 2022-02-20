//==============================================================================
// bl_type.h - bluccino typedefs
//==============================================================================

#ifndef __BL_TYPE_H__
#define __BL_TYPE_H__

  #include <stdint.h>
  #include <stdbool.h>
  #include <stdlib.h>

  #define BL_ID(cl,op)      (((cl)<<16)|((op)<0?-(op):(op))) // message ID
  #define BL_PAIR(cl,op)    BL_ID(cl,op)                     // message ID
  #define BL_LENGTH(a)      (sizeof(a)/sizeof(a[0]))         // array length
  #define BL_LEN(a)         (sizeof(a)/sizeof(a[0]))         // array length

    // useful macros for min(), max() and abs()

  #define BL_MAX(x,y) (((x) > (y)) ? (x) : (y))
  #define BL_MIN(x,y) (((x) < (y)) ? (x) : (y))
  #define BL_ABS(x)   ((x) < 0    ? -(x) : (x))

     // create from given object o either OUT type init aggregate (op > 0)
     // or IN type init aggregate (op < 0)

  #define BL_OP(o)          (o->op >= 0 ? o->op : -(o->op))  // always positive
  #define BL_OUT(o)         {o->cl,+BL_OP(o),o->id,o->data}  // opcode > 0
  #define BL_IN(o)          {o->cl,-BL_OP(o),o->id,o->data}  // opcode < 0

//==============================================================================
// typedefs
//==============================================================================

  typedef uint8_t BL_byte;

  typedef int8_t   BL_s8;               // we like short type identifiers :-)
  typedef int16_t  BL_s16;              // we like short type identifiers :-)
  typedef int32_t  BL_s32;              // we like short type identifiers :-)
  typedef int64_t  BL_s64;              // we like short type identifiers :-)

  typedef uint8_t  BL_u8;               // we like short type identifiers :-)
  typedef uint16_t BL_u16;              // we like short type identifiers :-)
  typedef uint32_t BL_u32;              // we like short type identifiers :-)
  typedef uint64_t BL_u64;              // we like short type identifiers :-)

  typedef const char *BL_txt;

    // we define ZL_ms to represent miliseconds since system start or clock
    // restart in 64-bit signed integer representation. This allows
    //
    //     a) negative time stamps to indicate invalid time stamps
    //     b) sufficient long time () before overrun
    //        (1 year .= 2^35 ms, 2^28 years .= 268 million years = 2^63 ms)

  typedef int64_t BL_ms;               // mili seconds
  typedef int64_t BL_us;               // micro seconds

  typedef enum
  {
      BL_ERR_ANY     = 1100,           // any error
      BL_ERR_BADARG  = 1200,           // bad input arg
      BL_ERR_FAILED  = 1300,           // operation failed
      BL_ERR_MEMORY  = 1400,           // out of memory
  } BL_err;

  typedef struct BL_pace               // tick/tock pace control
          {
            BL_ms start;               // start time frame for pace maker
            BL_ms clock;               // pace clock
            BL_ms period;              // tick period
            int divider;               // tock divider
          } BL_pace;                   // tick/tock pace control

#endif // __BL_TYPE_H__