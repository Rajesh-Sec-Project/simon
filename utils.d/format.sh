#!/bin/bash

if [ "$#" -gt 1 ]; then
    echo "Usage: format.sh [dir]"
    exit 1
fi

if [ "$#" -eq 0 ]; then
    format_path=".."
else
    format_path="$1"
fi

find "$format_path" \( -name "*.cpp" -o -name "*.h" \) -exec clang-format -i {} \;
