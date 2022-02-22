#ifndef __COMMON_H
#define __COMMON_H

#include <stdarg.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef float f32;
typedef double f64;

__attribute__((noreturn)) void die(const char *fmt, ...);

char *strip_extension(const char *fname);
#endif // __COMMON_H

