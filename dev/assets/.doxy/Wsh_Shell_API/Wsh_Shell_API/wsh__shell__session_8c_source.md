

# File wsh\_shell\_session.c

[**File List**](files.md) **>** [**src**](dir_68267d1309a1af8e8297ef4c3efbcdba.md) **>** [**wsh\_shell\_session.c**](wsh__shell__session_8c.md)

[Go to the documentation of this file](wsh__shell__session_8c.md)


```C++
#include "wsh_shell_session.h"

#if WSH_SHELL_SESSION

static WshShell_U32_t WshShellSession_CalcHash(const WshShellSession_t* pcSession) {
    return WshShellMisc_CalcJenkinsHash((const WshShell_U8_t*)&pcSession->RebootsLeft,
                                        sizeof(*pcSession) - sizeof(pcSession->Hash));
}

WshShell_Bool_t WshShellSession_IsValid(const WshShellSession_t* pcSession) {
    if (!pcSession)
        return false;

    return (pcSession->Hash == WshShellSession_CalcHash(pcSession)) && (pcSession->RebootsLeft > 0);
}

WshShellSession_t WshShellSession_Read(WshShellSessionIO_t* pSessIO) {
    WSH_SHELL_ASSERT(pSessIO && pSessIO->Read);
    if (!pSessIO || !pSessIO->Read)
        return (WshShellSession_t){0};

    return pSessIO->Read();
}

void WshShellSession_Store(WshShellSessionIO_t* pSessIO, WshShellSession_t session) {
    WSH_SHELL_ASSERT(pSessIO && pSessIO->Write);
    if (!pSessIO || !pSessIO->Write)
        return;

    session.Hash = WshShellSession_CalcHash(&session);
    pSessIO->Write(session);
}

void WshShellSession_Clear(WshShellSessionIO_t* pSessIO) {
    WshShellSession_Store(pSessIO, (WshShellSession_t){0});
}

void WshShellSession_Init(WshShellSessionIO_t* pSessIO, WshShellSession_ReadHandler_t readFn,
                          WshShellSession_WriteHandler_t writeFn) {
    WSH_SHELL_ASSERT(pSessIO && readFn && writeFn);
    if (!pSessIO || !readFn || !writeFn)
        return;

    pSessIO->Read  = readFn;
    pSessIO->Write = writeFn;

    /* Discard uninitialised/corrupt backing store so it is never restored. */
    WshShellSession_t stored = pSessIO->Read();
    if (!WshShellSession_IsValid(&stored))
        WshShellSession_Clear(pSessIO);
}

#else /* WSH_SHELL_SESSION */

void WshShellSession_Init(WshShellSessionIO_t* pSessIO, WshShellSession_ReadHandler_t readFn,
                          WshShellSession_WriteHandler_t writeFn) {
    (void)(pSessIO);
    (void)(readFn);
    (void)(writeFn);
}

WshShellSession_t WshShellSession_Read(WshShellSessionIO_t* pSessIO) {
    (void)(pSessIO);
    return (WshShellSession_t){0};
}

void WshShellSession_Store(WshShellSessionIO_t* pSessIO, WshShellSession_t session) {
    (void)(pSessIO);
    (void)(session);
}

WshShell_Bool_t WshShellSession_IsValid(const WshShellSession_t* pcSession) {
    (void)(pcSession);
    return false;
}

void WshShellSession_Clear(WshShellSessionIO_t* pSessIO) {
    (void)(pSessIO);
}

#endif /* WSH_SHELL_SESSION */
```


