set(SRCS
    ${PROJ_PATH}/src/wsh_shell_cmd.c
    ${PROJ_PATH}/src/wsh_shell_user.c
    ${PROJ_PATH}/src/wsh_shell_cmd_def.c
    ${PROJ_PATH}/src/wsh_shell_history.c
    ${PROJ_PATH}/src/wsh_shell_str.c
    ${PROJ_PATH}/src/wsh_shell.c
)

add_library(wsh-shell STATIC ${SRCS})

target_include_directories(wsh-shell PUBLIC ${PROJ_PATH}/src)
target_compile_options(wsh-shell PUBLIC ${GLOBAL_FLAGS})
