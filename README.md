# wsh-shell

## Description

`wsh-shell` is a small command line interface for creating custom and application specific services.

## Components

We have built `wsh-shell` using **Make**, **Doxygen** and **GCC**.
So we higly recommend you to install this components and add them to your system's environment variable.
Before heading forward check instalation of all components using:

```bash
make --version
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
make BUILD=Release
```
 To check if everything is built properly you can build and run tests using:

 ```bash
 make test
 ```

To clean builded files run:

```shell
make clean
```

## Examples

Each example is a separate project. So for proper interaction with example you need to navigate to it's directory.

### Basic

The basic example contains minimum code to run a functional shell on the PC. Build procedure is similar to *Simple build*:
So to build and run *basic* example you should:

```
make basic
./example/basic/build/basic
```

or

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
