#include "include/common.h"
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

__attribute__((__noreturn__)) void die(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  vprintf(fmt, args);
  exit(1);
}
