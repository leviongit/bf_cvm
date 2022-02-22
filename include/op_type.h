#ifndef __OP_TYPE_H
#define __OP_TYPE_H

enum __attribute__((__packed__)) OpType {
  OP_CEND = 0,
  OP_INC,
  OP_INC_N,
  OP_DEC,
  OP_DEC_N,
  OP_MVL,
  OP_MVL_N,
  OP_MVR,
  OP_MVR_N,
  OP_LBGN,
  OP_LEND,
  OP_WRITE,
  OP_READ
};

extern const char *OpTypeName[];

#endif // __OP_TYPE_H

