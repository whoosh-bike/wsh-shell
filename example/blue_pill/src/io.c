#include "usbd_cdc_if.h"

int __io_putchar(int ch) {
    uint8_t c = (uint8_t)ch;
    while (CDC_Transmit_FS(&c, 1) == USBD_BUSY) {
    }

    return ch;
}

int __io_getchar(void) {
    uint8_t c;
    while (CDC_GetRxBufferBytesAvailable_FS() == 0) {
    }

    if (CDC_ReadRxBuffer_FS(&c, 1) == USB_CDC_RX_BUFFER_OK)
        return c;

    return -1;
}
