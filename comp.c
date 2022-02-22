#define _DEFAULT_SOURCE

#include "include/comp.h"
#include "include/common.h"
#include "include/op_type.h"
#include "include/comp_header.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

enum OpType *compile(const char *fname) {
  int fd = open(fname, O_RDONLY);

  struct stat stat;
  fstat(fd, &stat);

  if (stat.st_size == 0) {
    exit(0);
  }
  
  u8 *map = (u8 *)mmap(NULL, stat.st_size, PROT_READ,
                       MAP_PRIVATE | MAP_POPULATE, fd, 0);
  close(fd);

  if (map == MAP_FAILED)
    die("Failed to open and map file %s\n", fname);

  enum OpType *ops = malloc(stat.st_size + sizeof(struct header));
  memset(ops, 0, stat.st_size);
  u32 ops_size = stat.st_size;
  u32 ops_idx = 0;

  u8 loop_stack_ptr = 0;
  u32 loop_stack[0xFF] = {0};

  u8 *cptr = map;
  const u8 *eptr = map + stat.st_size;
  ops_idx += sizeof(struct header);

  while (cptr != eptr) {
    if (ops_idx + 5 >= ops_size) {
      ops_size = ops_size * 2;
      ops = realloc(ops, ops_size);
    }
    switch (*cptr) {
    case '+':
      ops[ops_idx++] = OP_INC;
      break;
    case '-':
      ops[ops_idx++] = OP_DEC;
      break;
    case '<':
      ops[ops_idx++] = OP_MVL;
      break;
    case '>':
      ops[ops_idx++] = OP_MVR;
      break;
    case '[':
      if (loop_stack_ptr == 0xFF)
        die("too many nested loops (max 255)\n");
      loop_stack[loop_stack_ptr++] = ops_idx;
      ops[ops_idx] = OP_LBGN;
      ops_idx += 5;
      break;
    case ']':
      ops[ops_idx] = OP_LEND;
      u32 offset = ops_idx - loop_stack[--loop_stack_ptr];
      *(u32 *)(&ops[ops_idx + 1]) = offset;
      *(u32 *)(&ops[loop_stack[loop_stack_ptr] + 1]) = offset;
      ops_idx += 5;
      break;
    case '.':
      ops[ops_idx++] = OP_WRITE;
      break;
    case ',':
      ops[ops_idx++] = OP_READ;
      break;
    }

    ++cptr;
  }
  ops[ops_idx] = OP_CEND;

  *(struct header*)ops = (struct header){
    .header_size = sizeof(struct header),
    .length = ops_idx,
    .b = 'b',
    .f = 'f'
  };

  munmap(map, stat.st_size);

  return ops;
}

void print_opcodes(enum OpType *ops) {
  ops += sizeof(u32);
  while (*ops != OP_CEND) {
    printf("0x%02x : %s", *ops, OpTypeName[*ops]);
    switch (*ops) {
    case OP_INC_N:
      __attribute__((fallthrough));
    case OP_DEC_N:
      __attribute__((fallthrough));
    case OP_MVL_N:
      __attribute__((fallthrough));
    case OP_MVR_N:
      printf(" %08x", *(++ops));
      break;
    case OP_LBGN:
      __attribute__((fallthrough));
    case OP_LEND:
      printf(" %08x", *((u32 *)++ops));
      ops += 3;
      break;
    default:
      break;
    }
    printf("\n");
    ++ops;
  }
}

enum OpType *group(const enum OpType *ops) {
  die("TODO: Not Implemented [%s:%s] %*", __FILE__, __LINE__, ops);
};

void write_compiled_file(const enum OpType *ops, const char *name) {
  FILE *file = fopen(name, "wb");

  if (file == NULL)
    die("Couldn't open file: %s\n", name);

  struct header header = *(struct header*)ops;

  u32 length = header.length;
  u32 written;
  if ((written = fwrite(ops, 1, length, file)) != length) {
    die("Could not write bytecode in full (%d of %d bytes written)\n", written,
        length);
  }

  fclose(file);
}
