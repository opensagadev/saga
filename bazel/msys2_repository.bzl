"""Pinned MSYS2 MINGW32 binary packages, merged into one repository.

The i686 Windows cross build uses these for prebuilt SDL3 and ANGLE plus the
runtime DLLs they import. Every package is checked against its sha256.
"""

_MIRROR = "https://repo.msys2.org/mingw/mingw32/"

def _msys2_repository_impl(ctx):
    for package, sha256 in ctx.attr.packages.items():
        ctx.download_and_extract(
            url = _MIRROR + package.replace("+", "%2B") + ".pkg.tar.zst",
            sha256 = sha256,
            type = "tar.zst",
        )
    ctx.symlink(ctx.attr.build_file, "BUILD.bazel")

msys2_repository = repository_rule(
    implementation = _msys2_repository_impl,
    attrs = {
        "build_file": attr.label(allow_single_file = True, mandatory = True),
        "packages": attr.string_dict(
            mandatory = True,
            doc = "Package file names without .pkg.tar.zst, mapped to sha256.",
        ),
    },
)
