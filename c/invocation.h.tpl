#include <string.h>
#include <unistd.h>

void invocation() {
    const char *invocation =

"Only first 6 and last 6 lines of this template are used"
"Here goes some lines from README :-)"
    
    ;

    write(2, invocation, strlen(invocation));
}

