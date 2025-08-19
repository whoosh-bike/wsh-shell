#include "main.h"
#include "wsh_shell.h"

#define LED_PIN  GPIO_PIN_13
#define LED_PORT GPIOC

void GPIO_LedInit(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);

    GPIO_InitStruct.Pin   = LED_PIN;
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull  = GPIO_PULLUP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void GPIO_LedDeinit(void) {
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    GPIO_InitStruct.Pin  = LED_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(LED_PORT, &GPIO_InitStruct);
}

void GPIO_LedToggle(void) {
    HAL_GPIO_TogglePin(LED_PORT, LED_PIN);
}

void GPIO_LedOn(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_RESET);
}

void GPIO_LedOff(void) {
    HAL_GPIO_WritePin(LED_PORT, LED_PIN, GPIO_PIN_SET);
}

bool GPIO_LedIsOn(void) {
    return !(bool)HAL_GPIO_ReadPin(LED_PORT, LED_PIN);
}

/* clang-format off */
#define CMD_LED_OPT_TABLE() \
X_CMD_ENTRY(CMD_LED_OPT_HELP, WSH_SHELL_OPT_HELP()) \
X_CMD_ENTRY(CMD_LED_OPT_DEF, WSH_SHELL_OPT_NO(WSH_SHELL_OPT_ACCESS_EXECUTE)) \
X_CMD_ENTRY(CMD_LED_OPT_INTERACT, WSH_SHELL_OPT_INTERACT()) \
X_CMD_ENTRY(CMD_LED_OPT_INIT, WSH_SHELL_OPT_STR(WSH_SHELL_OPT_ACCESS_EXECUTE, "-o", "--gpio", "Init or reset LED pin [init/deinit]")) \
X_CMD_ENTRY(CMD_LED_OPT_STATE, WSH_SHELL_OPT_INT(WSH_SHELL_OPT_ACCESS_EXECUTE, "-s", "--state", "Set LED state [0/1]")) \
X_CMD_ENTRY(CMD_LED_OPT_REV, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_EXECUTE, "-r", "--reverse", "Reverse LED state")) \
X_CMD_ENTRY(CMD_LED_OPT_GET, WSH_SHELL_OPT_WO_PARAM(WSH_SHELL_OPT_ACCESS_READ, "-g", "--get", "Get LED state")) \
X_CMD_ENTRY(CMD_LED_OPT_END, WSH_SHELL_OPT_END())

#define X_CMD_ENTRY(en, m) en,
typedef enum {
	CMD_LED_OPT_TABLE()
	CMD_LED_OPT_ENUM_SIZE
} CMD_LED_OPT_t;
#undef X_CMD_ENTRY
/* clang-format on */

#define X_CMD_ENTRY(enum, opt) {enum, opt},
const WshShellOption_t LedOptArr[] = {CMD_LED_OPT_TABLE()};
#undef X_CMD_ENTRY

static WSH_SHELL_RET_STATE_t shell_cmd_led(const WshShellCmd_t* pcCmd, WshShell_Size_t argc,
                                           const char* pArgv[], void* pCtx) {
    if ((argc > 0 && pArgv == NULL) || pcCmd == NULL)
        return WSH_SHELL_RET_STATE_ERROR;

    WshShell_Size_t tokenPos = 0;
    while (tokenPos < argc) {
        WshShellOption_Context_t optCtx = WshShellCmd_ParseOpt(pcCmd, argc, pArgv, &tokenPos);
        if (optCtx.Option == NULL)
            return WSH_SHELL_RET_STATE_ERR_EMPTY;

        switch (optCtx.Option->ID) {
            case CMD_LED_OPT_HELP:
                WshShellCmd_PrintOptionsOverview(pcCmd);
                break;

            case CMD_LED_OPT_DEF:
                break;

            case CMD_LED_OPT_INTERACT:
                break;

            case CMD_LED_OPT_INIT: {
                char state[16];
                WshShellCmd_GetOptValue(&optCtx, argc, pArgv, sizeof(state),
                                        (WshShell_Size_t*)&state);
                if (WSH_SHELL_STRCMP(state, "init") == 0) {
                    GPIO_LedInit();
                    printf("LED pin initialized\r\n");
                } else if (WSH_SHELL_STRCMP(state, "deinit") == 0) {
                    GPIO_LedDeinit();
                    printf("LED pin deinitialized\r\n");
                } else {
                    printf("Unknown value for --gpio: '%s'. Use 'init' or 'deinit'\r\n", state);
                }
            } break;

            case CMD_LED_OPT_STATE: {
                uint32_t state           = 0;
                WSH_SHELL_RET_STATE_t rs = WshShellCmd_GetOptValue(
                    &optCtx, argc, pArgv, sizeof(state), (WshShell_Size_t*)&state);
                if (rs != WSH_SHELL_RET_STATE_SUCCESS)
                    return rs;

                if (state > 1)
                    state = 1;

                state == 0 ? GPIO_LedOff() : GPIO_LedOn();
            } break;

            case CMD_LED_OPT_REV:
                GPIO_LedToggle();
                break;

            case CMD_LED_OPT_GET:
                WSH_SHELL_PRINT_INFO("Led state: %s\r\n", GPIO_LedIsOn() ? "on" : "off");
                break;

            default:
                return WSH_SHELL_RET_STATE_ERROR;
        }
    }

    return WSH_SHELL_RET_STATE_SUCCESS;
}

const WshShellCmd_t Shell_LedCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ADMIN,
    .Name    = "led",
    .Descr   = "PC13 LED pin management",
    .Options = LedOptArr,
    .OptNum  = CMD_LED_OPT_ENUM_SIZE,
    .Handler = shell_cmd_led,
};
