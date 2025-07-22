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

project(wsh-shell C)
enable_language(C)
enable_testing()

set(GLOBAL_FLAGS
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
)

add_subdirectory(${PROJ_PATH}/src)
