#include "usart.h"
#include "stm32f1xx_ll_usart.h"

int __io_putchar(int ch) {
    while (!LL_USART_IsActiveFlag_TXE(USART2)) {
    }
    LL_USART_TransmitData8(USART2, (uint8_t)ch);
    return ch;
}

int __io_getchar(void) {
    while (!LL_USART_IsActiveFlag_RXNE(USART2)) {
    }
    return LL_USART_ReceiveData8(USART2);
}

void USART_Init(void) {
    LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
    LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE,
                             LL_USART_STOPBITS_1);
    LL_USART_SetBaudRate(USART2, SystemCoreClock / 2, 115200);

    LL_USART_Enable(USART2);
}
