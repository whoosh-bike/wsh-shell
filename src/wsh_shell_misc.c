#include "wsh_shell_misc.h"

/**
 * Jenkins hash function
 * https://en.wikipedia.org/wiki/Jenkins_hash_function
 */
WshShell_U32_t WshShellMisc_CalcJenkinsHash(const WshShell_U8_t* pcBuff, WshShell_Size_t len) {
    WSH_SHELL_ASSERT(pcBuff);

    WshShell_U32_t hash = 0;
    for (WshShell_Size_t idx = 0; idx < len; idx++) {
        hash += pcBuff[idx];
        hash += hash << 10;
        hash ^= hash >> 6;
    }

    hash += hash << 3;
    hash ^= hash >> 11;
    hash += hash << 15;

    return hash;
}

void WshShellMisc_HexDump(const WshShell_U8_t* pBuff, WshShell_Size_t len, WshShell_Size_t offset) {
    WSH_SHELL_ASSERT(pBuff || len == 0);
    if (!pBuff && len > 0)
        return;

    for (WshShell_Size_t i = 0; i < len; i += WSH_HEXDUMP_COLS) {
        WSH_SHELL_PRINT("%08x  ", (unsigned)(offset + i));

        for (WshShell_Size_t j = 0; j < WSH_HEXDUMP_COLS; j++) {
            if (i + j < len)
                WSH_SHELL_PRINT("%02x ", (unsigned)pBuff[i + j]);
            else
                WSH_SHELL_PRINT("   ");
            if (j == 7)
                WSH_SHELL_PRINT(" ");
        }

        WSH_SHELL_PRINT(" |");
        for (WshShell_Size_t j = 0; j < WSH_HEXDUMP_COLS && i + j < len; j++) {
            WshShell_U8_t c = pBuff[i + j];
            WSH_SHELL_PRINT("%c", (c >= 0x20 && c <= 0x7e) ? (char)c : '.');
        }
        WSH_SHELL_PRINT("|\r\n");
    }
}
