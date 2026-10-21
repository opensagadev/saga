#!/bin/bash
# Compiler and link driver for the i686 MinGW toolchain.
#
# i686 MinGW prefixes C and C++ symbol names with '_', but asm labels such as
# asm("_ZN16EdClassInterfaceD1Ev") are emitted verbatim. The engine uses many
# of those labels to match the original binary, so after each compile this
# wrapper gives the label symbols the prefix every other object expects.
set -euo pipefail

prefix="${SAGA_MINGW_PREFIX:-/usr/bin/i686-w64-mingw32-}"
# Debian and Ubuntu suffix the posix-thread-model driver; Arch's mingw-w64-gcc
# only has that model and uses the plain name.
cxx="${prefix}g++-posix"
if [ ! -x "$cxx" ]; then
    cxx="${prefix}g++"
fi

compile=0
for arg in "$@"; do
    if [ "$arg" = "-c" ]; then
        compile=1
    fi
done

if [ "$compile" != 1 ]; then
    # GNU ld for i686 PE leaves references to GCC's weak definitions
    # unresolved, so link with lld. This GCC build ignores -fuse-ld, so take
    # the collect2 command line it would run and hand it to ld.lld instead.
    # Expand Bazel's param file first: given an @file, GCC hands collect2 a
    # temporary response file that -### deletes before it can be read.
    expanded=()
    for arg in "$@"; do
        if [[ "$arg" == @* ]]; then
            mapfile -t lines < "${arg#@}"
            expanded+=("${lines[@]}")
        else
            expanded+=("$arg")
        fi
    done
    link="$("$cxx" -### "${expanded[@]}" 2>&1 | grep '/collect2 ')"
    eval "set -- $link"
    shift
    args=()
    while [ $# -gt 0 ]; do
        case "$1" in
            -plugin)
                shift
                ;;
            -plugin-opt=*) ;;
            *) args+=("$1") ;;
        esac
        shift
    done
    exec "${SAGA_LLD:-ld.lld}" "${args[@]}"
fi

"$cxx" "$@"

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
"${prefix}nm" -P "$output" | awk '
    $1 ~ /^_Z/ || $1 ~ /^[A-Za-z]/ { print $1 " _" $1 }
' | sort -u > "$map"
if [ -s "$map" ]; then
    "${prefix}objcopy" --redefine-syms="$map" "$output"
fi
