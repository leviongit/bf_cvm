#include "include/common.h"
#include "include/comp.h"
#include "include/op_type.h"
#include "include/vm.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>

void usage(const char *name) {
    printf("Usage: %s <use> <file.bf>\n", name);
    printf("use:\n"
           "  - compile\n"
           "  - comp\n"
           "  - c\n"
           "    - compiles the program to bytecode\n"
           "  - interpret\n"
           "  - i\n"
           "    - runs the program\n"
           "  - run\n"
           "  - r\n"
           "    - runs the compiled program\n");
}

i32 main(i32 argc, char **argv) {
  if (argc < 3) {
    usage(argv[0]);
    return 1;
  }

  if (!strcasecmp(argv[1], "c") || !strcasecmp(argv[1], "comp") || !strcasecmp(argv[1], "compile")) {
    enum OpType *ops = compile(argv[2]);
    write_compiled_file(ops, strcat(realloc(strip_extension(argv[2]), strlen(argv[2]) + 5), ".bfc"));
  } else if(!strcasecmp(argv[1], "i") || !strcasecmp(argv[1], "interpret")) {
    run_file(argv[2]);
  } else if(!strcasecmp(argv[1], "r") || !strcasecmp(argv[1], "run")) {
    run_compiled(argv[2]);
  } else {
    usage(argv[0]);
  }
}
