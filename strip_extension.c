#include "include/common.h"
#include <string.h>

char *strip_extension(const char *fname) {
  u32 len = strlen(fname);
  char *cpy = malloc(len + 1);
  char *end = cpy + len;

  memcpy(cpy, fname, len + 1);

  while (end > cpy && *end != '.')
    --end;

  if (end > cpy) *end = 0;

  return cpy;
}
