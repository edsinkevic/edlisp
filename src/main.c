#include "cli_app.h"
#include "file_app.h"
#include "lisp.h"
#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv) {
  if (argc == 2) {
    run_file(argv[1]);
    return EXIT_SUCCESS;
  }

  if (argc == 1) {
    run_cli();
    return EXIT_SUCCESS;
  }

  fprintf(stderr, "USAGE: '%s' or '%s file_name'!", argv[1], argv[1]);
  return EXIT_FAILURE;
}
