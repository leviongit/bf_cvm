#define _DEFAULT_SOURCE

#include "include/common.h"
#include "include/comp.h"
#include "include/comp_header.h"
#include "include/vm.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

void run(enum OpType *ops) {
  void *jmps[255] = {
      &&op_cend, &&op_inc,   &&op_inc_n, &&op_dec,   &&op_dec_n,
      &&op_mvl,  &&op_mvl_n, &&op_mvr,   &&op_mvr_n, &&op_lbgn,
      &&op_lend, &&op_write, &&op_read, 0
  };

  u8 tape[30000] = {0};
  u16 ptr = 0;

  struct header header = *((struct header*) ops);
  *((char **)&ops) += header.header_size;

  if(header.b != 'b' && header.f != 'f') die("Magic signature \"%02x%02x\" not found, instead found \"%02x%02x\"\n", 'b', 'f', header.b, header.f);
  
  goto *jmps[*ops];
op_cend:
  return;
op_inc:
  ++tape[ptr];
  goto *jmps[*(++ops)];
op_inc_n:
  tape[ptr] += *(++ops);
  goto *jmps[*(++ops)];
op_dec:
  --tape[ptr];
  goto *jmps[*(++ops)];
op_dec_n:
  tape[ptr] -= *(++ops);
  goto *jmps[*(++ops)];
op_mvl:
  --ptr;
  goto *jmps[*(++ops)];
op_mvl_n:
  ptr -= *(++ops);
  goto *jmps[*(++ops)];
op_mvr:
  ++ptr;
  goto *jmps[*(++ops)];
op_mvr_n:
  ptr += *(++ops);
  goto *jmps[*(++ops)];
op_lbgn:
  if (!tape[ptr]) {
    ops += *((u32 *) (ops + 1));
  } else {
    ops += 5;
  }
  goto *jmps[*(ops)];
op_lend:
  if (tape[ptr]) {
    ops -= *((u32 *) (ops + 1));
  } else {
    ops += 5;
  }
  goto *jmps[*(ops)];
op_write:
  printf("%c", tape[ptr]);
  goto *jmps[*(++ops)];
op_read:
  scanf("%c", &tape[ptr]);
  goto *jmps[*(++ops)];
}

void run_file(const char *fname) {
  enum OpType *ops = compile(fname);
  run(ops);
  free(ops);
}

void run_compiled(const char *fname) {
  int fd = open(fname, O_RDONLY);
  struct stat stat;
  fstat(fd, &stat);
  enum OpType *code = mmap(NULL, stat.st_size, PROT_READ, MAP_PRIVATE | MAP_POPULATE, fd, 0);

  close(fd);
  run(code);
  munmap(code, stat.st_size);
}
