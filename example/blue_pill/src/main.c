#include "gpio.h"
#include "sysclk.h"
#include "usart.h"

#include "shell.h"

int main(void) {
    Sysclk_Init();
    GPIO_LedInit();
    GPIO_UsartInit();
    USART_Init();

    Shell_Init("plue-pill");

    for (;;) {
        int symbol = getchar();
        // printf("key: %d (0x%02X)\n", symbol, (unsigned char)symbol);

        if (symbol == EOF)
            continue;

        Shell_SendChar((char)symbol);
    }
}
