"""i686 MinGW-w64 cross toolchain for the native Windows build.

Uses the system cross compiler installed on a Linux host, for example from
Ubuntu's `g++-mingw-w64-i686-posix` package. The posix thread model is
required for the C++ threading support used by the host layer.
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
    prefix = ctx.attr.tool_prefix
    suffix = ctx.attr.tool_suffix
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
            name = "static_runtime",
            enabled = True,
            flag_sets = [
                flag_set(
                    actions = link_actions,
                    flag_groups = [flag_group(flags = ["-static"])],
                ),
            ],
        ),
    ]
    paths = [
        tool_path(name = "ar", path = prefix + "ar"),
        tool_path(name = "cpp", path = prefix + "cpp" + suffix),
        # As in the NDK toolchain, g++ drives every compile and the link; the
        # force_c_language feature restores C semantics for c_compile. The
        # wrapper fixes up asm-label symbol names after each compile.
        tool_path(name = "gcc", path = "mingw_gcc.sh"),
        tool_path(name = "gcov", path = prefix + "gcov" + suffix),
        tool_path(name = "ld", path = prefix + "ld"),
        tool_path(name = "nm", path = prefix + "nm"),
        tool_path(name = "objcopy", path = prefix + "objcopy"),
        tool_path(name = "objdump", path = prefix + "objdump"),
        tool_path(name = "strip", path = prefix + "strip"),
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
        compiler = "gcc",
        cxx_builtin_include_directories = ctx.attr.builtin_include_directories,
        features = features,
        host_system_name = "x86_64-unknown-linux-gnu",
        target_cpu = "x86_32",
        target_libc = "msvcrt",
        target_system_name = "i686-w64-mingw32",
        tool_paths = paths,
        toolchain_identifier = "mingw-w64-i686",
    )

mingw_cc_toolchain_config = rule(
    implementation = _mingw_cc_toolchain_config_impl,
    attrs = {
        "builtin_include_directories": attr.string_list(default = [
            "/usr/lib/gcc/i686-w64-mingw32/",
            "/usr/i686-w64-mingw32/include/",
            "/usr/share/mingw-w64/include/",
        ]),
        "tool_prefix": attr.string(default = "/usr/bin/i686-w64-mingw32-"),
        "tool_suffix": attr.string(default = "-posix"),
    },
    provides = [CcToolchainConfigInfo],
)
