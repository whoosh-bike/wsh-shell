#include "et.h"
#include "wsh_shell_history.h"

void setup(void) {
    /* executed before *every* non-skipped test */
}

void teardown(void) {
    /* executed after *every* non-skipped and non-failing test */
}

/* clang-format off */

#define TOKENS_TABLE() \
X(TOKEN_QUICK, "quick") \
X(TOKEN_BROWN_1, "brown") \
X(TOKEN_BROWN_2, "brown") \
X(TOKEN_FOX, "fox") \
X(TOKEN_JUMPS_1, "jumps") \
X(TOKEN_JUMPS_2, "jumps") \
X(TOKEN_JUMPS_3, "jumps") \
X(TOKEN_OVER, "over") \
X(TOKEN_THE, "the") \
X(TOKEN_LAZY, "lazy") \
X(TOKEN_DOG, "dog")

#define X(en, t) en,
typedef enum { TOKENS_TABLE() TOKEN_ENUM_SIZE } TOKEN_t;
#undef X

#define X(en, t) t,
static char* TokensBuff[TOKEN_ENUM_SIZE] = {TOKENS_TABLE()};
#undef X

/* clang-format on */

static WshShellHistory_t History = {0};

void WshShellHistory_Write(WshShellHistory_t history) {
    WSH_SHELL_MEMCPY(&History, &history, sizeof(WshShellHistory_t));
}

WshShellHistory_t WshShellHistory_Read(void) {
    return History;
}

#define TEST_BUFF_SIZE 16

TEST_GROUP("History") {
    WshShellHistory_Init();
    char buff[TEST_BUFF_SIZE] = {0};
    char zero[TEST_BUFF_SIZE] = {0};

    TEST("Get from empty storage") {
        WshShellHistory_GetPrevCmd(buff, TEST_BUFF_SIZE);
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);

        WshShellHistory_GetNextCmd(buff, TEST_BUFF_SIZE);
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);
    }

    for (WshShell_U32_t i = 0; i < TOKEN_ENUM_SIZE; i++)
        WshShellHistory_SaveCmd(TokensBuff[i], WSH_SHELL_STRLEN(TokensBuff[i]));

    TEST("Get next command") {
        for (WshShell_S8_t i = TOKEN_ENUM_SIZE - 1; i >= 0; i--) {
            if (i == TOKEN_BROWN_2 || i == TOKEN_JUMPS_2 || i == TOKEN_JUMPS_3)
                continue;
            WshShellHistory_GetNextCmd(buff, TEST_BUFF_SIZE);
            VERIFY(WSH_SHELL_STRNCMP(buff, TokensBuff[i], TEST_BUFF_SIZE));
        }
        WSH_SHELL_MEMSET(buff, 0, sizeof(char) * TEST_BUFF_SIZE);
        WshShellHistory_GetNextCmd(buff, TEST_BUFF_SIZE);
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);
    }

    TEST("Get previous command") {
        for (WshShell_U32_t i = 0; i < TOKEN_ENUM_SIZE; i++) {
            if (i == TOKEN_BROWN_2 || i == TOKEN_JUMPS_2 || i == TOKEN_JUMPS_3)
                continue;
            WshShellHistory_GetPrevCmd(buff, TEST_BUFF_SIZE);
            VERIFY(WSH_SHELL_STRNCMP(buff, TokensBuff[i], TEST_BUFF_SIZE));
        }
        WSH_SHELL_MEMSET(buff, 0, sizeof(char) * TEST_BUFF_SIZE);
        WshShellHistory_GetPrevCmd(buff, TEST_BUFF_SIZE);
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);
    }

    TEST("Flush") {
        WshShellHistory_Flush();
        WshShellHistory_GetPrevCmd(buff, TEST_BUFF_SIZE);
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);

        WshShellHistory_GetNextCmd(buff,TEST_BUFF_SIZE );
        VERIFY(WSH_SHELL_MEMCMP(buff, zero, sizeof(char) * TEST_BUFF_SIZE) == 0);
    }
} /* TEST_GROUP() */
