#ifndef _COMMON_H
#define _COMMON_H

#include <stdio.h>

#include "ubx.h"
#include "help.h"
#include "ubx_io.h"

#include "config.h"

int VERBOSE = 0;

#define log(msg) if (VERBOSE) write(2, msg "\n", strlen(msg) + 1)
#define logf(...) if (VERBOSE) fprintf(stderr, __VA_ARGS__)

/* -h, --help, wrong number of arguments => print help */
/* -v, --verbose => vebose */
void parse_common_args(int *argc_p, char **argv, int params_min, int params_max) {
    int i, j;
    for (i = 1; i < *argc_p; i++) {
        if (strcmp(argv[i], "--verbose") == 0 || strcmp(argv[i], "-v") == 0) {
            VERBOSE = 1;
            /* delete that argument */
            *argc_p = (*argc_p) - 1;
            for (j = i; j < *argc_p; j++) {
                argv[j] = argv[j + 1];
            }
        }
    }
    for (i = 1; i < *argc_p; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            help();
            exit(0);
        }
    }
    if (*argc_p -1 < params_min || *argc_p - 1 > params_max) {
        help();
        exit(2);
    }
}

#endif
