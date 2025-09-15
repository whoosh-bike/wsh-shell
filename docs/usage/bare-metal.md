# Bare-Metal Integration Example

This guide describes how to integrate Whoosh Shell into your project.
The integration consists of a few simple steps: adding users, registering commands, setting up callbacks, initializing the shell, and using it for interaction.

---

## 1. Include the necessary headers

```c
#include "wsh_shell.h"
```

## 2. Create a global shell instance

```c
static WshShell_t Shell = {0};
```

## 3. Register commands

Define available commands and attach them to the shell:

```c
extern const WshShellCmd_t Shell_LedCmd;

static const WshShellCmd_t* Shell_CmdTable[] = {
    &Shell_LedCmd,
};

bool Shell_Commands_Init(WshShell_t* pShell) {
    return WshShellRetState_TranslateToProject(
        WshShellCmd_Attach(&(pShell->Commands), Shell_CmdTable, WSH_SHELL_ARR_LEN(Shell_CmdTable)));
}
```

`Shell_LedCmd` structure shoud be created before in another module (look for Blue Pill example)

```c
const WshShellCmd_t Shell_LedCmd = {
    .Groups  = WSH_SHELL_CMD_GROUP_ADMIN,
    .Name    = "led",
    .Descr   = "PC13 LED pin management",
    .Options = LedOptArr,
    .OptNum  = CMD_LED_OPT_ENUM_SIZE,
    .Handler = shell_cmd_led,
};
```

## 4. Define users

- Add at least one user with credentials, groups, and rights
- Add custom function for passwords decryption

```c
#include "crypto.h"
#include "stringlib.h"

static void Shell_UserAuth_HashFunc(const WshShell_Char_t* pcSalt, const WshShell_Char_t* pcPass,
                                    WshShell_Char_t* pHash) {
    u32 saltLen = strlen(pcSalt);
    u32 passLen = strlen(pcPass);

    ASSERT_CHECK(saltLen <= WSH_SHELL_SALT_LEN);
    ASSERT_CHECK(passLen <= WSH_SHELL_PASS_LEN);

    char saltPass[WSH_SHELL_SALT_LEN + WSH_SHELL_PASS_LEN + 1];
    memcpy(saltPass, pcSalt, saltLen);
    memcpy(saltPass + saltLen, pcPass, passLen);
    saltPass[saltLen + passLen] = '\0';
    u32 saltPassLen                = saltLen + passLen;

    u8 saltPassHashBytes[CRYPTO_SHA256_BLOCK_SIZE];
    char saltPassHashStr[CRYPTO_SHA256_BLOCK_SIZE * 2 + 1];

    Crypto_SHA256_t ctx;
    Crypto_SHA256_Init(&ctx);
    Crypto_SHA256_Update(&ctx, (const u8*)saltPass, saltPassLen);
    Crypto_SHA256_Finish(&ctx, saltPassHashBytes);

    StringLib_BytesToHex(saltPassHashBytes, sizeof(saltPassHashBytes), saltPassHashStr, false);

    memcpy(pHash, saltPassHashStr, strlen(saltPassHashStr) + 1);
}

static const WshShellUser_t Shell_UserTable[] = {
    {
        .Login    = "admin",
        .Salt    = "538a03bccc40a07f",
        .Hash    = "8818316ae96ae8b4bbb2d7504b1c7b759c62bbea2c0d1595e72b4fcc7af079fa",  //1234
        .Groups = WSH_SHELL_CMD_GROUP_ADMIN,
        .Rights = WSH_SHELL_USER_ACCESS_ADMIN,
    },
};

RET_STATE_t Shell_Users_Init(WshShell_t* pShell) {
    return WshShellRetState_TranslateToProject(WshShellUser_Attach(
        &(pShell->Users), Shell_UserTable, NUM_ELEMENTS(Shell_UserTable), Shell_UserAuth_HashFunc));
}
```

## 5. Setup command history

Provide persistent storage (RAM/Flash/EEPROM/etc.):

```c
static WshShellHistory_t Shell_HistoryStorage;

static WshShellHistory_t Shell_HistoryRead(void) {
    return Shell_HistoryStorage;
}

static void Shell_HistoryWrite(WshShellHistory_t history) {
    memcpy((void*)&Shell_HistoryStorage, (void*)&history, sizeof(WshShellHistory_t));
}
```

## 6. Define callbacks

You can implement hooks for authentication, de-authentication, input and other events:

```c
static void Shell_AuthClbk(void* pCtx)   { (void)(pCtx); }
static void Shell_DeAuthClbk(void* pCtx) { (void)(pCtx); }
static void Shell_SymInClbk(void* pCtx)  { (void)(pCtx); }

static WshShell_ExtCallbacks_t Shell_Callbacks = {
    .Auth     = Shell_AuthClbk,
    .DeAuth   = Shell_DeAuthClbk,
    .SymbolIn = Shell_SymInClbk,
};
```

## 7. Initialize the shell

Perform initialization at system startup:

```c
bool Shell_Init(const char* pcHostName) {
    if (WshShell_Init(&Shell, pcHostName, NULL, &Shell_Callbacks) != WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    if (WshShellUser_Attach(&(Shell.Users), Shell_UserTable, WSH_SHELL_ARR_LEN(Shell_UserTable)) !=
        WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    WshShellHistory_Init(&Shell.HistoryIO, Shell_HistoryRead, Shell_HistoryWrite);

    if (WshShellCmd_Attach(&(Shell.Commands), Shell_CmdTable, WSH_SHELL_ARR_LEN(Shell_CmdTable)) !=
        WSH_SHELL_RET_STATE_SUCCESS) {
        return false;
    }

    // Optional: auto-login for development/testing
    // WshShell_Auth(&Shell, Shell_UserTable[0].Login, Shell_UserTable[0].Pass);

    return true;
}
```

## 8. Feeding input into the shell

Each received character (e.g., from UART, USB CDC, ...) should be passed to the shell:

```c
void Shell_SendChar(char ch) {
    WshShell_InsertChar(&Shell, ch);
}
```
