# wsh-shell

## Description

`wsh-shell` is a small command line interface for creating custom and application specific services.

## Components

We have built `wsh-shell` using **CMake**, **Make**, **Ninja**, **Doxygen** and **GCC**.
So we higly recommend you to install this components and add them to your system's environment variable.
Before heading forward check instalation of all components using:

```bash
make --version
cmake --version
ninja --version
doxygen --version
gcc --version
```

## Build

If all of the dependencies are installed build procedure should work *out of the box*.

### Simple build
The easiest way to build project is to simply run:

```bash
make
```

By default project is build with *Debug* preset. If you want to build project with *Release* preset you shoud call:

```bash
make CMAKE_BUILD_TYPE=Release
```
 To check if everything is built properly you can run tests using:

 ```bash
 make test
 ```

To clean builded files run:

```shell
make clean
```

If you want to delete all build files and CMake cache consider running:

```shell
make purge
```

## Examples

Each example is a separate project. So for proper interaction with example you need to navigate to it's directory.

### Basic

The basic example contains minimum code to run a functional shell on the PC. Build procedure is similar to *Simple build*:
So to build and run *basic* example you should:

```
cd example/basic
make
./build/basic
```

## Documentation

If you want to generate doxygen documentation you can call `Generate Doxygen` VSCode task or call shell command:

```shell
doxygen
```

To open generated documentation open `/doc/html/index.html`.

### Project structure

.
├── build/
├── doc
│   └── shell_en.md
├── example
│   ├── basic/
│   └── stm32f103c8t6/
├── lib
│   └── et
│       ├── CMakeLists.txt
│       ├── et.c
│       ├── et.h
│       └── et_host.c
├── src
│   ├── CMakeLists.txt
│   ├── wsh_shell.c
│   ├── wsh_shell_cfg_def.h
│   ├── wsh_shell_cfg.h
│   ├── wsh_shell_cmd.c
│   ├── wsh_shell_cmd_def.c
│   ├── wsh_shell_cmd_def.h
│   ├── wsh_shell_cmd.h
│   ├── wsh_shell.h
│   ├── wsh_shell_history.c
│   ├── wsh_shell_history.h
│   ├── wsh_shell_io.h
│   ├── wsh_shell_option.h
│   ├── wsh_shell_str.c
│   ├── wsh_shell_str.h
│   ├── wsh_shell_types.h
│   ├── wsh_shell_user.c
│   └── wsh_shell_user.h
├── test
│   ├── CMakeLists.txt
│   ├── cmd
│   │   └── test_cmd.c
│   ├── history
│   │   └── test_history.c
│   ├── shell
│   │   └── test_shell.c
│   └── user
│       └── test_user.c
├── .clang-format
├── CMakeLists.txt
├── CMakePresets.json
├── CODE_OF_CONDUCT.md
├── CONTRIBUTING.md
├── ChangeLog.md
├── Doxyfile
├── KNOWN_ISSUES.md
├── Makefile
└── README.md
