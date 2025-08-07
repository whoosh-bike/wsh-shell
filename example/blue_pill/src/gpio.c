#include "gpio.h"
#include "stm32f1xx_ll_gpio.h"

#define LED_PIN       LL_GPIO_PIN_13
#define LED_GPIO_PORT GPIOC

#define USART_GPIO_PORT GPIOA
#define USART_TX_PIN    LL_GPIO_PIN_2
#define USART_RX_PIN    LL_GPIO_PIN_3

void GPIO_LedInit(void) {
    LL_GPIO_InitTypeDef gpioInitStruct = {0};

    gpioInitStruct.Pin        = LED_PIN;
    gpioInitStruct.Mode       = LL_GPIO_MODE_OUTPUT;
    gpioInitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    gpioInitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruct.Pull       = LL_GPIO_PULL_DOWN;
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

void GPIO_UsartInit(void) {
    LL_GPIO_InitTypeDef gpioInitStruct = {0};

    gpioInitStruct.Pin        = USART_TX_PIN;
    gpioInitStruct.Mode       = LL_GPIO_MODE_ALTERNATE;
    gpioInitStruct.Speed      = LL_GPIO_SPEED_FREQ_HIGH;
    gpioInitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpioInitStruct.Pull       = LL_GPIO_PULL_UP;
    LL_GPIO_Init(USART_GPIO_PORT, &gpioInitStruct);

    gpioInitStruct.Pin = USART_RX_PIN;
    LL_GPIO_Init(USART_GPIO_PORT, &gpioInitStruct);
}
