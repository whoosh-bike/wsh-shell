#include "usart.h"
#include "stm32f1xx_ll_usart.h"

int __io_putchar(int ch) {
	// Wait until TX buffer is empty
	while (!LL_USART_IsActiveFlag_TXE(USART2)) {
	}

	LL_USART_TransmitData8(USART2, (uint8_t)ch);  // Transmit character
	return ch;
}

int __io_getchar(void) {
	// Wait until RX buffer is not empty
	while (!LL_USART_IsActiveFlag_RXNE(USART2)) {
	}

	return LL_USART_ReceiveData8(USART2);  // Receive character
}

void USART_Init(void) {
	LL_USART_SetTransferDirection(USART2, LL_USART_DIRECTION_TX_RX);
	LL_USART_ConfigCharacter(USART2, LL_USART_DATAWIDTH_8B, LL_USART_PARITY_NONE, LL_USART_STOPBITS_1);
	LL_USART_SetBaudRate(USART2, SystemCoreClock / 2, 115200);
	NVIC_SetPriority(USART2_IRQn, 0);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	// LL_USART_EnableIT_TXE(USART2);
	// LL_USART_EnableIT_RXNE(USART2);
	LL_USART_Enable(USART2);
}
