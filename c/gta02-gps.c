#include "common.h"
#include "parse_args.h"
#include "setup_serial.h"
#include "dump_agps.h"
#include "load_agps.h"
#include "fixnow.h"

#define setup_serial_if_needed() if (p_setup_serial) { setup_serial(dev_in); setup_serial(dev_out); }

int main(int argc, char **argv) {
    int p_setup_serial;
    int dev_in;
    int dev_out;
    char *dump_file;
    
    parse_args_help(&argc, &argv);
    VERBOSE = parse_args_verbose(&argc, &argv);
    p_setup_serial = parse_args_setup_serial(&argc, &argv);
    parse_args_device(&argc, &argv, &dev_in, &dev_out);

    if (parse_args_dump_agps(&argc, &argv, &dump_file)) {
        setup_serial_if_needed();
        dump_agps(dev_in, dev_out, dump_file);
        return 0;
    }

    if (parse_args_load_agps(&argc, &argv, &dump_file)) {
        setup_serial_if_needed();
        load_agps(dev_in, dev_out, dump_file);
        return 0;
    }

    if (parse_args_sleep(&argc, &argv)) {
        setup_serial_if_needed();
        fixnow_sleep(dev_out);
        return 0;
    }
    
    if (parse_args_wake_up(&argc, &argv)) {
        setup_serial_if_needed();
        fixnow_wake_up(dev_out);
        return 0;
    }
    
    if (p_setup_serial) {
        setup_serial(dev_in);
        setup_serial(dev_out);
        return 0;
    }

    invocation();
    return 1;
}
