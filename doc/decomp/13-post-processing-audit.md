# Specular lighting and Android post-processing

Reference: `res/libTTapp.so`, Android x86. Addresses below are ELF addresses;
the Ghidra database adds `0x10000`.

## Directional-light intensity

`rtlApplySetScale` retained the type-5 light's selection priority (`2.0f`) as
its shading strength. The original selection code also uses that priority,
but `rtlCalcLights` resets the retained strength to `1.0f` at `0x3abcb8`
before multiplying the light intensity and set scale at `0x3abeb3`.
The reconstruction now performs that reset. This removes a twofold increase
in the affected light colors, including the colors used for specular shading.
It does not clamp specular material parameters or introduce exposure control.

## Actual Android rendering path

The generic post-processing graph is retained in the original binary, but
`renderThread_processRenderScenes` (`0x2a61a0`) does not call
`NuPostEffectRender`. It does call the post-effect reset/end bookkeeping.
The reconstructed render thread preserves this distinction.

Many Android framebuffer/effect-texture exports are empty in the reference,
including `NuFramebufferCreate` (`0x2a25d0`) and `NuEffectTexCreate2D`
(`0x2ff1a0`). Their return registers are unspecified. Framebuffer attachment,
bound/default/back/front-buffer and object queries explicitly return null.
Width, height, sample-count and effect-texture dimension queries do contain
code and have been reconstructed. Dimension queries round down to an even
number after the mip shift.

The platform motion, accumulation, speed-blur and deferred resource initializers
are also empty in the original. Main-filter initialization creates the DOF
blur shader; shared initialization creates the copy shader and fullscreen
geometry. Several other program pointers remain null. Generic copy and blur
draw routines omit texture binding in this Android binary. These facts make
the retained graph unsuitable for enabling as a working native renderer.
There is no recovered tone-mapping stage to enable.

## Reconstructed surface

- Scene parameters and setters: bloom, DOF, deferred shading, camera motion,
  speed blur, and accumulation blur.
- Post-effect API: initialization, destruction, flags, parameter forwarding,
  ordered filter dispatch, cached buffer queries, timing, dynamic-light feed,
  and end-of-frame proxy resets.
- Filter object layouts and virtual dispatch, shared resource ports, texture
  and framebuffer lifecycle, fullscreen geometry and embedded Android GLSL.
- Main bloom/DOF passes, 13-tap Gaussian sampling, separable seven-tap blur,
  motion/speed/accumulation passes, and deferred composition.
- The power/log helpers used by the retained passes.

The generic filter sizes are `0x0c` (base), `0x128` (main), `0xa0` (motion),
`0x28` (accumulation), `0x20` (speed), and `0x2f8` (deferred). Proxy descriptors
are 12 bytes, with flag bytes at offsets 8 and 9. The deferred light array has
32 entries at `0x58`, with its count at `0x54`.

Reference quirks are retained: disabling `0x40` targets the motion filter,
the motion/accumulation output proxy uses attachment kind 4, and End omits
the speed filter and depth-RT proxy. The accumulation exponent is stored as
an opaque 32-bit mode word and interpreted as float by the render pass.

## Verification and limits

This is a behavioral reconstruction, not a claim of full byte matching.
Common inline GL sequences and the two seven-tap variants share source helpers;
allocation/static initialization and instruction ordering still differ.
Representative comparisons using `scripts/objdiff-cli.py` report 100% for
`NuMainFilterGen::reset` and `NuEffectTexGetDimension`, and 93.556% for
`NuPostEffectEnable` (the remaining differences are address operands).
The target and native builds and all four repository checks pass. A hidden,
muted 15-second native startup run reaches its configured timeout without
an AddressSanitizer or undefined-behavior report; the harness reports that
intentional timeout as an error exit.

The generic deferred pass calls the existing `NuDynamicLight` shadow methods;
those methods have separate, still-incomplete reconstructions. This work does
not make deferred shadows operational. The original Android no-ops stay empty,
and no new post-processing call is added to the render thread.

Native startup is checked with a hidden, muted window and a bounded timeout.
That verifies initialization, not image parity or a measured before/after
specular comparison in a level.
