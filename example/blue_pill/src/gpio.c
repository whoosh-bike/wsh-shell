#include "gpio.h"
#include "stm32f1xx_ll_gpio.h"

#define LED_PIN		  LL_GPIO_PIN_13
#define LED_GPIO_PORT GPIOC

void GPIO_LedInit(void) {
	LL_GPIO_InitTypeDef gpioInitStruct = {0};
	gpioInitStruct.Pin				   = LED_PIN;
	gpioInitStruct.Mode				   = LL_GPIO_MODE_OUTPUT;
	gpioInitStruct.Speed			   = LL_GPIO_SPEED_FREQ_HIGH;
	gpioInitStruct.OutputType		   = LL_GPIO_OUTPUT_PUSHPULL;
	gpioInitStruct.Pull				   = LL_GPIO_PULL_DOWN;
	LL_GPIO_Init(LED_GPIO_PORT, &gpioInitStruct);
}

void GPIO_LedToggle(void) {
	LL_GPIO_TogglePin(LED_GPIO_PORT, LED_PIN);
}

void GPIO_LedOn(void) {
	LL_GPIO_ResetOutputPin(LED_GPIO_PORT, LED_PIN);
}

void GPIO_LedOff(void) {
	LL_GPIO_SetOutputPin(LED_GPIO_PORT, LED_PIN);
}

uint32_t GPIO_LedIsOn(void) {
	return !LL_GPIO_IsOutputPinSet(LED_GPIO_PORT, LED_PIN);
}

void GPIO_USART_Init(void) {
	/* Configure Tx Pin as : Alternate function, High Speed, Push pull, Pull up */
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_2, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_2, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_2, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_2, LL_GPIO_PULL_UP);

	/* Configure Rx Pin as : Alternate function, High Speed, Push pull, Pull up */
	LL_GPIO_SetPinMode(GPIOA, LL_GPIO_PIN_3, LL_GPIO_MODE_ALTERNATE);
	LL_GPIO_SetPinSpeed(GPIOA, LL_GPIO_PIN_3, LL_GPIO_SPEED_FREQ_HIGH);
	LL_GPIO_SetPinOutputType(GPIOA, LL_GPIO_PIN_3, LL_GPIO_OUTPUT_PUSHPULL);
	LL_GPIO_SetPinPull(GPIOA, LL_GPIO_PIN_3, LL_GPIO_PULL_UP);
}
