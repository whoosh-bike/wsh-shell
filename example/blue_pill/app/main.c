#include "main.h"
#include "usb_device.h"
#include "usbd_cdc_if.h"

#include "shell.h"

int _read(int file, char* ptr, int len) {
    char c;
    while (CDC_GetRxBufferBytesAvailable_FS() == 0) {
    }

    if (CDC_ReadRxBuffer_FS((uint8_t*)&c, 1) == USB_CDC_RX_BUFFER_OK) {
        *ptr++ = c;
        return 1;
    }

    return -1;
}

int _write(int file, char* ptr, int len) {
    (void)file;
    for (int i = 0; i < len; i++) {
        char c = (char)(*ptr++);
        while (CDC_Transmit_FS((uint8_t*)&c, 1) == USBD_BUSY) {
        }
    }

    return len;
}

void SystemClock_Config(void) {
    RCC_OscInitTypeDef RCC_OscInitStruct   = {0};
    RCC_ClkInitTypeDef RCC_ClkInitStruct   = {0};
    RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

    RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    RCC_OscInitStruct.HSEState       = RCC_HSE_ON;
    RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
    RCC_OscInitStruct.HSIState       = RCC_HSI_ON;
    RCC_OscInitStruct.PLL.PLLState   = RCC_PLL_ON;
    RCC_OscInitStruct.PLL.PLLSource  = RCC_PLLSOURCE_HSE;
    RCC_OscInitStruct.PLL.PLLMUL     = RCC_PLL_MUL9;
    if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK) {
        Error_Handler();
    }

    RCC_ClkInitStruct.ClockType =
        RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    RCC_ClkInitStruct.SYSCLKSource   = RCC_SYSCLKSOURCE_PLLCLK;
    RCC_ClkInitStruct.AHBCLKDivider  = RCC_SYSCLK_DIV1;
    RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
    RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

    if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK) {
        Error_Handler();
    }
    PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
    PeriphClkInit.UsbClockSelection    = RCC_USBCLKSOURCE_PLL_DIV1_5;
    if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK) {
        Error_Handler();
    }

    __HAL_RCC_GPIOC_CLK_ENABLE();
    __HAL_RCC_GPIOD_CLK_ENABLE();
    __HAL_RCC_GPIOA_CLK_ENABLE();
}

int main(void) {
    HAL_Init();
    SystemClock_Config();
    UsbDevice_Init();
    HAL_Delay(2000);

    if (!Shell_Init("blue-pill"))
        Error_Handler();

    for (;;) {
        int symbol = getchar();
        // printf("key: %d (0x%02X)\n", symbol, (unsigned char)symbol);

        if (symbol == EOF)
            continue;

        Shell_SendChar((char)symbol);
    }
}

void Error_Handler(void) {
    __disable_irq();
    while (1) {
    }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
    Error_Handler();
}
#endif /* USE_FULL_ASSERT */
