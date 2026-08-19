#!/usr/bin/env bash

# chmod +x utils/code-format.sh
# utils/code-format.sh
#
# Single entry point for C formatting — `make format` calls this script, so both
# routes format exactly the same set of files.

INCLUDE_DIRS=("src" "example")
# example/build holds compiler output; example/wsh_shell_cfg.h is generated from
# src/wsh_shell_cfg_def.h on every build and must stay byte-identical to it.
EXCLUDE_PATHS=("example/build" "example/wsh_shell_cfg.h")

PROJECT_ROOT=$(dirname "$(dirname "$0")")

format_files() {
    for dir in "${INCLUDE_DIRS[@]}"; do
        if [ -d "$PROJECT_ROOT/$dir" ]; then
            find "$PROJECT_ROOT/$dir" -name '*.c' -o -name '*.h' | while read -r file; do
                skip=false
                for excluded in "${EXCLUDE_PATHS[@]}"; do
                    if [[ $file == "$PROJECT_ROOT/$excluded"* ]]; then
                        skip=true
                        break
                    fi
                done

                if [ "$skip" = false ]; then
                    clang-format --style=file -i "$file"
                    echo "Formatted: $file"
                fi
            done
        else
            echo "Directory $PROJECT_ROOT/$dir does not exist."
        fi
    done
}

format_files
