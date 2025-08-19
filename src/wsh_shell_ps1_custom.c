#include "wsh_shell_ps1_custom.h"

#if WSH_SHELL_PS1_CUSTOM

    #define WSH_PS1_SPACE_LEFT(out, base) ((out) - (base) < (WSH_SHELL_PS1_MAX_LEN - 1))

static const WshShell_Char_t* WshShellStr_ColorMap[] = {
    WSH_SHELL_COLOR_BLACK,   // %c0
    WSH_SHELL_COLOR_RED,     // %c1
    WSH_SHELL_COLOR_GREEN,   // %c2
    WSH_SHELL_COLOR_YELLOW,  // %c3
    WSH_SHELL_COLOR_BLUE,    // %c4
    WSH_SHELL_COLOR_PURPLE,  // %c5
    WSH_SHELL_COLOR_CYAN,    // %c6
    WSH_SHELL_COLOR_WHITE    // %c7
};

void WshShell_GeneratePS1(WshShell_Char_t* pPS1, WshShell_PS1Data_t* pPS1Data) {
    WSH_SHELL_ASSERT(pPS1 && pPS1Data->UserName && pPS1Data->DevName);
    if (!pPS1 || !pPS1Data->UserName || !pPS1Data->DevName)
        return;

    const WshShell_Size_t colorMapSize = WSH_SHELL_ARR_LEN(WshShellStr_ColorMap);
    WshShell_Char_t* pOut              = pPS1;
    const WshShell_Char_t* pcIn        = WSH_SHELL_PS1_TEMPLATE;
    const WshShell_Char_t* pcCurrColor = "";

    while (*pcIn && WSH_PS1_SPACE_LEFT(pOut, pPS1)) {
        if (*pcIn == '%') {
            ++pcIn;
            if (!*pcIn)
                break;

            if (*pcIn == 'u') {  // User name
                const WshShell_Char_t* pcUser = pPS1Data->UserName;
                while (*pcUser && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcUser++;
            } else if (*pcIn == 'd') {  // Device name
                const WshShell_Char_t* pcDev = pPS1Data->DevName;
                while (*pcDev && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcDev++;
            } else if (*pcIn == 'c') {  // Color choice
                ++pcIn;
                if (*pcIn >= '0' && *pcIn <= '9') {  // Valid colors range
                    WshShell_Size_t idx = *pcIn - '0';
                    const WshShell_Char_t* pcColor =
                        (idx < colorMapSize) ? WshShellStr_ColorMap[idx] : "";

                    pcCurrColor = pcColor;

                    while (*pcColor && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcColor++;
                }
            } else if (*pcIn == 'r') {  // Reset style
                const WshShell_Char_t* pcReset = WSH_SHELL_ESC_RESET_STYLE;
                while (*pcReset && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcReset++;
            } else if (*pcIn == 'b') {  // Bold style
                const WshShell_Char_t* pcBold = WSH_SHELL_ECS_SET_MODE_BOLD;
                while (*pcBold && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcBold++;
            } else if (*pcIn == 'i') {  // Interactive command name
                if (pPS1Data->InterCmdName && *pPS1Data->InterCmdName) {
                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = ' ';

                    const WshShell_Char_t* pcWhite = WSH_SHELL_COLOR_WHITE;
                    while (*pcWhite && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcWhite++;

                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = '(';

                    const WshShell_Char_t* pcColor = pcCurrColor;
                    while (*pcColor && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcColor++;

                    const WshShell_Char_t* pcInter = pPS1Data->InterCmdName;
                    while (*pcInter && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcInter++;

                    pcWhite = WSH_SHELL_COLOR_WHITE;
                    while (*pcWhite && WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = *pcWhite++;

                    if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                        *pOut++ = ')';
                }
            } else {
                if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = '%';
                if (WSH_PS1_SPACE_LEFT(pOut, pPS1))
                    *pOut++ = *pcIn;
            }
        } else {
            *pOut++ = *pcIn;
        }
        ++pcIn;
    }

    *pOut = '\0';
}

#else /* WSH_SHELL_PS1_CUSTOM */

void WshShell_GeneratePS1(WshShell_Char_t* pPS1, WshShell_PS1Data_t* pPS1Data) {
    WSH_SHELL_ASSERT(pPS1 && pPS1Data->UserName && pPS1Data->DevName);
    if (!pPS1 || !pPS1Data->UserName || !pPS1Data->DevName)
        return;

    const WshShell_Char_t* pcInterCmd = "";

    if (pPS1Data->InterCmdName && *pPS1Data->InterCmdName) {
        static WshShell_Char_t interCmdBuff[2 * WSH_SHELL_CMD_NAME_LEN];
        WSH_SHELL_SNPRINTF(interCmdBuff, sizeof(interCmdBuff),
                           WSH_SHELL_COLOR_WHITE " (" WSH_SHELL_COLOR_GREEN
                                                 "%s" WSH_SHELL_COLOR_WHITE ")",
                           pPS1Data->InterCmdName);
        pcInterCmd = interCmdBuff;
    }

    WSH_SHELL_SNPRINTF(pPS1, WSH_SHELL_PS1_MAX_LEN,
                       WSH_SHELL_ESC_RESET_STYLE WSH_SHELL_ECS_SET_MODE_BOLD WSH_SHELL_COLOR_CYAN
                       "%s" WSH_SHELL_COLOR_WHITE "@" WSH_SHELL_COLOR_PURPLE "%s"
                       "%s"  // pcInterCmd
                       WSH_SHELL_COLOR_WHITE " > " WSH_SHELL_ESC_RESET_STYLE,
                       pPS1Data->UserName, pPS1Data->DevName, pcInterCmd);
}

#endif /* WSH_SHELL_PS1_CUSTOM */
