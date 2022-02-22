#ifndef __COMP_H
#define __COMP_H

#include "op_type.h"

#include <stdbool.h>
#include <stdio.h>

enum OpType *compile(const char *fname);
void print_opcodes(enum OpType *ops);
enum OpType *group(const enum OpType *ops);
void write_compiled_file(const enum OpType *ops, const char *name); 

#endif // __COMP_H
