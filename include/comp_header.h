#ifndef __COMP_HEADER_H
#define __COMP_HEADER_H

#include "common.h"

struct header {
  u32 length;
  u8 b;
  u8 f;
  u16 header_size;
};

#endif // __COMP_HEADER_H

