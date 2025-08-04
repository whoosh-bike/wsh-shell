#ifndef GPIO_H
#define GPIO_H

#include <stdint.h>

void GPIO_LedInit(void);
void GPIO_LedToggle(void);
void GPIO_LedOn(void);
void GPIO_LedOff(void);
uint32_t GPIO_LedIsOn(void);
void GPIO_USART_Init(void);

#endif /* GPIO_H */
