// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "nu2api/nu3d/nutex.h"
#include <GLES2/gl2.h>
#include <GLES2/gl2ext.h>
#include <squish.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "decomp.h"
#include "nu2api/nu3d/NuRenderDevice.h"
#include "nu2api/nuandroid/ios_graphics.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nuplatform/nuplatform.h"
#include "nu2api/nucore/nuvuvec.hpp"

u32 g_currentTexUnit = -1;
i32 g_loadDefaultTexture;
GLuint g_lastBound2DTexIds[16];
GLuint g_lastBoundCubeTexIds[16];
GLuint g_earlyColorFramebuffer;
GLuint g_defaultFramebuffer;
GLuint g_currentFramebuffer;
GLuint g_earlyColorMSAAFramebuffer;
SAGA_HOST_WEAK GLuint g_earlyColorTexture = 0;

u32 g_textureHash;
bool comeFromHash;
i32 g_logoTexture;
const char *g_textureName;
i32 g_fileSize;

extern i32 g_loadingCharacterInHub;

SAGA_HOST_HOOK void NuIOS_UploadCompressedTexture(GLenum target, GLint level, GLenum internal_format, GLsizei width,
                                                  GLsizei height, GLint border, GLsizei image_size, const void *data) {
    glCompressedTexImage2D(target, level, internal_format, width, height, border, image_size, data);
}

SAGA_HOST_HOOK bool NuIOS_TextureFormatSupported(i32 format) {
    return g_renderDevice.enabled_extensions[format];
}

#define SQUISH_KDXT1 (1 << 0) // 1
#define SQUISH_KDXT3 (1 << 1) // 2
#define SQUISH_KDXT5 (1 << 2) // 4

static const i32 CSWTCH_249[] = {SQUISH_KDXT3, SQUISH_KDXT1, SQUISH_KDXT5};

SAGA_HOST_HOOK void *NuIOS_AllocateDefaultTexturePixels(usize bytes) {
    return malloc(bytes);
}

i32 GetMipOffset(i32 width, i32 height, NUTEXFORMAT format, i32 depth, bool isCubemap, i32 mips, i32 targetMip,
                 i32 targetSlice);

void GetTextureFormatInfo(NUTEXFORMAT texture, u32 &param_2, u32 &param_3) {
    param_2 = 1;
    switch (texture) {
        case NUTEX_DXT1:
        case NUTEX_DX1A:
            param_2 = 4;
            param_3 = 4;
            return;
        case NUTEX_DXT2:
        case NUTEX_DXT3:
        case NUTEX_DXT4:
        case NUTEX_ETC1:
            param_2 = 4;
            param_3 = 8;
            return;
        case NUTEX_DXT5:
            param_2 = 4;
            param_3 = 8;
            return;
        case NUTEX_RGBA32:
        case 0x72:
        case 0x74:
            param_3 = 0x20;
            return;
        case NUTEX_FLOAT16:
        case 0x6e:
        case 0x75:
        case 0x76:
            param_3 = 0x40;
            return;
        case NUTEX_FLOAT32:
        case 0x13:
            param_3 = 0x80;
            return;
        case 0x10:
            param_3 = 8;
            return;
        case NUTEX_PVRTC2:
        case NUTEX_PVRTC2A:
            param_2 = 8;
            param_3 = 2;
            return;
        case NUTEX_PVRTC4:
        case NUTEX_PVRTC4A:
            param_2 = 8;
            param_3 = 4;
            return;
        case 0x6c:
            param_3 = 0x10;
            return;
    }
}

i32 GetMipLevelSize(NUTEXFORMAT param_1, i32 unclamped_width, i32 unclamped_height) {
    u32 block_size;
    u32 bpp;

    GetTextureFormatInfo(param_1, block_size, bpp);
    u32 width = 1;
    if (unclamped_width > 0) {
        width = unclamped_width;
    }
    u32 height = unclamped_height;
    if (unclamped_height <= 0) {
        height = 1;
    }
    u32 div_w = width / block_size;
    u32 multiplier = (block_size * block_size * bpp) >> 3;
    u32 m_w = 1;
    if (div_w != 0) {
        m_w = div_w;
    }
    u32 div_h = height / block_size;
    u32 m_h = 1;
    if (div_h != 0) {
        m_h = div_h;
    }
    u32 base_area = m_w * m_h;
    return base_area * multiplier;
}

i32 GetMipOffset(NuHardwareTexture *tex, i32 targetMip, i32 targetSlice, NUTEXFORMAT overrideFormat) {
    if (tex->depth == 0) {
        tex->depth = 1;
    }

    NUTEXFORMAT format = (overrideFormat != 0) ? overrideFormat : tex->format;
    bool isAutoMode = (targetMip < 0 && targetSlice < 0);

    // Auto mode walks every mip of every slice; the limits replace the targets.
    if (isAutoMode) {
        targetMip = tex->mips;
        targetSlice = (tex->flags & 1) ? 5 : tex->depth;
    }

    i32 currentSlice = 0;
    i32 totalOffset = 0;

    while (true) {
        i32 innerMipLimit = targetMip;

        if ((tex->flags & 1) && (currentSlice < targetSlice)) {
            innerMipLimit = tex->mips;
        }

        i32 currentMip = 0;
        while (true) {
            i32 w = tex->width >> currentMip;
            i32 h = tex->height >> currentMip;

            i32 levelSize = GetMipLevelSize(format, w, h);

            if (currentMip == innerMipLimit) {
                if (!isAutoMode && tex->depth > 1) {
                    totalOffset += levelSize * targetSlice;
                }
                break;
            }

            i32 d = tex->depth >> currentMip;
            if (d < 1)
                d = 1;

            totalOffset += d * levelSize;
            currentMip++;
        }

        if (!(tex->flags & 1) || currentSlice == targetSlice) {
            break;
        }

        currentSlice++;
    }

    return totalOffset;
}

void DecompressTextureToRGBA(unsigned char *ddsData, u32 size, unsigned char *&outBuffer) {
    i32 squishFlags;
    NuMemory *v8;
    NuMemoryManager *threadMem;
    static unsigned char *buffer = nullptr;

    NuHardwareTexture tex;

    NUTEXFORMAT format;
    i32 width, height, depth;
    i32 mips;
    bool isCubemap;
    bool unkDesc;

    NuDDSGetTextureDescription((char *)ddsData, format, width, height, depth, mips, isCubemap, &unkDesc);

    squishFlags = 1;
    if ((u32)(format - 4) < 3) {
        squishFlags = CSWTCH_249[format - 4];
    }

    tex.width = width;
    tex.height = height;
    tex.format = format;
    tex.depth = depth;

    tex.flags = (tex.flags & 0x80) | (isCubemap & 1);

    tex.lp_raw_data = nullptr;
    tex.lp_device_resource = nullptr;
    tex.data_size = 0;
    tex.ref_count = 0;
    tex.checksum = 0;
    tex.unknown = 0;
    tex.mips = mips;
    tex.type = 0;

    ddsData += 128;

    GetMipOffset(&tex, -1, -1, NUTEX_RGBA32);

    if (buffer == nullptr) {
        v8 = NuMemoryGet();
        threadMem = v8->GetThreadMem();
        buffer = (unsigned char *)threadMem->_BlockAlloc(0x6000000, 4, 1, "", 0);
    }

    outBuffer = buffer;

    i32 faceLimit = isCubemap ? 6 : 1;

    for (i32 face = 0; face < faceLimit; face++) {
        for (u32 mip = 0; mip < tex.mips; mip++) {
            i32 mipWidth = tex.width >> mip;
            i32 mipHeight = tex.height >> mip;
            i32 mipOffset = GetMipOffset(&tex, mip, face, NUTEX_UNKNOWN);
            i32 rgbaOffset = GetMipOffset(&tex, mip, face, NUTEX_RGBA32);
            squish::DecompressImage(outBuffer + rgbaOffset, mipWidth, mipHeight, ddsData + mipOffset, squishFlags);
        }
    }
}

GLuint CreateTexturePS(void) {
    GLuint tex;

    tex = 0;
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x1ad);
    glGenTextures(1, &tex);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x1b1);
    return tex;
}

GLuint loadDefaultTexture(GLuint texture, GLint level, GLsizei size, GLenum texture_type, GLenum target) {
    isize pixel_count = size * size;
    u8 *pixels = (u8 *)NuIOS_AllocateDefaultTexturePixels(pixel_count * 4);
    u8 *p1 = pixels + 8;
    u8 *p2 = pixels + 4;
    for (i32 i = 0; i < pixel_count; i += 2) {
        i32 row = i / size;

        if (!(row & 1)) {
            p1[-8] = 0x7A;
            p1[-7] = 0x7A;
            p1[-6] = 0x7A;
            p1[-5] = 0xFF;
            p2[3] = 0xFF;
            p2[0] = 0x52;
            p2[1] = 0x52;
            p2[2] = 0x99;
        } else {
            p1[-8] = 0x52;
            p1[-7] = 0x52;
            p1[-6] = 0x99;
            p1[-5] = 0xFF;
            p2[3] = 0xFF;
            p2[0] = 0x7A;
            p2[1] = 0x7A;
            p2[2] = 0x7A;
        }
        p1 += 8;
        p2 += 8;
    }

    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 552);

    GLuint textures[3];
    textures[0] = texture;
    if (texture == 0) {
        glGenTextures(1, textures);
    }

    if (texture_type == GL_TEXTURE_2D) {
        glActiveTexture(GL_TEXTURE0);
        g_currentTexUnit = 0;
        glBindTexture(GL_TEXTURE_2D, textures[0]);
    } else {
        if (g_currentTexUnit != 0) {
            glActiveTexture(GL_TEXTURE0);
            g_currentTexUnit = 0;
        }
        if (g_lastBoundCubeTexIds[0] != textures[0]) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, textures[0]);
            g_lastBoundCubeTexIds[0] = textures[0];
        }
    }

    glTexImage2D(target, level, GL_RGBA, size, size, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    NuCheckGLErrorsFL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 574);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    NuCheckGLErrorsFL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 583);
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 585);
    free(pixels);

    return textures[0];
}

// PVR metadata records are packed, so their u32 fields can sit at any alignment.
typedef u32 __attribute__((aligned(1), may_alias)) pvr_unaligned_u32;

GLuint NuIOS_CreateGLTexFromPVRInMemory(void *data, i32 *out_width, i32 *out_height) {
    extern bool comeFromHash;
    extern i32 g_loadDefaultTexture;
    extern i32 g_loadingCharacterInHub;

    GLuint texture = 0;
    GLenum cube_faces[6] = {
        GL_TEXTURE_CUBE_MAP_POSITIVE_X, GL_TEXTURE_CUBE_MAP_NEGATIVE_X, GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
        GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, GL_TEXTURE_CUBE_MAP_POSITIVE_Z, GL_TEXTURE_CUBE_MAP_NEGATIVE_Z,
    };

    comeFromHash = false;

    u8 *header = (u8 *)data;
    u8 *metadata_start = header + 0x34;
    u8 *pixels = metadata_start + *(u32 *)(header + 0x30);
    const i32 &height = *(i32 *)(header + 0x18);
    const i32 &width = *(i32 *)(header + 0x1c);
    const u32 &depth = *(u32 *)(header + 0x20);
    const u32 &surfaces = *(u32 *)(header + 0x24);
    const u32 &faces = *(u32 *)(header + 0x28);
    const u32 &mip_count = *(u32 *)(header + 0x2c);

    if (faces == 6) {
        u8 *record = metadata_start;
        while (record < pixels) {
            const usize remaining = pixels - record;
            if (remaining < 12 || *(const pvr_unaligned_u32 *)(record + 8) > remaining - 12) {
                break;
            }
            if (*(const pvr_unaligned_u32 *)record == 0x03525650 && *(const pvr_unaligned_u32 *)(record + 4) == 2 && *(const pvr_unaligned_u32 *)(record + 8) == 6) {
                const u8 *metadata = record + 12;
                switch (metadata[0]) {
                    case 'X':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[0] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }

                switch (metadata[1]) {
                    case 'X':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[1] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }

                switch (metadata[2]) {
                    case 'X':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[2] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }

                switch (metadata[3]) {
                    case 'X':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[3] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }

                switch (metadata[4]) {
                    case 'X':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[4] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }

                switch (metadata[5]) {
                    case 'X':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_POSITIVE_X;
                        break;
                    case 'x':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_X;
                        break;
                    case 'Y':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_POSITIVE_Y;
                        break;
                    case 'y':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y;
                        break;
                    case 'Z':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_POSITIVE_Z;
                        break;
                    case 'z':
                        cube_faces[5] = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z;
                        break;
                }
            }
            record += *(const pvr_unaligned_u32 *)(record + 8) + 12;
        }
    }

    const u32 pixel_format = *(u32 *)(header + 0x08);
    const u32 channel_bits = *(u32 *)(header + 0x0c);
    GLenum format = 0;
    GLenum type = 0;
    i32 bits_per_pixel = 0;
    bool compressed = false;

    if (__builtin_expect(channel_bits == 0, 0)) {
        if (pixel_format >= 6) {
            return 0;
        }
        bits_per_pixel = pixel_format < 2 ? 2 : 4;
        static const GLenum compressed_formats[6] = {
            0x8c01, // GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG
            0x8c03, // GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG
            0x8c00, // GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG
            0x8c02, // GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG
            0x9137, // GL_COMPRESSED_RGBA_PVRTC_2BPPV2_IMG
            0x9138, // GL_COMPRESSED_RGBA_PVRTC_4BPPV2_IMG
        };
        format = compressed_formats[pixel_format];
        compressed = true;
    } else {
        u8 name[4];
        u8 bits[4];
        memcpy(name, header + 0x08, sizeof(name));
        memcpy(bits, header + 0x0c, sizeof(bits));
        if (name[0] == 'r' && name[1] == 'g' && name[2] == 'b' && name[3] == 'a') {
            if (bits[0] == 4 && bits[1] == 4 && bits[2] == 4 && bits[3] == 4) {
                format = GL_RGBA;
                type = GL_UNSIGNED_SHORT_4_4_4_4;
                bits_per_pixel = 16;
            } else if (bits[0] == 8 && bits[1] == 8 && bits[2] == 8 && bits[3] == 8) {
                format = GL_RGBA;
                type = GL_UNSIGNED_BYTE;
                bits_per_pixel = 32;
            }
        }
    }

    if (out_width != NULL) {
        *out_width = width;
    }
    if (out_height != NULL) {
        *out_height = height;
    }

    const GLenum texture_target = faces > 1 ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x2e7);
    glGenTextures(1, &texture);
    if (faces <= 1) {
        glActiveTexture(GL_TEXTURE0);
        g_currentTexUnit = 0;
        glBindTexture(GL_TEXTURE_2D, texture);
    } else {
        if (g_currentTexUnit != 0) {
            glActiveTexture(GL_TEXTURE0);
            g_currentTexUnit = 0;
        }
        if (g_lastBoundCubeTexIds[0] != texture) {
            glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
            g_lastBoundCubeTexIds[0] = texture;
        }
    }
    glTexParameteri(texture_target, GL_TEXTURE_MIN_FILTER, mip_count < 2 ? GL_LINEAR : GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(texture_target, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    if (faces > 1) {
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    }
    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x2fe);

    if (g_loadingCharacterInHub != 0 && bgProcIsBgThread() != 0) {
        NuIOS_YieldThread();
    }

    usize offset = 0;
    for (u32 mip = 0; mip < mip_count; ++mip) {
        for (u32 surface = 0; surface < surfaces; ++surface) {
            for (u32 face = 0; face < faces; ++face) {
                for (u32 z = 0; z < depth; ++z) {
                    const i32 mip_width = width >> mip;
                    const i32 mip_height = height >> mip;
                    const GLenum target = faces > 1 ? cube_faces[face] : GL_TEXTURE_2D;
                    usize mip_size = (usize)mip_width * (usize)mip_height * (usize)bits_per_pixel / 8;
                    if (mip_size < 0x20) {
                        mip_size = 0x20;
                    }
                    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                           0x31d);
                    if (faces <= 1) {
                        glActiveTexture(GL_TEXTURE0);
                        g_currentTexUnit = 0;
                        glBindTexture(GL_TEXTURE_2D, texture);
                    } else {
                        if (g_currentTexUnit != 0) {
                            glActiveTexture(GL_TEXTURE0);
                            g_currentTexUnit = 0;
                        }
                        if (g_lastBoundCubeTexIds[0] != texture) {
                            glBindTexture(GL_TEXTURE_CUBE_MAP, texture);
                            g_lastBoundCubeTexIds[0] = texture;
                        }
                    }

                    if (compressed) {
                        if ((NuPlatform::Get()->GetCurrentPlatform() == IOS_PLATFORM ||
                             NuPlatform::Get()->GetCurrentPlatform() == ANDROID_PVRTC_PLATFORM) &&
                            g_loadDefaultTexture == 0) {
                            glCompressedTexImage2D(target, mip, format, mip_width, mip_height, 0, mip_size,
                                                   pixels + offset);
                        } else {
                            loadDefaultTexture(texture, mip, mip_width, target, target);
                        }
                    } else {
                        glTexImage2D(target, mip, format, mip_width, mip_height, 0, format, type, pixels + offset);
                    }
                    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                         0x341);

                    if (bgProcIsBgThread() != 0) {
                        NuIOS_YieldThread();
                    }
                    offset += mip_size;
                }
            }
        }
    }

    return texture;
}

char const *GetNativeTextureFormatName(NUTEXFORMAT format) {
    switch (format) {
        case NUTEX_DXT1:
            return "NUTEX_FMT_DXT1";
        case NUTEX_DX1A:
            return "NUTEX_FMT_DXT1A";
        case NUTEX_DXT5:
            return "NUTEX_FMT_DXT5";
        case NUTEX_RGBA32:
            return "NUTEX_FMT_8888";
        case NUTEX_FLOAT16:
            return "NUTEX_FMT_A16B16G16R16F";
        case NUTEX_FLOAT32:
            return "NUTEX_FMT_A32B32G32R32F";
        case NUTEX_L8:
            return "NUTEX_FMT_L8";
        case NUTEX_ETC1:
            return "NUTEX_FMT_ETC1";
        case NUTEX_PVRTC2:
            return "NUTEX_FMT_PVRTC1_2_RGB";
        case NUTEX_PVRTC2A:
            return "NUTEX_FMT_PVRTC1_2";
        case NUTEX_PVRTC4:
            return "NUTEX_FMT_PVRTC1_4_RGB";
        case NUTEX_PVRTC4A:
            return "NUTEX_FMT_PVRTC1_4";
        case NUTEX_ATCA:
            return "NUTEX_FMT_ATITC_RGBA";
        case NUTEX_ATC:
            return "NUTEX_FMT_ATITC_RGB";
        case NUTEX_RT_RGBX32:
            return "NUTEX_FMT_RT_RGBX32";
        case NUTEX_RT_RGBA32:
            return "NUTEX_FMT_RT_RGBA32";
        case NUTEX_RT_D24S8:
            return "NUTEX_FMT_RT_D24S8";
        case NUTEX_RT_A16B16G16R16:
            return "NUTEX_FMT_RT_A16B16G16R16";
        case NUTEX_RT_R32F:
            return "NUTEX_FMT_RT_R32F";
        case NUTEX_RT_ZBUFFER:
            return "NUTEX_FMT_RT_ZBUFFER";
        case NUTEX_RT_SHADOWMAP_COLOR:
            return "NUTEX_FMT_RT_SHADOWMAP_COLOR";
        case NUTEX_RT_SHADOWMAP_DEPTH:
            return "NUTEX_FMT_RT_SHADOWMAP_DEPTH";
        case NUTEX_RT_G16R16F:
            return "NUTEX_FMT_RT_G16R16F";
        case NUTEX_RT_HDR:
            return "NUTEX_FMT_RT_HDR";
        case NUTEX_RT_A16B16G16R16F:
            return "NUTEX_FMT_RT_A16B16G16R16F";
        case NUTEX_RT_A2R10G10B10:
            return "NUTEX_FMT_RT_A2R10G10B10";
        default:
            return "Not defined";
    }
}

void GetNativeTextureFormat(NUTEXFORMAT inFormat, i32 &outBpp, u32 &outInternalFormat, u32 &outType, u32 &outFormat,
                            bool &outIsCompressed, NUTEXFORMAT &outFormatEnum) {
    i32 formatToCheck = inFormat;

    if (inFormat != NUTEX_DXT1) {
        outIsCompressed = false;
        outBpp = 0;
    } else {
        if (__builtin_expect(!!(g_renderDevice.enabled_extensions[NUTEX_DXT1]), 0)) {
            outBpp = 0;
        } else if (__builtin_expect(!!(g_renderDevice.enabled_extensions[NUTEX_ETC1]), 0)) {
            inFormat = NUTEX_ETC1;
            formatToCheck = NUTEX_ETC1;
            outBpp = 0;
        } else {
            outIsCompressed = false;
            inFormat = NUTEX_RGBA32;
            outBpp = 0;
        }
    }

    switch (inFormat) {
        case NUTEX_DXT1:                                         // 1
            outInternalFormat = GL_COMPRESSED_RGB_S3TC_DXT1_EXT; // 0x83F0
            outType = GL_UNSIGNED_BYTE;
            outIsCompressed = true; // 0x1907
            outFormat = GL_RGB;     // 0x1401
            break;

        case NUTEX_DX1A:                                          // 2
            outInternalFormat = GL_COMPRESSED_RGBA_S3TC_DXT1_EXT; // 0x83F1
            outFormat = GL_RGBA;
            outType = GL_UNSIGNED_BYTE; // 0x1908
            outIsCompressed = true;     // 0x1401
            break;

        case NUTEX_DXT5:                 // 6
            outInternalFormat = GL_RGBA; // 0x1908
            outType = GL_UNSIGNED_BYTE;
            outFormat = GL_RGBA;    // 0x1908
            outIsCompressed = true; // 0x1401
            break;

        case 0x10:
            outInternalFormat = GL_LUMINANCE; // 0x1909
            outType = GL_UNSIGNED_BYTE;
            outFormat = GL_LUMINANCE; // 0x1909
            outBpp = 8;               // 0x1401
            outFormatEnum = inFormat;
            return;

        case NUTEX_ETC1:                          // 0x11
            outInternalFormat = GL_ETC1_RGB8_OES; // 0x8D64
            outFormat = GL_RGB;
            outType = GL_UNSIGNED_BYTE; // 0x1907
            outIsCompressed = true;     // 0x1401
            break;

        case NUTEX_PVRTC2:                                          // 0x14
            outInternalFormat = GL_COMPRESSED_RGB_PVRTC_2BPPV1_IMG; // 0x8C01
            outIsCompressed = true;
            outFormat = GL_RGB;         // 0x1907
            outType = GL_UNSIGNED_BYTE; // 0x1401
            break;

        case NUTEX_PVRTC2A:                                          // 0x15
            outInternalFormat = GL_COMPRESSED_RGBA_PVRTC_2BPPV1_IMG; // 0x8C03
            outType = GL_UNSIGNED_BYTE;
            outIsCompressed = true; // 0x1908
            outFormat = GL_RGBA;    // 0x1401
            break;

        case NUTEX_PVRTC4:                                          // 0x16
            outInternalFormat = GL_COMPRESSED_RGB_PVRTC_4BPPV1_IMG; // 0x8C00
            outFormat = GL_RGB;
            outIsCompressed = true;     // 0x1907
            outType = GL_UNSIGNED_BYTE; // 0x1401
            break;

        case NUTEX_PVRTC4A:                                          // 0x17
            outInternalFormat = GL_COMPRESSED_RGBA_PVRTC_4BPPV1_IMG; // 0x8C02
            outIsCompressed = true;
            outFormat = GL_RGBA;        // 0x1908
            outType = GL_UNSIGNED_BYTE; // 0x1401
            break;

        case NUTEX_ATCA:                                            // 0x18
            outInternalFormat = GL_ATC_RGBA_INTERPOLATED_ALPHA_AMD; // 0x87EE
            outFormat = GL_RGBA;
            outIsCompressed = true;     // 0x1908
            outType = GL_UNSIGNED_BYTE; // 0x1401
            break;

        case NUTEX_ATC:                         // 0x19
            outInternalFormat = GL_ATC_RGB_AMD; // 0x8C92
            outFormat = GL_RGB;
            outIsCompressed = true;     // 0x1907
            outType = GL_UNSIGNED_BYTE; // 0x1401
            break;

        case 0x66:
            outInternalFormat = GL_RGB; // 0x1907
            outFormat = GL_RGB;         // 0x18
            outType = GL_UNSIGNED_BYTE; // 0x1907
            outBpp = 24;                // 0x1401
            outFormatEnum = inFormat;
            return;

        case 0x69:
        case 0x72:
            outFormatEnum = inFormat;
            return;

        default:
            outInternalFormat = GL_RGBA;
            outType = GL_UNSIGNED_BYTE;
            outFormat = GL_RGBA;
            outFormatEnum = inFormat;
            outBpp = 32;
            return;
    }

    if (__builtin_expect(!NuIOS_TextureFormatSupported(formatToCheck), 1)) {
        while (true) {
        }
    }

    outFormatEnum = inFormat;
}

i32 GetMipOffset(i32 width, i32 height, NUTEXFORMAT format, i32 depth, bool isCubemap, i32 mips, i32 targetMip,
                 i32 targetSlice) {
    i32 totalOffset = 0;
    i32 currentSlice = 0;
    bool isAuto;

    if (depth == 0) {
        depth = 1;
    }

    isAuto = targetMip < 0 && targetSlice < 0;

    u8 isCompressed;
    i32 bytesPerBlockOrPixel;
    i32 blockWidth;
    i32 minBlocks;

    if (format > 0 && format < 119) {
        const i32 formatIndex = format - 1;
        isCompressed = FormatIsCompressedTable[formatIndex];
        minBlocks = static_cast<i8>(FormatMinBlocksYTable[formatIndex]);
        bytesPerBlockOrPixel = static_cast<i8>(FormatBytesPerElementTable[formatIndex]);
        blockWidth = static_cast<i8>(FormatBlockWidthTable[formatIndex]);
    } else {
        isCompressed = false;
        minBlocks = 1;
        bytesPerBlockOrPixel = 0;
        blockWidth = 4;
    }

    if (isAuto != 0) {
        targetSlice = isCubemap ? 5 : depth;
        targetMip = mips;
    }

    while (true) {
        i32 innerMipLimit = targetMip;

        if (isCubemap && (currentSlice < targetSlice)) {
            innerMipLimit = mips;
        }

        i32 m = 0;

        while (true) {
            i32 w = width >> m;
            if (w < 1)
                w = 1;

            i32 h = height >> m;
            if (h < 1)
                h = 1;

            i32 levelSize;

            if (!isCompressed) {
                levelSize = w * h * bytesPerBlockOrPixel;
            } else {
                i32 blocksY = h >> 2;
                if (blocksY < minBlocks)
                    blocksY = minBlocks;

                i32 blocksX = w / blockWidth;
                if (blocksX < minBlocks)
                    blocksX = minBlocks;

                levelSize = blocksX * blocksY * bytesPerBlockOrPixel;
            }

            if (m == innerMipLimit) {
                if (isAuto == 0 && depth > 1) {
                    totalOffset += levelSize * targetSlice;
                }
                break;
            }

            i32 d = depth >> m;
            if (d <= 0)
                d = 1;

            totalOffset += d * levelSize;
            m++;
        }

        if (!isCubemap || (currentSlice == targetSlice)) {
            break;
        }

        currentSlice++;
    }

    return totalOffset;
}

void UnlockTexturePS(u32 texID, void *pixels, i32 width, i32 height, i32 depth, bool isCubemap, i32 mips,
                     NUTEXFORMAT format, u32 &glFormat, u32 &glInternalFormat, u32 glType, bool isCompressed) {
    const i32 lastSlice = isCubemap ? 5 : 0;
    const i32 faceCount = isCubemap ? 6 : 1;

    for (i32 faceTarget = GL_TEXTURE_CUBE_MAP_POSITIVE_X; faceTarget - GL_TEXTURE_CUBE_MAP_POSITIVE_X < faceCount;
         ++faceTarget) {
        if (mips != 0) {
            u32 texTarget = GL_TEXTURE_2D;
            if (isCubemap) {
                texTarget = faceTarget;
            }

            i32 mipWidth;
            i32 hLimit;

            for (i32 mip = 0; mip != mips; ++mip) {
                mipWidth = width >> mip;
                mipWidth = mipWidth > 0 ? mipWidth : 1;

                i32 mipHeight = height >> mip;
                hLimit = mipHeight > 0 ? mipHeight : 1;

                i32 currentOffset = GetMipOffset(width, height, format, depth, isCubemap, mips, mip,
                                                 faceTarget - GL_TEXTURE_CUBE_MAP_POSITIVE_X);

                unsigned char *mipData = (unsigned char *)pixels + currentOffset;

                if (!isCompressed) {
                    BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                           0x58e);

                    u32 bindTarget = isCubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
                    glBindTexture(bindTarget, texID);

                    if (g_loadDefaultTexture == 0) {
                        glTexImage2D(texTarget, mip, glInternalFormat, mipWidth, hLimit, 0, glFormat, glType, mipData);
                    } else {
                        loadDefaultTexture(texID, mip, mipWidth, GL_TEXTURE_2D, GL_TEXTURE_2D);
                    }
                    EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                         0x5b3);
                } else {
                    i32 nextOffset;
                    i32 sizeOffset;
                    i32 targetSlice;

                    if (mips - 1 == mip) {
                        nextOffset = GetMipOffset(width, height, format, depth, isCubemap, mips, -1, -1);
                        sizeOffset = mips - 1;
                        targetSlice = lastSlice;
                    } else {
                        nextOffset = GetMipOffset(width, height, format, depth, isCubemap, mips, mip + 1, 0);
                        sizeOffset = mip;
                        targetSlice = 0;
                    }

                    sizeOffset = GetMipOffset(width, height, format, depth, isCubemap, mips, sizeOffset, targetSlice);
                    i32 mipSize = nextOffset - sizeOffset;

                    if (isCubemap) {
                        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                               0x54b);
                        glBindTexture(GL_TEXTURE_CUBE_MAP, texID);
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                        if (g_loadDefaultTexture != 0) {
                            loadDefaultTexture(texID, mip, mipWidth, GL_TEXTURE_CUBE_MAP, faceTarget);
                        } else {
                            NuIOS_UploadCompressedTexture(faceTarget, mip, glInternalFormat, mipWidth, hLimit, 0,
                                                          mipSize, mipData);
                        }
                        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                             0x566);

                        g_loadDefaultTexture = 0;
                    } else {
                        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                               0x56e);
                        glBindTexture(GL_TEXTURE_2D, texID);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                        if (g_loadDefaultTexture != 0) {
                            loadDefaultTexture(texID, mip, mipWidth, GL_TEXTURE_2D, GL_TEXTURE_2D);
                        } else {
                            NuIOS_UploadCompressedTexture(GL_TEXTURE_2D, mip, glInternalFormat, mipWidth, hLimit, 0,
                                                          mipSize, mipData);
                        }
                        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp",
                                             0x589);
                    }
                }
                if (__builtin_expect(hLimit == 1 && mipWidth == 1, 0)) {
                    break;
                }
            }
        }
    }

    if (!isCompressed) {
        BeginCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x5bf);
        u32 bindTarget = isCubemap ? GL_TEXTURE_CUBE_MAP : GL_TEXTURE_2D;
        glGenerateMipmap(bindTarget);
        EndCriticalSectionGL("i:/SagaTouch-Android_9176564/nu2api.saga/nu3d/android/nutex_ios_ex.cpp", 0x5c1);
    }

    glFlush();
    g_loadDefaultTexture = 0;
}

GLuint NuIOS_CreateGLTexFromMemoryDDS(void *ddsPointer, i32 *out_width, i32 *out_height) {
    GLuint texID = 0;
    unsigned char *decompressedBuffer = nullptr;
    unsigned char *pixelData = nullptr;

    NUTEXFORMAT format;
    i32 mipCount = 0;
    i32 depth = 0;
    bool isCubemap = false;
    bool hasFourCC = false;

    bool success = NuDDSGetTextureDescription((char *)ddsPointer, format, *out_width, *out_height, depth, mipCount,
                                              isCubemap, &hasFourCC);

    if (success && (*out_width != 0 || *out_height != 0 || mipCount > 1)) {
        texID = CreateTexturePS();
        i32 bpp;
        u32 glInternalFormat;
        u32 glType;
        u32 glFormat;
        bool isCompressed;
        NUTEXFORMAT nativeFormat;

        GetNativeTextureFormat(format, bpp, glInternalFormat, glType, glFormat, isCompressed, nativeFormat);

        pixelData = (unsigned char *)ddsPointer + sizeof(dds_header_s);

        if (format == NUTEX_DXT5) {
            decompressedBuffer = nullptr;
            DecompressTextureToRGBA((unsigned char *)ddsPointer, 0, decompressedBuffer);

            format = NUTEX_RGBA32;
            isCompressed = false;
            glInternalFormat = GL_RGBA;
            glFormat = GL_RGBA;

            pixelData = decompressedBuffer;
        }
        UnlockTexturePS(texID, pixelData, *out_width, *out_height, depth, isCubemap, mipCount, format, glFormat,
                        glInternalFormat, glType, isCompressed);
    }

    return texID;
}

GLuint NuIOS_CreateGLTexFromPlatformInMemory(void *data, i32 *width, i32 *height, bool is_pvrtc) {
    const PLATFORMS_SUPPORTED platform = NuPlatform::Get()->GetCurrentPlatform();
    GLuint texture = 0;
    if (__builtin_expect(!!(is_pvrtc), 1)) {
        texture = NuIOS_CreateGLTexFromPVRInMemory(data, width, height);
    } else {
        switch (platform) {
            case IOS_PLATFORM:
            case ANDROID_PVRTC_PLATFORM:
                texture = NuIOS_CreateGLTexFromPVRInMemory(data, width, height);
                break;
            case ANDROID_ATITC_PLATFORM:
            case ANDROID_S3TC_PLATFORM:
            case ANDROID_ETC1_PLATFORM:
                texture = NuIOS_CreateGLTexFromMemoryDDS(data, width, height);
                break;
            default:
                break;
        }
    }
    if (texture != 0) {
        return texture;
    }
    return loadDefaultTexture(0, 0, 0x20, GL_TEXTURE_2D, GL_TEXTURE_2D);
}

GLuint NuIOS_CreateGLTexFromPlatfomSpecificFile(const char *filename) {
    static u8 buffer[0x600081];
    NUFILE file = NuFileOpen((char *)filename, NUFILE_READ);
    g_textureName = filename;
    if (file == 0) {
        return 0;
    }

    i32 remaining = NuFileOpenSize(file);
    g_fileSize = remaining;
    NuThreadCriticalSectionBegin(g_textureLoadBufferCriticalSection);
    const i32 chunk_limit = g_loadingCharacterInHub != 0 ? 0x4000 : static_cast<i32>(sizeof(buffer));
    u8 *dst = buffer;
    do {
        i32 chunk;
        if (bgProcIsBgThread() != 0) {
            chunk = remaining > chunk_limit ? chunk_limit : remaining;
            remaining -= chunk;
        } else {
            chunk = remaining;
            remaining = 0;
        }
        NuFileRead(file, dst, chunk);
        dst += chunk;
        if (g_loadingCharacterInHub != 0 && bgProcIsBgThread() != 0) {
            NuIOS_YieldThread();
        }
    } while (remaining != 0);
    NuFileOpenSize(file);
    NuFileClose(file);

    GLuint texture;
    {
        i32 width = 0;
        i32 height = 0;
        texture = NuIOS_CreateGLTexFromPlatformInMemory(buffer, &width, &height, false);
    }
    NuThreadCriticalSectionEnd(g_textureLoadBufferCriticalSection);
    return texture;
}

GLuint NuIOS_CreateGLTexFromPlatfomSpecificForecPVR(const char *filename) {
    static u8 buffer[0x20004c];
    NUFILE file = NuFileOpen((char *)filename, NUFILE_READ);
    if (file == 0) {
        return 0;
    }

    i32 remaining = NuFileOpenSize(file);
    NuThreadCriticalSectionBegin(g_textureLoadBufferCriticalSection);
    const i32 chunk_limit = g_loadingCharacterInHub != 0 ? 0x4000 : static_cast<i32>(sizeof(buffer));
    u8 *dst = buffer;
    do {
        i32 chunk;
        if (bgProcIsBgThread() != 0) {
            chunk = remaining > chunk_limit ? chunk_limit : remaining;
            remaining -= chunk;
        } else {
            chunk = remaining;
            remaining = 0;
        }
        NuFileRead(file, dst, chunk);
        dst += chunk;
        if (g_loadingCharacterInHub != 0 && bgProcIsBgThread() != 0) {
            NuIOS_YieldThread();
        }
    } while (remaining != 0);
    NuFileOpenSize(file);
    NuFileClose(file);

    GLuint texture = NuIOS_CreateGLTexFromPlatformInMemory(buffer, nullptr, nullptr, true);
    NuThreadCriticalSectionEnd(g_textureLoadBufferCriticalSection);
    return texture;
}

GLuint NuIOS_CreateGLTexFromFile(const char *filename) {
    switch (NuPlatform::Get()->GetCurrentPlatform()) {
        case ANDROID_ATITC_PLATFORM:
        case ANDROID_S3TC_PLATFORM:
        case ANDROID_ETC1_PLATFORM:
            break;
        case ANDROID_PVRTC_PLATFORM:
            return NuIOS_CreateGLTexFromPlatfomSpecificFile(filename);
        default:
            return 0;
    }

    char fixed_filename[0x200];
    char extension[0x200];
    NuStrCpy(extension, NuPlatform::Get()->GetCurrentTextureExtension());
    NuStrFixExtPlatform(fixed_filename, const_cast<char *>(filename), extension, sizeof(fixed_filename),
                        const_cast<char *>("MOB"));

    if (g_datfileMode == 0) {
        char external_filename[0x200] = "mnt/sdcard/TTGames/com.tt.LegoStarWarsSaga/files/androidTextures/";
        const usize path_length = strlen(external_filename);
        strcpy(&external_filename[path_length], fixed_filename);
        for (usize index = path_length; index < strlen(external_filename); ++index) {
            i32 character = external_filename[index];
            SAGA_UPPERCASE_CHAR(character, _toupper_tab_);
            external_filename[index] = static_cast<char>(character);
            if (external_filename[index] == '\\') {
                external_filename[index] = '/';
            }
        }
        NuStrCpy(fixed_filename, external_filename);
    }

    GLuint texture = NuIOS_CreateGLTexFromPlatfomSpecificFile(fixed_filename);
    if (__builtin_expect(texture != 0, 1)) {
        return texture;
    }
    return NuIOS_CreateGLTexFromPlatfomSpecificForecPVR(filename);
}

GLuint NuIOS_CreateGLTexFromHash(u32 hash) {
    char filename[0x10c];
    GLuint texture;
    const char *extension;

    comeFromHash = true;
    g_textureHash = hash;
    if (hash == 0x280145f0) {
        g_logoTexture = 1;
    }

    switch (NuPlatform::Get()->GetCurrentPlatform()) {
        case IOS_PLATFORM:
        case ANDROID_PVRTC_PLATFORM:
            sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, "PVR");
            texture = NuIOS_CreateGLTexFromPlatfomSpecificFile(filename);
            break;
        case ANDROID_ATITC_PLATFORM:
            sprintf(filename, "SHAREDTEXTURES/0x%08x.%s", hash, "atitc");
            texture = NuIOS_CreateGLTexFromPlatfomSpecificFile(filename);
            if (texture == 0) {
                sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, "PVRNC");
                texture = NuIOS_CreateGLTexFromPlatfomSpecificForecPVR(filename);
            }
            break;
        case ANDROID_S3TC_PLATFORM:
            extension = "S3TC";
            goto load_dds_format;
        case ANDROID_ETC1_PLATFORM: {
            extension = "ETC1";
        load_dds_format:
            sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, extension);
            texture = NuIOS_CreateGLTexFromPlatfomSpecificFile(filename);
            if (texture == 0) {
                sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, "PVRNC");
                texture = NuIOS_CreateGLTexFromPlatfomSpecificForecPVR(filename);
            }
            break;
        }
        default:
            sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, "PVR");
            texture = NuIOS_CreateGLTexFromPlatfomSpecificFile(filename);
            if (texture == 0) {
                sprintf(filename, "SHAREDTEXTURES/0X%08X.%s", hash, "PVRNC");
                texture = NuIOS_CreateGLTexFromPlatfomSpecificForecPVR(filename);
            }
            break;
    }

    g_textureHash = 0;
    g_logoTexture = 0;
    return texture;
}

