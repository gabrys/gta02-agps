#include <termios.h>
#include <unistd.h>

int main() {
    struct termios termios_p;
    cfmakeraw(&termios_p);
    tcsetattr(0, 0, &termios_p);
    return 0;
}
