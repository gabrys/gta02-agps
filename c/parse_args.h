#include "common.h"

#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

int parse_args_find(int *argc, char ***argv, const char *param) {
    int i, j, found = 0;

    /* find matching params */
    for (i = 1; i < *argc; i++) {
        if (strcmp((*argv)[i], param) == 0) {
            found++;
        }
    }

    /* update argc */
    *argc = *argc - found;

    /* update argv */
    for (i = 1, j = 1; j < *argc; i++) {
        if (strcmp((*argv)[i], param) != 0) {
            (*argv)[j] = (*argv)[i];
            j++;
        }
    }
    
    /* return how many matching params found */
    return found;
}

int parse_args_find_last(int *argc, char ***argv, const char *param) {
    if (parse_args_find(argc, argv, param)) {
        if (1 < *argc) {
            /* that was not last parameter -- fail */
            invocation();
            exit(1);
        }
        return 1;
    }
    return 0;
}

char *parse_args_get_first(int *argc, char ***argv) {
    char *first;
    int i;

    /* see if enough args */
    if (1 >= *argc) {
        invocation();
        exit(1);
    }

    /* grab it */
    first = (*argv)[1];

    /* remove it */
    *argc = *argc - 1;
    for (i = 1; i < *argc; i++) {
        (*argv)[i] = (*argv)[i + 1];
    }

    /* return it */
    return first;
}

char *parse_args_get_first_last(int *argc, char ***argv) {
    char *first;

    first = parse_args_get_first(argc, argv);
    if (1 < *argc) {
        /* that was not last parameter -- fail */
        invocation();
        exit(1);
    }
    return first;
}

void parse_args_help(int *argc, char ***argv) {
    if (parse_args_find(argc, argv, "--help")) {
        help();
        exit(0);
    }
}

int parse_args_verbose(int *argc, char ***argv) {
    return parse_args_find(argc, argv, "--verbose");
}

int parse_args_setup_serial(int *argc, char ***argv) {
    return parse_args_find(argc, argv, "--setup-serial");
}

void parse_args_device(int *argc, char ***argv, int *dev_in, int *dev_out) {
    int fd;
    char *path;

    path = parse_args_get_first(argc, argv);
    
    if (strcmp(path, "-") == 0) {
        *dev_in = 0; /* stdin */
        *dev_out = 1; /* stdout */
    } else {
        fd = open(path, O_RDWR);
        if (fd < 0) {
            perror("open");
            exit(1);
        }
        *dev_in = fd;
        *dev_out = fd;
    }
}

int parse_args_dump_agps(int *argc, char ***argv, char **dump_file) {
    if (parse_args_find(argc, argv, "--dump-agps")) {
        *dump_file = parse_args_get_first_last(argc, argv);
        return 1;
    }
    return 0;
}

int parse_args_load_agps(int *argc, char ***argv, char **dump_file) {
    if (parse_args_find(argc, argv, "--load-agps")) {
        *dump_file = parse_args_get_first_last(argc, argv);
        return 1;
    }
    return 0;
}

int parse_args_sleep(int *argc, char ***argv) {
    return parse_args_find_last(argc, argv, "--sleep");
}

int parse_args_wake_up(int *argc, char ***argv) {
    return parse_args_find_last(argc, argv, "--wake-up");
}

