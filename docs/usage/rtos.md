# RTOS Integration Example

The example for RTOS is not very different from Bare-Metal; you need to do the same things.

---

## Data transmitting from main app to shell task

```c
static QueueHandle_t ShellSymbol_Queue;
static SemaphoreHandle_t ShellCommand_Mutex;

bool ShellRoot_SendChar(char ch, u32 waitTmo) {
    if (xSemaphoreTake(ShellCommand_Mutex, waitTmo) != pdTRUE)
        return false;  // Need mutex here in case of ShellRoot_SendCommand simultaneous using

    bool ret = (bool)(xQueueSend(ShellSymbol_Queue, &ch, waitTmo) == pdPASS);
    xSemaphoreGive(ShellCommand_Mutex);

    return ret;
}

bool ShellRoot_SendCharFromISR(char ch, BaseType_t* pWoken) {
    if (xSemaphoreTakeFromISR(ShellCommand_Mutex, pWoken) != pdTRUE)
        return false;  // Need mutex here in case of ShellRoot_SendCommand simultaneous using

    bool ret = (bool)(xQueueSendFromISR(ShellSymbol_Queue, &ch, pWoken) == pdPASS);
    xSemaphoreGiveFromISR(ShellCommand_Mutex, pWoken);

    return ret;
}

bool ShellRoot_SendCommand(char* pCmd, u32 waitTmo) {
    ASSERT_CHECK(pCmd);

    if (!pCmd)
        return false;

    if (!WshShell_IsAuth(&ShellRoot))
        return false;

    if (xSemaphoreTake(ShellCommand_Mutex, waitTmo) != pdTRUE)
        return false;

    u32 cmdLen = strlen(pCmd);
    if (cmdLen >= Shell_Hardware_GetRxBuffLen()) {
        xSemaphoreGive(ShellCommand_Mutex);
        PANIC();
        return false;
    }

    for (u32 i = 0; i < cmdLen; i++) {
        if (xQueueSend(ShellSymbol_Queue, &pCmd[i], waitTmo) != pdTRUE) {
            xSemaphoreGive(ShellCommand_Mutex);
            PANIC();
            return false;
        }
    }

    // Send trailing '\n' to simulate Enter key
    char newLine = '\n';
    if (xQueueSend(ShellSymbol_Queue, &newLine, waitTmo) != pdTRUE) {
        xSemaphoreGive(ShellCommand_Mutex);
        PANIC();
        return false;
    }

    xSemaphoreGive(ShellCommand_Mutex);
    return true;
}
```

## Task example of shell data processing

```c
static void vTask_Shell_Process(void* pvParameters) {
    vTaskDelay(2000);
    while (!Shell_Hardware_IsInit())
        vTaskDelay(100);

    ShellRoot_Init();

#if DEBUG_ENABLE
    WshShell_Auth(&ShellRoot, "admin", "1234");
#endif /* DEBUG_ENABLE */

    for (;;) {
        char symbol = 0;
        xQueueReceive(ShellSymbol_Queue, &symbol, portMAX_DELAY);
        WshShell_InsertChar(&ShellRoot, symbol);

        vTaskDelay(RTOS_MIN_TIMEOUT_MS);
    }
}
```
