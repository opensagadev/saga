"""Pinned llvm-mingw release: the hermetic i686 Windows cross toolchain.

llvm-mingw bundles clang, lld and the MinGW-w64 headers and runtime libraries
for Linux x86-64 hosts, so the Windows build needs nothing installed.
"""

def _build_file(root):
    return """load("@rules_cc//cc/toolchains:cc_toolchain.bzl", "cc_toolchain")
load(":cc_toolchain_config.bzl", "mingw_cc_toolchain_config")

package(default_visibility = ["//visibility:public"])

filegroup(
    name = "toolchain_files",
    srcs = ["llvm_mingw_cc.sh"] + glob(
        [
            "bin/**",
            "lib/clang/**",
            "lib/libLLVM*",
            "lib/libclang-cpp*",
            "i686-w64-mingw32/**",
        ],
    ),
)

mingw_cc_toolchain_config(
    name = "config",
    builtin_include_directories = [
        {root} + "/i686-w64-mingw32/include",
        {root} + "/lib/clang",
    ],
)

cc_toolchain(
    name = "cc_toolchain",
    all_files = ":toolchain_files",
    ar_files = ":toolchain_files",
    compiler_files = ":toolchain_files",
    dwp_files = ":toolchain_files",
    linker_files = ":toolchain_files",
    objcopy_files = ":toolchain_files",
    strip_files = ":toolchain_files",
    supports_param_files = 1,
    toolchain_config = ":config",
)

# Cross-compiles 32-bit Windows binaries from a Linux x86-64 host.
toolchain(
    name = "toolchain",
    exec_compatible_with = [
        "@platforms//cpu:x86_64",
        "@platforms//os:linux",
    ],
    target_compatible_with = [
        "@platforms//cpu:x86_32",
        "@platforms//os:windows",
    ],
    toolchain = ":cc_toolchain",
    toolchain_type = "@bazel_tools//tools/cpp:toolchain_type",
)
""".format(root = json.encode(root))

def _llvm_mingw_repository_impl(ctx):
    name = "llvm-mingw-{}-msvcrt-ubuntu-22.04-x86_64".format(ctx.attr.version)
    ctx.download_and_extract(
        url = "https://github.com/mstorsjo/llvm-mingw/releases/download/{}/{}.tar.xz".format(
            ctx.attr.version,
            name,
        ),
        sha256 = ctx.attr.sha256,
        stripPrefix = name,
    )
    ctx.symlink(ctx.attr._config, "cc_toolchain_config.bzl")
    ctx.symlink(ctx.attr._wrapper, "llvm_mingw_cc.sh")
    ctx.file("BUILD.bazel", _build_file(str(ctx.path("."))))

llvm_mingw_repository = repository_rule(
    implementation = _llvm_mingw_repository_impl,
    attrs = {
        "sha256": attr.string(mandatory = True),
        "version": attr.string(mandatory = True),
        "_config": attr.label(default = Label("//bazel:llvm_mingw_cc_toolchain_config.bzl")),
        "_wrapper": attr.label(default = Label("//bazel:llvm_mingw_cc.sh")),
    },
)
