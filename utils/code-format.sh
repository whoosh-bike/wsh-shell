#!/usr/bin/env bash

# chmod +x utils/code-format.sh
# utils/code-format.sh

INCLUDE_DIRS=("src")
EXCLUDE_DIRS=()

PROJECT_ROOT=$(dirname "$(dirname "$0")")

format_files() {
    for dir in "${INCLUDE_DIRS[@]}"; do
        if [ -d "$PROJECT_ROOT/$dir" ]; then
            find "$PROJECT_ROOT/$dir" -name '*.c' -o -name '*.h' | while read -r file; do
                skip=false
                for excluded in "${EXCLUDE_DIRS[@]}"; do
                    if [[ $file == "$PROJECT_ROOT/$excluded"* ]]; then
                        skip=true
                        break
                    fi
                done

                if [ "$skip" = false ]; then
                    clang-format -i "$file"
                    echo "Formatted: $file"
                fi
            done
        else
            echo "Directory $PROJECT_ROOT/$dir does not exist."
        fi
    done
}

format_files