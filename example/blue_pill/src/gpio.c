#include "gpio.h"

#define LED_PIN  GPIO_PIN_13
#define LED_PORT GPIOC

void GPIO_LedInit(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);

    GPIO_InitStruct.Pin   = LED_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void GPIO_LedToggle(void) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

void GPIO_LedOn(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

void GPIO_LedOff(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

uint32_t GPIO_LedIsOn(void) {
    return HAL_GPIO_ReadPin(LED_PORT, LED_PIN);
}
