#pragma once

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/NuDataPortManager.h"

struct VuVec;
struct nueffecttex_s;
struct nuframebuffer_s;
struct nushaderprogram_s;
struct numtx_s;

struct NuPostFilterGen {
    NuPostFilterGen() : enabled(false), input_fbo(NULL) {
    }
    virtual ~NuPostFilterGen() {
    }

    virtual void initResources();
    virtual void destroyResources();
    virtual void initTextureResources(i32, i32) {
    }
    virtual void destroyTextureResources() {
    }
    virtual void render() {
    }
    virtual void reset();
    virtual void resetAll();
    virtual bool isEnabled() {
        return enabled;
    }
    virtual nuframebuffer_s *getInputFbo() {
        return input_fbo;
    }

    bool enabled;
    u8 unknown_005[0x08 - 0x05];
    nuframebuffer_s *input_fbo;

    static NuDataPortManager resourceManager;
    static NuPostDataPort portOutFramebuffer, portColorBuffer, portNormalBuffer;
    static NuPostDataPort portVelocityBuffer, portDepthRTBuffer, portDepthBuffer;
    static nuframebuffer_s *blurFbo, *copyFbo;
    static nueffecttex_s *workTex;
    static nushaderprogram_s *copyTexProgram, *copyTexLodProgram, *copyTexColorDepthProgram;
    static nushaderprogram_s *blendTexProgram, *blur5x5Program, *blur7x7Program, *blurGuardProgram;

    static void GetSampleOffsets_GaussBlur5x5(i32, i32, VuVec *, float);
    static void blend(nueffecttex_s *, nueffecttex_s *, nuframebuffer_s *);
    static void blur5x5(nueffecttex_s *, i32, nueffecttex_s *, i32, i32, i32, bool);
    static void blur7x7Loopback(nueffecttex_s *, i32, nueffecttex_s *, i32, i32, i32, bool, float, nushaderprogram_s *);
    static void blur7x7Separate(nueffecttex_s *, i32, nueffecttex_s *, i32, i32, i32, bool, float, nushaderprogram_s *);
    static void copy(nueffecttex_s *, i32, nueffecttex_s *, i32, nushaderprogram_s *, nueffecttex_s *);
    static void copy(nueffecttex_s *, nueffecttex_s *, nuframebuffer_s *);
    static void copy(nueffecttex_s *, nuframebuffer_s *);
    static void copyDepth(nueffecttex_s *, nuframebuffer_s *);
    static void destroySharedResources();
    static void destroySharedTextureResources();
    static void initSharedResources();
    static void initSharedTextureResources(i32, i32);
    static void renderFrustum(numtx_s *);
    static void renderQuad();
    static void renderQuadGrid();
};
