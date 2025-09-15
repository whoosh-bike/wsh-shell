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
