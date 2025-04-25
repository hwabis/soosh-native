#!/bin/bash

# run_clang_tidy.sh
# Usage: ./run_clang_tidy.sh [build-dir]
# Example: ./run_clang_tidy.sh build-wsl

set -e

BUILD_DIR="${1:-build-wsl}"

if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
    echo "❌ Error: compile_commands.json not found in $BUILD_DIR"
    exit 1
fi

echo "Running clang-tidy using compile_commands.json from '$BUILD_DIR'..."

find ./src \( -name '*.cc' -o -name '*.h' \) | while read -r file; do
    echo "👉 Checking $file"
    clang-tidy "$file" -p "$BUILD_DIR"
done

echo "✅ All files processed."
