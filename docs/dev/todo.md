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
- [x] Add USB CDC support for STM32F1 Blue Pill
- [x] Change command table attachment from storing commands to storing pointers to commands
- [x] Add an extra space (' ') on double-Tab during command autocompletion
- [x] Implement input blocking while waiting for specific keys (Enter, Yes/No, etc.)
- [x] Save previous symbol and handle different terminals setup (`\r`, `\n` or `\r\n`)
- [x] Add string print with deAuth reason on deAuth event
- [x] Automatically generate documentation on GitHub Pages

## v2.2

- [x] Add support for storing passwords in encrypted(hashed) form in memory

## v2.3

- [x] Review default command behavior and provide more detailed output about it
- [x] Fix fault on history read and write callbacks if history isn't inited
- [ ] Review groups, permissions, and access levels for commands, users, and options; document them. Add `reader` user for reference
- [ ] Check the same names for commands and options (if short and long options repited in one command or commands are same)

## v3.0

- [ ] Automate build and test runs via GitHub Actions
