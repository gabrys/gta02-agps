#include <termios.h>
#include <unistd.h>

void setup_serial(int fd) {
    struct termios termios_p;
    tcgetattr(fd, &termios_p);
    cfmakeraw(&termios_p);
    tcsetattr(fd, 0, &termios_p);
}
