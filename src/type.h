/* @page type.h
 *       Structured and union types
 *       2020
 */

#ifndef TYPE_H_
#define TYPE_H_

#include <stdint.h>


/** @def ID of types
 */
#define TYPE_BYTE         static_cast<uint8_t>(1)
#define TYPE_WORD         static_cast<uint8_t>(2)
#define TYPE_DWORD        static_cast<uint8_t>(3)
#define TYPE_LWORD        static_cast<uint8_t>(4)
#define TYPE_FLOAT        static_cast<uint8_t>(5)
#define TYPE_DOUBLE       static_cast<uint8_t>(6)
#define TYPE_SINT         static_cast<uint8_t>(7)
#define TYPE_INT          static_cast<uint8_t>(8)
#define TYPE_DINT         static_cast<uint8_t>(9)
#define TYPE_LINT         static_cast<uint8_t>(10)
#define TYPE_BOOL         TYPE_BYTE
#define TYPE_UINT         TYPE_WORD
#define TYPE_UDINT        TYPE_DWORD
#define TYPE_ULINT        TYPE_LWORD
#define TYPE_REAL         TYPE_FLOAT
#define TYPE_LREAL        TYPE_DOUBLE


/** @def Size of types
 *       in bytes
 */
#define TYPE_BYTE_SZ      static_cast<uint8_t>(sizeof(uint8_t))
#define TYPE_BOOL_SZ      TYPE_BYTE_SZ
#define TYPE_SINT_SZ      static_cast<uint8_t>(sizeof(int8_t))

#define TYPE_WORD_SZ      static_cast<uint8_t>(sizeof(uint16_t))
#define TYPE_UINT_SZ      TYPE_WORD_SZ
#define TYPE_INT_SZ       static_cast<uint8_t>(sizeof(int16_t))

#define TYPE_DWORD_SZ     static_cast<uint8_t>(sizeof(uint32_t))
#define TYPE_UDINT_SZ     TYPE_DWORD_SZ
#define TYPE_DINT_SZ      static_cast<uint8_t>(sizeof(int32_t))

#define TYPE_FLOAT_SZ     static_cast<uint8_t>(sizeof(float))
#define TYPE_REAL_SZ      TYPE_FLOAT_SZ

#define TYPE_LWORD_SZ     static_cast<uint8_t>(sizeof(uint64_t))
#define TYPE_ULINT_SZ     TYPE_LWORD_SZ
#define TYPE_LINT_SZ      static_cast<uint8_t>(sizeof(int64_t))

#define TYPE_DOUBLE_SZ    static_cast<uint8_t>(sizeof(double))
#define TYPE_LREAL_SZ     TYPE_DOUBLE_SZ


/** @def Size of types
 *       in words
 */
#define TYPE_BYTE_WSZ      static_cast<uint8_t>(1)
#define TYPE_BOOL_WSZ      TYPE_BYTE_WSZ
#define TYPE_SINT_WSZ      static_cast<uint8_t>(1)

#define TYPE_WORD_WSZ      static_cast<uint8_t>(1)
#define TYPE_UINT_WSZ      static_cast<uint8_t>(1)
#define TYPE_INT_WSZ       static_cast<uint8_t>(1)

#define TYPE_DWORD_WSZ     static_cast<uint8_t>((TYPE_DWORD_SZ/TYPE_WORD_SZ))
#define TYPE_UDINT_WSZ     TYPE_DWORD_WSZ
#define TYPE_DINT_WSZ      static_cast<uint8_t>((TYPE_DINT_SZ/TYPE_WORD_SZ))
#define TYPE_FLOAT_WSZ     static_cast<uint8_t>((TYPE_FLOAT_SZ/TYPE_WORD_SZ))
#define TYPE_REAL_WSZ      TYPE_FLOAT_WSZ

#define TYPE_LWORD_WSZ     static_cast<uint8_t>((TYPE_LWORD_SZ/TYPE_WORD_SZ))
#define TYPE_ULINT_WSZ     TYPE_LWORD_WSZ
#define TYPE_LINT_WSZ      static_cast<uint8_t>((TYPE_LINT_SZ/TYPE_WORD_SZ))
#define TYPE_DOUBLE_WSZ    static_cast<uint8_t>((TYPE_DOUBLE_SZ/TYPE_WORD_SZ))
#define TYPE_LREAL_WSZ     TYPE_DOUBLE_WSZ


/** @typedef Union type
 *           DOUBLE > BYTES
 */
typedef union {
    double   data;
    uint8_t  bytes[TYPE_DOUBLE_SZ];
} DOUBLE_ubt;

/** @typedef Union type
 *           DOUBLE > WORDS
 */
typedef union {
    double   data;
    uint16_t words[TYPE_DOUBLE_WSZ];
} DOUBLE_uwt;


/** @typedef Union type
 *           LINT > BYTES
 */
typedef union {
    int64_t data;
    uint8_t bytes[TYPE_LWORD_SZ];
} LINT_ubt;

/** @typedef Union type
 *           LINT > WORDS
 */
typedef union {
    int64_t  data;
    uint16_t words[TYPE_LWORD_WSZ];
} LINT_uwt;


/** @typedef Union type
 *           LWORD > BYTES
 */
typedef union {
    uint64_t data;
    uint8_t  bytes[TYPE_LWORD_SZ];
} LWORD_ubt;

/** @typedef Union type
 *           LWORD > WORDS
 */
typedef union {
    uint64_t data;
    uint16_t words[TYPE_LWORD_WSZ];
} LWORD_uwt;


/** @typedef Union type
 *           FLOAT > BYTES
 */
typedef union {
    float    data;
    uint8_t  bytes[TYPE_FLOAT_SZ];
} FLOAT_ubt;

/** @typedef Union type
 *           FLOAT > WORDS
 */
typedef union {
    float    data;
    uint16_t words[TYPE_FLOAT_WSZ];
} FLOAT_uwt;


/** @typedef Union type
 *           DINT > BYTES
 */
typedef union {
    int32_t data;
    uint8_t bytes[TYPE_DINT_SZ];
} DINT_ubt;

/** @typedef Union type
 *           DINT > WORDS
 */
typedef union {
    int32_t  data;
    uint16_t words[TYPE_DINT_WSZ];
} DINT_uwt;


/** @typedef Union type
 *           DWORD > BYTES
 */
typedef union {
    uint32_t data;
    uint8_t  bytes[TYPE_DWORD_SZ];
} DWORD_ubt;

/** @typedef Union type
 *           DWORD > WORDS
 */
typedef union {
    uint32_t data;
    uint16_t words[TYPE_DWORD_WSZ];
} DWORD_uwt;


/** @typedef Union type
 *           INT > BYTES
 */
typedef union {
    int16_t data;
    uint8_t bytes[TYPE_INT_SZ];
} INT_ubt;

/** @typedef Union type
 *           INT > WORDS
 */
typedef union {
    int16_t  data;
    uint16_t words[TYPE_INT_WSZ];
} INT_uwt;


/** @typedef Union type
 *           WORD > BYTES
 */
typedef union {
    uint16_t data;
    uint8_t  bytes[TYPE_WORD_SZ];
} WORD_ubt;

/** @typedef Union type
 *           WORD > WORDS
 */
typedef union {
    uint16_t data;
    uint16_t words[TYPE_WORD_WSZ];
} WORD_uwt;


/** @typedef Union type
 *           SINT > BYTES
 */
typedef union {
    int8_t  data;
    uint8_t bytes[TYPE_SINT_SZ];
} SINT_ubt;

/** @typedef Union type
 *           SINT > WORDS
 */
typedef union {
    int8_t   data;
    uint16_t words[TYPE_SINT_WSZ];
} SINT_uwt;


/** @typedef Union type
 *           BYTE > BYTES
 */
typedef union {
    uint8_t  data;
    uint8_t  bytes[TYPE_BYTE_SZ];
} BYTE_ubt;

/** @typedef Union type
 *           BYTE > WORDS
 */
typedef union {
    uint8_t  data;
    uint16_t words[TYPE_BYTE_WSZ];
} BYTE_uwt;


#endif /* TYPE_H_ */
