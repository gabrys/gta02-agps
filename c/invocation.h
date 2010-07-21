#include <string.h>
#include <unistd.h>

void invocation() {
    const char *invocation =

"    gta02-gps --help\n"
"    gta02-gps [--verbose] --setup-serial <device-file>\n"
"    gta02-gps [--verbose] [--setup-serial] <device-file> --load-agps <agps-path>\n"
"    gta02-gps [--verbose] [--setup-serial] <device-file> --dump-agps <agps-path>\n"
"    gta02-gps [--verbose] [--setup-serial] <device-file> --sleep\n"
"    gta02-gps [--verbose] [--setup-serial] <device-file> --wake-up\n"
    
    ;

    write(2, invocation, strlen(invocation));
}

