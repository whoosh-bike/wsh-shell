# TODO / Roadmap

## v1.0

- [x] Implement base string parsing flow
- [x] Implement user authentication flow
- [x] Implement history buffer support
- [x] Implement autocomplete actions triggered by the Tab key
- [x] Add basic and Blue Pill examples
- [x] Add support for ESC sequences and other ASCII control commands

## v2.0

- [x] Review approaches for using the shell module with multiple instances, if needed
- [x] Optimize history handling — fix bugs, improve user experience, replace CRC32 with Jenkins hash, etc.
- [x] Implement advanced autocomplete with flag suggestions
- [x] Simplify configuration file structure
- [x] Add a built-in default command for querying the shell interface
- [x] Optimize options file structure

## v2.1

- [x] Support input range only from `space` till `~`
- [x] Change command table attachment from storing commands to storing pointers to commands
- [x] Add an extra space (' ') on double-Tab during command autocompletion
- [x] Implement input blocking while waiting for specific keys (Enter, Yes/No, etc.)
- [x] Save previous symbol and handle different terminals setup (`\r`, `\n` or `\r\n`)
- [x] Add string print with deAuth reason on deAuth event
- [x] Automatically generate documentation on GitHub Pages

## v2.2

- [x] Add support for storing passwords in encrypted (hashed) form in memory

## v2.3

- [x] Review default command behavior and provide more detailed output about it
- [x] Fix fault on history read and write callbacks if history isn't inited
- [x] Review groups, permissions, and access levels for commands, users, and options; document them
- [x] Check the same names for commands and options (if short and long options repited in one command or commands are same)

## v2.4 (Security, Robustness & Developer Experience)

### Security Improvements

- [x] Implement constant-time comparison for login/password validation to prevent timing attacks
- [x] Add secure memory zeroing for sensitive data (password hash buffer) using volatile loop
- [x] Add comprehensive input validation for login and password lengths
- [x] Improve credential checking with length validation before comparison

### Robustness & Bug Fixes

- [x] Fix potential fault in history read/write callbacks when history is not initialized
- [x] Fix escape sequence bug in input handling
- [x] Add duplicate detection for short and long option flags within commands (triggers ASSERT)
- [x] Fix command line flush bug
- [x] Fix argument processing flow
- [x] Multiple small fixes and code quality improvements

### Features & Enhancements

- [x] Add history save support for interactive commands
- [x] Add default command description to help output
- [x] Add more detailed build information to shell startup banner (compiler, OS, build date/time)
- [x] Add FreeRTOS version detection support
- [x] Add `WSH_SHELL_PRINT_LEVEL` macro for better print control
- [x] Add debug build flag support
- [x] Review and document groups, permissions, and access levels system

### Documentation & Tooling

- [x] Update documentation for permission rules and access control
- [x] Add ELF size analysis utility script
- [x] Fix grammar and typos in documentation
- [x] Update README with clearer examples
- [x] Fix VSCode task configurations
- [x] Improve Makefile structure

## v2.5 (Security & Stability Fixes)

- [x] Fix typo in `WshShellCmd_FindOpt` parameter validation (`pcCmd || pcCmd` → `pcCmd || pcStr`)
- [x] Add VLA stack overflow protection in autocomplete (check against `WSH_SHELL_AUTOCOMPLETE_MAX_CANDIDATES`)
- [x] Add bounds checking before buffer copy in autocomplete to prevent buffer overflow
- [x] Add secure erase of sensitive data (`saltPass`) in default hash function using volatile loop
- [x] Add `volatile` qualifier to constant-time comparison variables to prevent compiler optimization
- [x] Add validation for unclosed quotes in command parsing (`WshShellStr_ParseToArgcArgv`)

## v2.6 (Build System, UX & Developer Tooling)

- [x] Restructure build system: remove static library target, add `gen-config` target, move config generation to `example/` directory
- [x] Add compiler selection support (`CC ?= gcc` / `clang`) with passthrough from top-level Makefile
- [x] Add `--ping` / `-p` flag to default `wsh` command for adapter health check
- [x] Improve print macros format: add colored `[TAG]` prefix to `INFO/WARN/ERR` messages for easier log parsing
- [x] Fix unknown token handling: print `[WARN]` for unrecognized options instead of silently falling back to default behavior
- [x] Fix spurious `ERR_EMPTY` response when valid options are followed by unknown tokens in the same command call
- [x] Add entry/exit info messages for interactive mode
- [x] Fix IntelliSense configuration (`c_cpp_properties.json`)
- [x] Update and improve documentation
- [x] Extend autocomplete to support flag suggestions (not just command names)

## v3.0

- [ ] Automate build and test runs via GitHub Actions
