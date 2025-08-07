#include "sysclk.h"
#include "stm32f1xx_ll_bus.h"
#include "stm32f1xx_ll_cortex.h"
#include "stm32f1xx_ll_rcc.h"
#include "stm32f1xx_ll_system.h"
#include "stm32f1xx_ll_utils.h"

volatile uint32_t SysTickCnt = 0;

void SysTick_Handler(void) {
    SysTickCnt++;
}

uint32_t Sysclk_TickGet(void) {
    return SysTickCnt;
}

void Sysclk_Init(void) {
    NVIC_SetPriority(SysTick_IRQn, 0);
    NVIC_EnableIRQ(SysTick_IRQn);

    /* Set FLASH latency */
    LL_FLASH_SetLatency(LL_FLASH_LATENCY_2);

    /* Enable HSE oscillator */
    LL_RCC_HSE_Enable();
    while (LL_RCC_HSE_IsReady() != 1) {
    };

    LL_RCC_PLL_Disable();
    while (LL_RCC_PLL_IsReady() != 0) {
    }

    /* Main PLL configuration and activation */
    LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSE_DIV_1, LL_RCC_PLL_MUL_9);

    LL_RCC_PLL_Enable();
    while (LL_RCC_PLL_IsReady() != 1) {
    };

    /* Sysclk activation on the main PLL */
    LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
    LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
    while (LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL) {
    };

    /* Set APB1 & APB2 prescaler*/
    LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_2);
    LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

    /* Set systick to 1ms in using frequency set to 72MHz */
    LL_Init1msTick(72000000);
    /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
    LL_SetSystemCoreClock(72000000);

    LL_SYSTICK_EnableIT();

    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOA);
    LL_APB2_GRP1_EnableClock(LL_APB2_GRP1_PERIPH_GPIOC);

    LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_USART2);

    SystemCoreClockUpdate();
}
