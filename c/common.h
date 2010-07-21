#ifndef _COMMON_H
#define _COMMON_H

#include "ubx.h"
#include "ubx_io.h"

#include "help.h"
#include "invocation.h"

#include "config.h"

int VERBOSE = 0;

#define log(msg) if (VERBOSE) write(2, msg "\n", strlen(msg) + 1)
#define logf(...) if (VERBOSE) fprintf(stderr, __VA_ARGS__)

#endif
