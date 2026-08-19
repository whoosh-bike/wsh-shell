/**
 * @file wsh_shell_session.h
 * @brief Persistent login session across reboots.
 *
 * Lets the host keep the current login valid across a bounded number of reboots,
 * so a device that reboots mid-session (firmware bug, watchdog) does not force a
 * re-login. The budget is a plain reboot counter — no wall-clock/RTC is needed:
 * each restore consumes one reboot from the budget.
 *
 * Storage is delegated to the integrator through read/write handlers (same
 * pattern as the command history), so the shell core stays storage-agnostic.
 * A no-init RAM region is the natural backing store: it survives a warm reboot
 * but is lost on real power removal.
 *
 * @author Whoosh Embedded Team
 * @copyright Copyright (c) 2025
 */

#ifndef __WSH_SHELL_SESSION_H
#define __WSH_SHELL_SESSION_H

#include "wsh_shell_cfg.h"
#include "wsh_shell_misc.h"
#include "wsh_shell_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Persisted session descriptor with an integrity hash.
 *
 * Hash covers every field after itself, so uninitialised RAM (or a torn write)
 * reads back as invalid. A valid descriptor with RebootsLeft > 0 means an active
 * "keep me logged in" request.
 */
typedef struct {
    WshShell_U32_t Hash;        /**< Hash over the fields below. */
    WshShell_U32_t RebootsLeft; /**< Reboots the login may still survive. */
    WshShell_U32_t UserIdx;     /**< Index into the shell user table. */
} WshShellSession_t;

/** @brief Read the persisted session descriptor. */
typedef WshShellSession_t (*WshShellSession_ReadHandler_t)(void);

/** @brief Persist the session descriptor. */
typedef void (*WshShellSession_WriteHandler_t)(WshShellSession_t);

/**
 * @brief Function pointers for session persistence I/O.
 *
 * Keeps the session system storage-agnostic by delegating read/write to the
 * integrator (typically no-init RAM).
 */
typedef struct {
    WshShellSession_ReadHandler_t Read;   /**< Callback for loading the saved session. */
    WshShellSession_WriteHandler_t Write; /**< Callback for saving the session. */
} WshShellSessionIO_t;

/**
 * @brief Initialise the session system with custom I/O handlers.
 *
 * Installs the handlers and normalises the backing store: any invalid descriptor
 * (e.g. random RAM after a cold boot) is cleared so it will not be restored.
 *
 * @param[in,out] pSessIO Pointer to the I/O structure.
 * @param[in]     readFn  Callback to read the saved descriptor.
 * @param[in]     writeFn Callback to persist the descriptor.
 */
void WshShellSession_Init(WshShellSessionIO_t* pSessIO, WshShellSession_ReadHandler_t readFn,
                          WshShellSession_WriteHandler_t writeFn);

/**
 * @brief Read the persisted descriptor.
 * @param[in] pSessIO Pointer to the I/O structure.
 * @return The stored descriptor (may be invalid — check with WshShellSession_IsValid()).
 */
WshShellSession_t WshShellSession_Read(WshShellSessionIO_t* pSessIO);

/**
 * @brief Persist a descriptor, computing its integrity hash first.
 * @param[in] pSessIO  Pointer to the I/O structure.
 * @param[in] session  Descriptor to store (its Hash field is filled in).
 */
void WshShellSession_Store(WshShellSessionIO_t* pSessIO, WshShellSession_t session);

/**
 * @brief Check whether a descriptor is a valid, active keep request.
 * @param[in] pcSession Descriptor to validate.
 * @retval true  Hash matches and RebootsLeft > 0.
 * @retval false Corrupt/uninitialised, or budget exhausted.
 */
WshShell_Bool_t WshShellSession_IsValid(const WshShellSession_t* pcSession);

/**
 * @brief Drop any persisted session.
 * @param[in] pSessIO Pointer to the I/O structure.
 */
void WshShellSession_Clear(WshShellSessionIO_t* pSessIO);

#ifdef __cplusplus
}
#endif

#endif /* __WSH_SHELL_SESSION_H */
