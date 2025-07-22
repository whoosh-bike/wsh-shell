cmake_minimum_required(VERSION 3.30)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu17")
else()
    set(CMAKE_C_STANDARD 17)
endif()

set(CMAKE_EXPORT_COMPILE_COMMANDS   ON)
set(CMAKE_C_STANDARD_REQUIRED       ON)
set(PROJ_PATH                       ${CMAKE_CURRENT_SOURCE_DIR})
message("Build type: "              ${CMAKE_BUILD_TYPE})
message("Generator: "               ${CMAKE_GENERATOR})
message("Compiler: "                ${CMAKE_C_COMPILER})

project(basic C)
enable_language(C)

set(EXECUTABLE ${CMAKE_PROJECT_NAME})

set(SRCS
    ${PROJ_PATH}/basic.c
    ${PROJ_PATH}/../../src/wsh_shell_cmd.c
    ${PROJ_PATH}/../../src/wsh_shell_user.c
    ${PROJ_PATH}/../../src/wsh_shell_cmd_def.c
    ${PROJ_PATH}/../../src/wsh_shell_history.c
    ${PROJ_PATH}/../../src/wsh_shell_str.c
    ${PROJ_PATH}/../../src/wsh_shell.c
)

set(INCLUDE_DIRS
    ${PROJ_PATH}/example/basic
    ${PROJ_PATH}/../../src
)

add_executable(${EXECUTABLE} ${SRCS})
target_include_directories(${EXECUTABLE} PRIVATE ${INCLUDE_DIRS})
target_compile_options(${EXECUTABLE} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)
