"""i686 llvm-mingw cross toolchain for the native Windows build.

Instantiated inside the @llvm_mingw repository (see llvm_mingw_repository.bzl),
so the tool paths below are relative to that repository's root.
"""

load("@rules_cc//cc:action_names.bzl", "ACTION_NAMES")
load(
    "@rules_cc//cc:cc_toolchain_config_lib.bzl",
    "artifact_name_pattern",
    "feature",
    "flag_group",
    "flag_set",
    "tool_path",
)
load("@rules_cc//cc/common:cc_common.bzl", "cc_common")
load("@rules_cc//cc/toolchains:cc_toolchain_config_info.bzl", "CcToolchainConfigInfo")

def _mingw_cc_toolchain_config_impl(ctx):
    compile_actions = [
        ACTION_NAMES.c_compile,
        ACTION_NAMES.cpp_compile,
        ACTION_NAMES.cpp_header_parsing,
        ACTION_NAMES.cpp_module_codegen,
        ACTION_NAMES.cpp_module_compile,
    ]
    link_actions = [
        ACTION_NAMES.cpp_link_dynamic_library,
        ACTION_NAMES.cpp_link_executable,
        ACTION_NAMES.cpp_link_nodeps_dynamic_library,
    ]
    features = [
        feature(
            name = "archive_param_file",
            enabled = True,
        ),
        feature(
            name = "force_c_language",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = [ACTION_NAMES.c_compile],
                    flag_groups = [flag_group(flags = ["-x", "c"])],
                ),
            ],
        ),
        feature(
            name = "dbg",
            flag_sets = [
                flag_set(
                    actions = compile_actions,
                    flag_groups = [flag_group(flags = ["-g"])],
                ),
            ],
        ),
        feature(
            name = "opt",
            flag_sets = [
                flag_set(
                    actions = compile_actions,
                    flag_groups = [flag_group(flags = ["-DNDEBUG"])],
                ),
            ],
        ),
        feature(
            # Ship one executable that only needs the bundled third-party DLLs.
            # The host layer uses POSIX threads (winpthreads).
            name = "static_runtime",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = link_actions,
                    flag_groups = [flag_group(flags = ["-static", "-pthread"])],
                ),
            ],
        ),
    ]
    paths = [
        tool_path(name = "ar", path = "bin/llvm-ar"),
        tool_path(name = "cpp", path = "bin/i686-w64-mingw32-clang"),
        # clang++ drives every compile and the link (with lld); the
        # force_c_language feature restores C semantics for c_compile. The
        # wrapper fixes up asm-label symbol names after each compile.
        tool_path(name = "gcc", path = "llvm_mingw_cc.sh"),
        tool_path(name = "gcov", path = "/bin/false"),
        tool_path(name = "ld", path = "bin/ld.lld"),
        tool_path(name = "nm", path = "bin/llvm-nm"),
        tool_path(name = "objcopy", path = "bin/llvm-objcopy"),
        tool_path(name = "objdump", path = "bin/llvm-objdump"),
        tool_path(name = "strip", path = "bin/llvm-strip"),
    ]

    return cc_common.create_cc_toolchain_config_info(
        ctx = ctx,
        abi_libc_version = "msvcrt",
        abi_version = "i686-w64-mingw32",
        artifact_name_patterns = [
            artifact_name_pattern(
                category_name = "executable",
                prefix = "",
                extension = ".exe",
            ),
            artifact_name_pattern(
                category_name = "dynamic_library",
                prefix = "",
                extension = ".dll",
            ),
        ],
        compiler = "clang",
        cxx_builtin_include_directories = ctx.attr.builtin_include_directories,
        features = features,
        host_system_name = "x86_64-unknown-linux-gnu",
        target_cpu = "x86_32",
        target_libc = "msvcrt",
        target_system_name = "i686-w64-mingw32",
        tool_paths = paths,
        toolchain_identifier = "llvm-mingw-i686",
    )

mingw_cc_toolchain_config = rule(
    implementation = _mingw_cc_toolchain_config_impl,
    attrs = {
        "builtin_include_directories": attr.string_list(mandatory = True),
    },
    provides = [CcToolchainConfigInfo],
)
