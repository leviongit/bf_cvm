#ifndef __VM_H
#define __VM_H

#include "op_type.h"

void run(enum OpType *ops);
void run_file(const char *fname);
void run_compiled(const char *fname);

#endif // __VM_H

