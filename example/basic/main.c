#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

#include "shell.h"

#ifndef HOST_NAME_MAX
    #ifdef _POSIX_HOST_NAME_MAX
        #define HOST_NAME_MAX _POSIX_HOST_NAME_MAX
    #else
        #define HOST_NAME_MAX 255
    #endif
#endif

static struct termios orig_termios;

void ResetTermios(void) {
    tcsetattr(STDIN_FILENO, TCSANOW, &orig_termios);
}

void SetRawTermios(void) {
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(ResetTermios);

    struct termios raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);  // выключаем канонический режим и эхо
    raw.c_lflag |= ISIG;              // оставляем обработку Ctrl+C
    raw.c_iflag &= ~(IXON | ICRNL);   // отключаем Ctrl-S/Q и перевод \r в \n
    raw.c_cc[VMIN]  = 1;
    raw.c_cc[VTIME] = 0;

    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(void) {
    SetRawTermios();

    char hostname[HOST_NAME_MAX + 1];
    if (gethostname(hostname, sizeof(hostname)) == 0) {
        hostname[sizeof(hostname) - 1] = '\0';
    } else {
        perror("gethostname");
        return 1;
    }

    Shell_Init(hostname);

    for (;;) {
        int symbol = getchar();
        // printf("key: %d (0x%02X)\n", symbol, (unsigned char)symbol);

        if (symbol == EOF)
            continue;

        if (symbol == 3) {
            printf("Exit from shell");
            break;
        }

        Shell_SendChar((char)symbol);
    }

    ResetTermios();
    return 0;
}
