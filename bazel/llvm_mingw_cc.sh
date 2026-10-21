#!/bin/bash
# Compiler and link driver for the llvm-mingw i686 toolchain.
#
# i686 Windows prefixes C and C++ symbol names with '_', but asm labels such as
# asm("_ZN16EdClassInterfaceD1Ev") are emitted verbatim. The engine uses many
# of those labels to match the original binary, so after each compile this
# wrapper gives the label symbols the prefix every other object expects.
set -euo pipefail

bin="$(dirname "$0")/bin"
"$bin/i686-w64-mingw32-clang++" "$@"

compile=0
output=""
while [ $# -gt 0 ]; do
    case "$1" in
        -c) compile=1 ;;
        -o)
            shift
            output="$1"
            ;;
    esac
    shift
done
if [ "$compile" != 1 ] || [ -z "$output" ]; then
    exit 0
fi

# Compiler-named symbols start with '_' (C++ ones with "__Z"), '.' or '@'.
# A mangled name with one underscore, or a bare identifier, came from an asm
# label.
map="$(mktemp)"
trap 'rm -f "$map"' EXIT
"$bin/llvm-nm" -P "$output" | awk '
    $1 ~ /^_Z/ || $1 ~ /^[A-Za-z]/ { print $1 " _" $1 }
' | sort -u > "$map"
if [ -s "$map" ]; then
    "$bin/llvm-objcopy" --redefine-syms="$map" "$output"
fi
