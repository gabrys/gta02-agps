#include <string.h>
#include <unistd.h>

void help() {
    char *help =

"Only first 6 and last 6 lines of this template are used"
"Here goes some lines from README :-)"
    
    ;

    write(2, help, strlen(help));
}

