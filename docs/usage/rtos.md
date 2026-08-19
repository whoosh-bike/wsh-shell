# RTOS Integration Example

The example for RTOS is not very different from Bare-Metal; you need to do the same things.

---

## Data transmitting from main app to shell task

```c
static QueueHandle_t ShellSymbol_Queue;

// The symbol queue is the synchronization point — a FreeRTOS queue is already
// safe for single-element enqueue from concurrent task and ISR context, so the
// per-char paths need no extra lock. Do NOT guard this with a mutex: a mutex
// cannot be taken from an ISR (there is no owner task for priority
// inheritance), so ShellRoot_SendCharFromISR could never lock correctly — and
// with the "take mutex" pattern it would silently drop the received char
// whenever a task held the mutex (i.e. exactly during a ShellRoot_SendCommand).

bool ShellRoot_SendChar(char ch, u32 waitTmo) {
    if (!ShellSymbol_Queue)
        return false;

    return xQueueSend(ShellSymbol_Queue, &ch, waitTmo) == pdPASS;
}

bool ShellRoot_SendCharFromISR(char ch, BaseType_t* pWoken) {
    if (!ShellSymbol_Queue)
        return false;

    return xQueueSendFromISR(ShellSymbol_Queue, &ch, pWoken) == pdPASS;
}

bool ShellRoot_SendCommand(char* pCmd) {
    ASSERT_CHECK(pCmd);

    if (!pCmd || !ShellSymbol_Queue)
        return false;

    if (!WshShell_IsAuth(&ShellRoot))
        return false;

    u32 cmdLen = strlen(pCmd);
    // +1 for the trailing '\n' that simulates the Enter key.
    if (cmdLen + 1 > Shell_Hardware_GetRxBuffLen())
        return false;

    bool ok = true;

    // Inject the whole command atomically. A critical section masks the RX ISR
    // (it raises BASEPRI to configMAX_SYSCALL_INTERRUPT_PRIORITY), so no
    // received char interleaves with the command — this is the correct way to
    // exclude an ISR, since a mutex cannot. Sends must be non-blocking inside a
    // critical section, and the queue cannot drain while it is held, so the
    // whole command must fit the current free space; otherwise nothing is
    // enqueued and the injection fails atomically (no partial command).
    taskENTER_CRITICAL();
    if (uxQueueSpacesAvailable(ShellSymbol_Queue) < cmdLen + 1) {
        ok = false;
    } else {
        for (u32 i = 0; i < cmdLen; i++)
            xQueueSend(ShellSymbol_Queue, &pCmd[i], 0);

        char newLine = '\n';  // trailing Enter
        xQueueSend(ShellSymbol_Queue, &newLine, 0);
    }
    taskEXIT_CRITICAL();

    return ok;
}
```

> **Why no mutex?** A FreeRTOS mutex relies on priority inheritance, which
> requires a task to be recorded as the owner — an ISR has none, so
> `xSemaphoreTakeFromISR()` must not be used on a mutex. The queue itself is
> already ISR/task-safe for single-char enqueue, so the per-char paths need no
> lock at all. Whole-command atomicity (so an injected command is not
> interleaved with live input) is instead achieved with a short critical
> section, which masks the RX interrupt for the duration of the injection.

## Task example of shell data processing

```c
static void vTask_Shell_Process(void* pvParameters) {
    vTaskDelay(pdMS_TO_TICKS(2000));
    while (!Shell_Hardware_IsInit())
        vTaskDelay(pdMS_TO_TICKS(100));

    ShellRoot_Init();

#if DEBUG_ENABLE
    WshShell_Auth(&ShellRoot, "admin", "1234");
#endif /* DEBUG_ENABLE */

    for (;;) {
        char symbol = 0;
        xQueueReceive(ShellSymbol_Queue, &symbol, portMAX_DELAY);
        WshShell_InsertChar(&ShellRoot, symbol);

        vTaskDelay(pdMS_TO_TICKS(RTOS_MIN_TIMEOUT_MS));
    }
}
```
