// Original translation unit reconstructed from the reference address map.
// Functions are defined in original address order.

// The original unit constructs the homogeneous VuVec header constants.
#include "nu2api/nucore/nuvuvec.hpp"

#include "decomp.h"
#include "nu2api/nucore/implode.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "nu2api/nucore/common.h"
#include "nu2api/nucore/nuapi.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/nuportal.h"
#include "nu2api/nu3d/nuwater.h"
#include "nu2api/nucore/nukeyboard.h"
#include "nu2api/nucore/numouse.h"
#include "nu2api/nucore/numem.h"
#include "nu2api/nucore/nuheap.h"
#include "nu2api/nucore/numemory.h"
#include "nu2api/nucore/numem_types.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/deflate.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/nucore/nuthread.h"
#include "nu2api/nuplatform/nuplatform.h"
#include "nu2api/nufile/nufilepak.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nucore/nurdp.h"
#include "nu2api/nucore/nulist.h"
#include <stddef.h>
#include "nu2api/nucore/nuqt.h"
#include "nu2api/nucore/nutime.h"
#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/android/nupostfilter.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nuonline.h"
#include "gamelib/nuwind/nuwind.h"
#include "nu2api/numath/nuvec4.h"
#include <math.h>
#include <float.h>
#include <GLES2/gl2.h>
#include "nu2api/numath/nufloat.h"
#include "nu2api/nu3d/android/nutex_ios_ex.h"
#include "java/java.h"
#include "nu2api/nucore/nuhgobj.h"
#include "nu2api/nucore/nupad.h"
#include "nu2api/nucore/bgproc.h"
#include "nu2api/nucore/nuptrblock.h"
#include "nu2api/nu3d/nurndrstat.h"
#include "nu2api/numath/numtx.h"
#include "globals.h"
#include "nu2api/nucore/NuMemoryManager.h"
#include "nu2api/nucore/nuvideo.h"
#include "nu2api/nu3d/android/numtl_android.h"
#include "nu2api/nu3d/nu3d_includes.h"
#include "nu2api/nucore/android/bgproc_android.h"
#include "nu2api/nucore/android/nuerror_android.h"
#include "nu2api/nucore/nucore_includes.h"

static i32 gExplodeInitialised = 1;

char *implode_inbuffer;
char *implode_outbuffer;
usize implode_origsize;
usize implode_compsize;

static i32 bytes_to_copy;
static void decode_start();
static void decode(u32 size, unsigned char *buf);
static unsigned char buffer[0x2000];

static u32 getmasktbl[5] = {0, 0xff, 0xffff, 0xffffff, 0xffffffff};

#define SELECT(dst, table, shift, count, initial_mask, len)                                                            \
    ({                                                                                                                 \
        (dst) = (table)[implode_bitbuf >> shift];                                                                      \
                                                                                                                       \
        if ((dst) > (count)) {                                                                                         \
            u32 mask = (initial_mask);                                                                                 \
                                                                                                                       \
            do {                                                                                                       \
                if ((implode_bitbuf & mask) != 0) {                                                                    \
                    (dst) = implode_right[(dst)];                                                                      \
                } else {                                                                                               \
                    (dst) = implode_left[(dst)];                                                                       \
                }                                                                                                      \
                                                                                                                       \
                mask >>= 1;                                                                                            \
            } while ((dst) > (count));                                                                                 \
        }                                                                                                              \
                                                                                                                       \
        ImplodeFillBuf((len)[(dst)]);                                                                                  \
    })

static u32 blocksize;
static u16 offset_table[0x100];
static u8 offset_len[0x13];
static u16 literal_table[0x1000];
static u8 literal_len[0x1fe];

u16 implode_left[0x3fb];
u16 implode_right[0x3fb];

u32 implode_bitbuf;
static u32 subbitbuf;
static u32 bitcount;

// Encoding-side memory I/O shares the decoder's input/output buffers.
static i32 inlen;

// The encoder and decoder share the Huffman branch arrays. The encoder builds
// a frequency heap, limits code lengths to 16 bits, then assigns canonical codes.
static i32 n;
static i32 heapsize;
static i16 heap[511];
static u16 *freq;
static u16 *sortptr;
static u16 len_cnt[17];
static unsigned char *len;
static NUMEMALLOCATION *alloc_list;
static NUMEMEXTERNAL *memexternal;
static NUMEMEXTERNAL memext;
static NUMEMDISCARDABLE *discardbuff;
static isize peakallocaddr;
static i32 totalloc;

extern "C" {
    void *NuMem_Heap;
    extern i32 highallocaddr;
}

void memmove(void *dest, const void *source, i32 size);

struct NUHEAPBLOCK {
    u32 size_and_flags;
    NUHEAPBLOCK *next_free;
    NUHEAPBLOCK *previous_free;
};

struct NUHEAP {
    void *base;
    u32 size;
    u32 allocated_blocks;
    u32 allocated_bytes;
};

enum NUHEAPBLOCKFLAGS { NUHEAPBLOCK_IN_USE = 0x80000000u, NUHEAPBLOCK_SIZE_MASK = 0x7fffffffu };

#ifdef ANDROID
#include "nu2api/nufile/android/nufile_android.h"
#endif

i32 read_critical_section = -1;

static NUDATHDR *curr_dat;
i32 nufile_try_packed = 0;
static FILEINFO file_info[33];
i32 nufile_buffering_enabled;
static i32 nufile_lasterror = 0;
NUDATFILEINFO dat_file_infos[20];

NUFILE_DEVICE host_device = {
    0,
    0,
    0xe,
    0,
    '/',

    0x400,
    -1,
    -1,

    -1,
    {0, 0, 0, 0},
    "d:",
    2,
    2,
    "",
    "Host Xbox 360 filesystem",
    "",
    "",
    "",

    &DEV_FormatName,
    &DEVHOST_Interrogate,
};

NUFILE_DEVICE *default_device = &host_device;

static i32 file_time_count;
static FILEBUFF file_buff[4];

i32 numdevices = 0;

NUFILE_DEVICE devices[16] = {0};
NUFILE_DEVICE enum_devices[16];
i32 enum_numdevices;

i32 file_criticalsection;

i32 DontDoFileUpperCaseHack;

i32 NuFile_SwapEndianOnWrite;

static NUDATFILEINFO *unpack_file_info;
static NUDATOPENFILEINFO *unpack_file_odi;
static i64 unpack_file_pos;
static i32 decode_buffer_left;
static char decode_buffer[0x40000];
static i32 decode_buffer_pos;
static char read_buffer[0x40000];
// Volatile in the original: -O0 comparisons of these emit setcc/test/jcc.
static volatile i32 read_buffer_size;
static volatile i32 read_buffer_decoded_size;

NUMEMFILE memfiles[20];

struct NUPPHEADER {
    u32 magic;
    u32 unpacked_size;
    u32 packed_size;
};

#define NUPP_SWAP32(value)                                                                                             \
    (((value) >> 24) | (((value) & 0x00ff0000) >> 8) | (((value) & 0x0000ff00) << 8) | ((value) << 24))

// The original decoder accesses both whole registers and their low bytes.
// Keep those views together to preserve its byte stores at the file's O0 setting.
union NUPPREG {
    u32 value;
    u16 word;
    u8 byte;
};
union NUPPADDR {
    uintptr_t value;
    u8 *bytes;
};

static FILEEXTINFO extensions[64];
static i32 num_extensions;
static void NuFileExtInit(void);

char *iop_image_name = "ioprp300.img";

struct NuFileAddress {
    void *address;
    i64 position;
};

static i32 MAX_FILE_ADDRESS;
static NuFileAddress *addr_ref;
static NuFileAddress *addr_pos;
static i32 ref_count;
static i32 pos_count;

struct NuFileWriteBlock {
    i32 tag;
    i32 size_sign;
    i64 position;
};

static NuFileWriteBlock nufile_wblkinfo[1024];
static i32 wblkcnt = 1;
static i32 iff_padsize = 16;
i32 nufile_lsn_allowed = 1;

struct NuFileReadBlock {
    i32 tag;
    i32 size;
    i64 position;
};
static NuFileReadBlock nufile_blkinfo[1024];
static i32 blkcnt = 1;

// The original unit constructs its six homogeneous vectors.

i32 datsys_offset = 0;

static NUFILEPAK_ERROR fpk_err;

#define PARSER_COUNT 4
#define PARSER_BUF_SIZE 0x1000

static NUFPAR _fpars[PARSER_COUNT];
static char _fparbuffers[PARSER_COUNT * PARSER_BUF_SIZE];
static NUFPAR *fpars;
static char *fparbuffers;
static i32 old_line_pos;

#define CLAMP_LINE(pos) pos &(parser->line_buf_size - 1)
#define CLAMP_WORD(pos) pos &(parser->word_buf_size - 1)
#define CLAMP_WIDE_LINE(pos) pos &((parser->line_buf_size >> 1) - 1)
#define CLAMP_WIDE_WORD(pos) pos &((parser->word_buf_size >> 1) - 1)

nufpcomfn *fnInterpreterError;

struct NUMEMHIGHBLOCK {
    NUMEMHIGHBLOCK *next;
    u32 units;
    u8 reserved[8];
};

static NUMEMHIGHBLOCK *freep;

typedef struct rdpfctxt_s {
    i32 cur_tok;
    f32 cur_val;
    char *buf;
    i32 pos;
    nurdpgetvarfn *get_var_fn;
} RDPFCTXT;

enum {
    TOK_ADD = 0,
    TOK_SUB = 1,
    TOK_MUL = 2,
    TOK_DIV = 3,
    TOK_LPAREN = 4,
    TOK_RPAREN = 5,
    TOK_END = 6,
    TOK_NUM = 7,
    TOK_VAR = 8,
    TOK_CNT = 9,
};

static f32 f_expr(RDPFCTXT *ctx);

i32 global_tr;

static NUSTRINGBANK StringBank[3];
static i16 CurrentStringBank;
static i32 nustring_format = 1;
char *NuBlankString = " ";

static u16 bad_equiv[][2] = {{'a', '@'}, {'a', '4'}, {'e', '3'}, {'i', '1'},
                             {'l', '1'}, {'o', '0'}, {'t', '7'}, {0, 0}};
static u16 bad_fluff[] = {'!', 0xffa3, '$', '%', '^', '&', '*', '(', ')', '_', '+', '-', '=',
                          '{', '}',    '[', ']', '@', '~', '#', '?', '<', '>', ',', '.', ' '};
static NUWCHAR **BadWords;
static i32 NumBadWords;
static u32 frameStartTime;

NUAPI nuapi;

i32 nuapi_use_target_manager;
char *nuapi_target_manager_mac_address;
i32 nuapi_deferspotfxload;
i32 nuapi_use_target_manager_for_sound;

float nuapi_forced_frame_time;
i32 nuapi_max_fps = 60;

static i32 NUAPI_PADREC_DEFAULT_BUFFERSIZE = 0x500000;
/* NEEDS-REVIEW: static variable shared with src/nu2api/nucore/nuapi.cpp */
// The Android command-line provider is empty. These are still the original
// private cursors consumed by ParseCommandLine, not host process arguments.
static i32 argc;
/* NEEDS-REVIEW: static variable shared with src/nu2api/nucore/nuapi.cpp */
static char **argv;

extern "C" {
    extern void (*preRenderFlashingHack)();
    extern void (*postRenderFlashingHack)();
    extern void (*nuapi_endframe_callbackfn)();
    static i32 min_delay = 20;
f32 NuFrameEnd(void);

}

void NuPadRecordEndFrame(void);                      // nupad_interface.cpp

extern "C" {
    extern "C++" {
        static __attribute__((noinline, noclone)) void
        DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry, NUDISPLAYLISTITEM **first_and_last,
                               NUMTX *world_matrix, void **transform_packet, NUMTX *skin_matrices,
                               DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster)
            __asm__("_ZL22DisplayListProcessSkinP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvS7_P22deformerweightsarray_si.isra.36");
static void DisplayListProcessSkin(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                           NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                           void **transform_packet, NUMTX *skin_matrices,
                                           DEFORMERWEIGHTSARRAY *deformer_weights, i32 shadow_caster);

        static __attribute__((noinline, noclone)) void
        DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                      NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix, void **transform_packet,
                                      f32 alpha)
            __asm__("_ZL29DisplayListProcessLightmappedP7numtl_sP15nudisplaylist_sP19nudisplaylistitem_sPS4_P7numtx_"
                    "sPPvf.isra.34");
static void DisplayListProcessLightmapped(NUDISPLAYLIST *list, NUDISPLAYLISTITEM *geometry,
                                                  NUDISPLAYLISTITEM **first_and_last, NUMTX *world_matrix,
                                                  void **transform_packet, f32 alpha);
    }

    extern void (*nuapi_endframe_callbackfn)(void);

#include "nu2api/nucore/nucore_plain_types.h"
#undef NUGCUT_CURVE_VALUE

    // ---------------------------------------------------------------------------
    // Culling / visibility / portals / occlusion
    // ---------------------------------------------------------------------------

    extern "C++" {
        static __attribute__((noinline, noclone)) void
        ClipInstTree(NuInstanceTree_s *tree,
                     i32 tree_index) __asm__("_ZL12ClipInstTreeP16NuInstanceTree_siP16nudisplayscene_sPc.isra.92");
    }

#ifndef ANDROID
    void NuGetCurrentThreadId(void) {
        STUBBED();
    }
#endif
#ifndef ANDROID
    void NuThreadCreate(void) {
        STUBBED();
    }
#endif

    extern void (*nuapi_endframe_callbackfn)(void);
} // extern "C"

#define MAX_GAME_PAD_COUNT 2

#define ANALOG_CENTER 0x80

static NUGENERICPAD g_nupadScannedPads[MAX_GAME_PAD_COUNT];
static i32 g_atLeastOnePadBeenActivated;
static i32 g_directpadMapping;

NUPADMAPPING g_nupadMapping[MAX_GAME_PAD_COUNT];
i32 g_profilePlayerPad = -1;
i32 MaxGamePads = MAX_GAME_PAD_COUNT;
i16 PlayerLastPort[8] = {-1, -1, -1, -1, -1, -1, -1, -1};
char UseCorrectDeadZoning;
i32 enable_touch_controls = 1;

static f32 ramp_vals[][2] = {{0.0f, 0.0f}, {0.5f, 0.1f}, {0.75f, 0.25f}, {0.85f, 0.5f}, {1.0f, 1.0f}};

static f32 ramp_rate = 0.1f;

enum {
    PAD_BUTTON_JUMP_SELECT = 1 << 0x6,
    PAD_BUTTON_START = 1 << 0xb,
    PAD_BUTTON_UP = 1 << 0xc,
    PAD_BUTTON_RIGHT = 1 << 0xd,
    PAD_BUTTON_DOWN = 1 << 0xe,
    PAD_BUTTON_LEFT = 1 << 0xf,
};

typedef struct rdpictxt_s {
    i32 cur_tok;
    i32 cur_val;
    char *buf;
    i32 pos;
    nurdpgetvarfn *get_var_fn;
} RDPICTXT;

enum {
    TOKI_ADD = 0,
    TOKI_SUB = 1,
    TOKI_MUL = 2,
    TOKI_DIV = 3,
    TOKI_MOD = 4,
    TOKI_LPAREN = 5,
    TOKI_RPAREN = 6,
    TOKI_END = 7,
    TOKI_NUM = 8,
    TOKI_OR = 9,
    TOKI_AND = 10,
    TOKI_NOT = 11,
    TOKI_XOR = 12,
    TOKI_LOGICAL_OR = 13,
    TOKI_LOGICAL_AND = 14,
    TOKI_LOGICAL_NOT = 15,
    TOKI_EQ = 16,
    TOKI_NEQ = 17,
    TOKI_LT = 18,
    TOKI_GT = 19,
    TOKI_LT_EQ = 20,
    TOKI_GT_EQ = 21,
    TOKI_CNT = 22,
};

static i32 i_expr(RDPICTXT *ctx);

extern "C" {
    typedef void (*NuErrorFunctionPtr)(char *, ...);
    typedef void (*NuDebugTTYFunctionPtr)(i32, char *, ...);
    static i32 bHaveErr;
    // Volatile in the original: NuErrorCheck keeps the otherwise unused load and test.
    static volatile i32 nuerror_status;
    static char ErrMsg[1024];
    static char *nufile;
    static i32 nuline;
    static char txt[16384];
    static char captxt[16384];
    static char err_msg_filter[32];
    void (*nuerror_handler)(char *);
    void (*nudebug_handler)(char *);
    void (*nuwarning_handler)(char *);
    i32 DisableDebugMsg;

}

extern "C" {
static void NuPrimReset() {
}
}

void NuMemSetHeap(void *heap) {
    NuMem_Heap = heap;
}

NUMEMDISCARDABLE *NuMemSetDiscardable(NUMEMDISCARDABLE *buffer) {
    NUMEMDISCARDABLE *previous = discardbuff;
    discardbuff = buffer;
    return previous;
}

void NuMemFlushDiscardable(NUMEMDISCARDABLE *buffer) {
    if (buffer != NULL) {
        NUMEMDISCARDABLE *pool = buffer;
        pool->cursor = (u8 *)(pool + 1);
        pool->remaining = pool->capacity;
    }
}

NUMEMDISCARDABLE *NuMemCreateDiscardable(i32 size) {
    NUMEMDISCARDABLE *previous = discardbuff;
    discardbuff = NULL;
    NUMEMDISCARDABLE *pool =
        (NUMEMDISCARDABLE *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(size + sizeof(NUMEMDISCARDABLE), 4, 1, "", 0);
    if (pool != NULL) {
        pool->capacity = size;
        pool->remaining = size;
        pool->cursor = (u8 *)(pool + 1);
    }
    discardbuff = previous;
    return pool;
}

void NuMemDestroyDiscardable(NUMEMDISCARDABLE *buffer) {
    NU_FREE(buffer);
}

void NuMemSetExternal(VARIPTR *cursor, VARIPTR *end) {
    if (cursor != NULL) {
        memexternal = &memext;
        memexternal->cursor = cursor;
        if (end != NULL) {
            memexternal->end = *end;
        }
    } else {
        memexternal = NULL;
    }
}

isize NuMemGetPeakAllocAddr(void) {
    return peakallocaddr;
}

void *NuMemAlloc(i32 size) {
    void *block;
    isize end;
    if (memexternal != NULL) {
        memexternal->cursor->addr = (memexternal->cursor->addr + 15) & ~(usize)15;
        if (memexternal->end.addr - memexternal->cursor->addr >= (usize)size) {
            block = memexternal->cursor->void_ptr;
            memexternal->cursor->addr += size;
            return block;
        }
        return NULL;
    }
    if (discardbuff != NULL) {
        size = (size + 15) & ~15;
        if (discardbuff->remaining > size) {
            block = discardbuff->cursor;
            discardbuff->cursor += size;
            discardbuff->remaining -= size;
            return block;
        }
        return NULL;
    }
    totalloc += size;
    if (NuMem_Heap != NULL) {
        block = NuHeapAllocAligned(NuMem_Heap, size, 4);
    } else {
        block = malloc(size);
    }
    memset(block, 0, size);
    end = (isize)block + size;
    if (end > highallocaddr)
        highallocaddr = end;
    if (end > peakallocaddr)
        peakallocaddr = end;
    return block;
}

void NuMemFree(void *ptr) {
    if (NuMem_Heap != NULL) {
        NuHeapFree(NuMem_Heap, ptr);
    } else {
        free(ptr);
    }
}

void *NuMemValidateFn(void) {
    i32 result;
    NUMEMALLOCATION *block;
    VARIPTR tail;
    void *tail_data;
    block = alloc_list;
    while (block) {
        tail.void_ptr = block + 1;
        tail.addr += block->size;
        // The original also computes the data following the tail guard;
        // its diagnostic reporting body is absent in this build.
        tail_data = (u8 *)tail.void_ptr + 12;
        result = memcmp(block->guard, "MEMBLKHEADER", 12);
        if (result != 0) {
            return block;
        }
        result = memcmp(tail.void_ptr, "MEMBLKTAIL01", 12);
        if (result != 0) {
            return tail.void_ptr;
        }
        block = block->next;
    }
    return NULL;
}

void NuMemDumpFn(i32 mode) {
    NUMEMALLOCATION *block = alloc_list;
    VARIPTR tail;
    void *tail_data;
    // The Android binary retains the traversal and metadata calculations,
    // but contains no calls to print the allocation records.
    while (block) {
        tail.void_ptr = block + 1;
        tail.addr += block->size;
        tail_data = (u8 *)tail.void_ptr + 12;
        block = block->next;
    }
}

void NuMemFlushFn(void) {
    NUMEMALLOCATION *block = alloc_list;
    while (block) {
        NUMEMALLOCATION *next = block->next;
        free(block);
        block = next;
    }
}

void memmove(void *dest, const void *source, i32 size) {
    u8 *out = (u8 *)dest;
    const u8 *in = (const u8 *)source;
    if (out < in) {
        while (size != 0) {
            size--;
            *out = *in;
            out++;
            in++;
        }
    } else {
        out += size;
        in += size;
        while (size != 0) {
            size--;
            out--;
            in--;
            *out = *in;
        }
    }
}

i32 numeminit() {
    return 0;
}

NUMEMEXTERNAL *NuMemGetExternal(void) {
    return memexternal;
}

void NuMemSet128(void *ptr, u8 value, isize size) {
    memset(ptr, value, size << 4);
}

void NuMemCopy128(void *dest, const void *source, i32 count) {
    memmove(dest, source, count << 4);
}

static void NuHeapBlock_SetInUse(NUHEAPBLOCK *block, u32 in_use) {
    if (in_use) {
        block->size_and_flags |= NUHEAPBLOCK_IN_USE;
    } else {
        block->size_and_flags &= NUHEAPBLOCK_SIZE_MASK;
    }
}

static u32 NuHeapBlock_GetInUse(NUHEAPBLOCK *block) {
    if (block->size_and_flags & NUHEAPBLOCK_IN_USE)
        return 1;
    return 0;
}

static void NuHeapBlock_SetSize(NUHEAPBLOCK *block, u32 size) {
    block->size_and_flags &= NUHEAPBLOCK_IN_USE;
    block->size_and_flags |= size;
}

static u32 NuHeapBlock_GetSize(NUHEAPBLOCK *block) {
    return block->size_and_flags & NUHEAPBLOCK_SIZE_MASK;
}

static void NuHeapBlock_SetNextFree(NUHEAPBLOCK *block, NUHEAPBLOCK *next) {
    block->next_free = next;
}

static NUHEAPBLOCK *NuHeapBlock_GetNextFree(NUHEAPBLOCK *block) {
    return block->next_free;
}

static void NuHeapBlock_SetPrevFree(NUHEAPBLOCK *block, NUHEAPBLOCK *previous) {
    block->previous_free = previous;
}

static void *NuHeapBlock_GetUsableMemory(NUHEAPBLOCK *block) {
    return block + 1;
}

static void NuHeapBlock_WriteFooter(NUHEAPBLOCK *block) {
    *(NUHEAPBLOCK **)((u8 *)block + sizeof(NUHEAPBLOCK) + NuHeapBlock_GetSize(block)) = block;
}

static void NuHeapBlock_RemoveFromFreeList(NUHEAPBLOCK *block) {
    if (block->previous_free != NULL) {
        block->previous_free->next_free = block->next_free;
    }
    if (block->next_free != NULL) {
        block->next_free->previous_free = block->previous_free;
    }
}

static void NuHeapBlock_SetName(NUHEAPBLOCK *block, char *name) {
    STUBBED();
    // Block names are disabled in the original Android build.
}

static void NuHeapBlock_SplitFreeBlock(NUHEAPBLOCK *block, u32 *size, NUHEAPBLOCK **allocated, NUHEAPBLOCK **remainder,
                                       u32 alignment) {
    u32 remaining_size;
    u8 *base = (u8 *)block;
    u32 total_size = NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    usize address;
    usize aligned_address;
    remaining_size = total_size - *size - 2 * (sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    *remainder = (NUHEAPBLOCK *)base;
    *allocated = (NUHEAPBLOCK *)(base + remaining_size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    if (alignment != 0) {
        address = (usize)(*allocated + 1);
        aligned_address = (alignment + address - 1) & -(usize)alignment;
        *allocated = (NUHEAPBLOCK *)((u8 *)*allocated + (aligned_address - address));
        remaining_size += aligned_address - address;
        *size += address - aligned_address;
    }
    NuHeapBlock_SetInUse(*allocated, 1);
    NuHeapBlock_SetSize(*allocated, *size);
    NuHeapBlock_SetNextFree(*allocated, NULL);
    NuHeapBlock_SetPrevFree(*allocated, NULL);
    NuHeapBlock_WriteFooter(*allocated);
    NuHeapBlock_SetName(*allocated, NULL);
    NuHeapBlock_SetInUse(*remainder, 0);
    NuHeapBlock_SetSize(*remainder, remaining_size);
    NuHeapBlock_SetNextFree(*remainder, NULL);
    NuHeapBlock_SetPrevFree(*remainder, NULL);
    NuHeapBlock_WriteFooter(*remainder);
    NuHeapBlock_SetName(*remainder, NULL);
}

static NUHEAPBLOCK *NuHeapBlock_GetNext(NUHEAPBLOCK *block) {
    u8 *next = (u8 *)block;
    next += NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    return (NUHEAPBLOCK *)next;
}

static NUHEAPBLOCK *NuHeapBlock_GetPrev(NUHEAPBLOCK *block) {
    u8 *footer = (u8 *)block;
    footer -= sizeof(NUHEAPBLOCK *);
    return *(NUHEAPBLOCK **)footer;
}

void *NuHeapCreate(VARIPTR *buffer, void *buffer_end, u32 size) {
    u8 *base;
    NUHEAP *control;
    NUHEAPBLOCK *free_head;
    NUHEAPBLOCK *start;
    NUHEAPBLOCK *first;
    NUHEAPBLOCK *end;
    enum { block_overhead = sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *) };

    if (size < sizeof(NUHEAP) + 4 * block_overhead)
        return NULL;
    base = (u8 *)buffer->void_ptr;
    buffer->addr += size;
    control = (NUHEAP *)base;
    free_head = (NUHEAPBLOCK *)(base + sizeof(NUHEAP));
    start = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + block_overhead);
    first = (NUHEAPBLOCK *)(base + sizeof(NUHEAP) + 2 * block_overhead);
    end = (NUHEAPBLOCK *)(base + size - block_overhead);

    NuHeapBlock_SetInUse(free_head, 0);
    NuHeapBlock_SetSize(free_head, 0);
    NuHeapBlock_SetNextFree(free_head, first);
    NuHeapBlock_SetPrevFree(free_head, NULL);
    NuHeapBlock_WriteFooter(free_head);
    NuHeapBlock_SetName(free_head, NULL);

    NuHeapBlock_SetInUse(start, 1);
    NuHeapBlock_SetSize(start, 0);
    NuHeapBlock_SetNextFree(start, NULL);
    NuHeapBlock_SetPrevFree(start, NULL);
    NuHeapBlock_WriteFooter(start);
    NuHeapBlock_SetName(start, NULL);

    NuHeapBlock_SetInUse(end, 1);
    NuHeapBlock_SetSize(end, 0);
    NuHeapBlock_SetNextFree(end, NULL);
    NuHeapBlock_SetPrevFree(end, NULL);
    NuHeapBlock_WriteFooter(end);
    NuHeapBlock_SetName(end, NULL);

    NuHeapBlock_SetInUse(first, 0);
    NuHeapBlock_SetSize(first, size - sizeof(NUHEAP) - 4 * block_overhead);
    NuHeapBlock_SetNextFree(first, NULL);
    NuHeapBlock_SetPrevFree(first, free_head);
    NuHeapBlock_WriteFooter(first);
    NuHeapBlock_SetName(first, NULL);

    control->base = base;
    control->size = size;
    control->allocated_blocks = 0;
    control->allocated_bytes = 0;
    return control;
}

static void NuHeap_PrintAllocations(void *heap) {
    STUBBED();
    // Allocation reporting is disabled in the original Android build.
}

void NuHeapDestroy(void *heap) {
    NuHeap_PrintAllocations(heap);
}

void *NuHeapAlloc(void *heap, u32 size) {
    return NuHeapAllocAlignedNamed(heap, size, 4, NULL);
}

void *NuHeapAllocNamed(void *heap, u32 size, char *name) {
    return NuHeapAllocAlignedNamed(heap, size, 4, name);
}

void *NuHeapAllocAligned(void *heap, u32 size, u32 alignment) {
    return NuHeapAllocAlignedNamed(heap, size, alignment, NULL);
}

void *NuHeapAllocAlignedNamed(void *heap, u32 size, u32 alignment, char *name) {
    NUHEAPBLOCK *block;
    NUHEAP *control = (NUHEAP *)heap;
    NUHEAPBLOCK *free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    u32 required_size;
    void *result;
    NUHEAPBLOCK *remainder;
    NUHEAPBLOCK *allocated;
    if (size == 0)
        return NULL;
    size += alignment;
    required_size = size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    block = free_head;
    while (true) {
        if (block == NULL)
            return NULL;
        if (NuHeapBlock_GetSize(block) > required_size)
            break;
        block = NuHeapBlock_GetNextFree(block);
    }
    NuHeapBlock_RemoveFromFreeList(block);
    NuHeapBlock_SplitFreeBlock(block, &size, &allocated, &remainder, alignment);
    NuHeapBlock_SetName(allocated, name);
    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, remainder);
    }
    NuHeapBlock_SetNextFree(remainder, free_head->next_free);
    NuHeapBlock_SetPrevFree(remainder, free_head);
    NuHeapBlock_SetNextFree(free_head, remainder);
    control->allocated_blocks++;
    control->allocated_bytes += NuHeapBlock_GetSize(allocated) + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    result = NuHeapBlock_GetUsableMemory(allocated);
    return result;
}

void NuHeapFree(void *heap, void *ptr) {
    NUHEAP *control = (NUHEAP *)heap;
    NUHEAPBLOCK *free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    u8 *address = (u8 *)ptr;
    NUHEAPBLOCK *block;
    NUHEAPBLOCK *next;
    NUHEAPBLOCK *previous;

    if (ptr == NULL)
        return;
    if (address < (u8 *)control->base || address > (u8 *)control->base + control->size)
        return;
    block = (NUHEAPBLOCK *)(address - sizeof(NUHEAPBLOCK));
    NuHeapBlock_SetInUse(block, 0);
    control->allocated_blocks--;
    control->allocated_bytes -= NuHeapBlock_GetSize(block);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK);
    control->allocated_bytes -= sizeof(NUHEAPBLOCK *);

    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, block);
    }
    NuHeapBlock_SetNextFree(block, free_head->next_free);
    NuHeapBlock_SetPrevFree(block, free_head);
    NuHeapBlock_SetNextFree(free_head, block);

    next = NuHeapBlock_GetNext(block);
    if (!NuHeapBlock_GetInUse(next)) {
        NuHeapBlock_RemoveFromFreeList(next);
        NuHeapBlock_SetSize(block, NuHeapBlock_GetSize(block) + NuHeapBlock_GetSize(next) + sizeof(NUHEAPBLOCK) +
                                       sizeof(NUHEAPBLOCK *));
        NuHeapBlock_WriteFooter(block);
    }
    previous = NuHeapBlock_GetPrev(block);
    if (!NuHeapBlock_GetInUse(previous)) {
        NuHeapBlock_RemoveFromFreeList(block);
        NuHeapBlock_SetSize(previous, NuHeapBlock_GetSize(previous) + NuHeapBlock_GetSize(block) + sizeof(NUHEAPBLOCK) +
                                          sizeof(NUHEAPBLOCK *));
        NuHeapBlock_WriteFooter(previous);
    }
}

u32 NuHeapGetAllocatedBlockCount(void *heap) {
    NUHEAP *control = (NUHEAP *)heap;
    return control->allocated_blocks;
}

u32 NuHeapGetFreeBlockCount(void *heap) {
    NUHEAPBLOCK *block;
    u32 count;
    NUHEAP *control = (NUHEAP *)heap;
    block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    count = 0;
    while (true) {
        if (block == NULL)
            break;
        count++;
        block = NuHeapBlock_GetNextFree(block);
    }
    return count;
}

u32 NuHeapGetTotalAllocated(void *heap) {
    NUHEAP *control = (NUHEAP *)heap;
    return control->allocated_bytes;
}

u32 NuHeapGetTotalFree(void *heap) {
    NUHEAPBLOCK *block;
    u32 size;
    NUHEAP *control = (NUHEAP *)heap;
    block = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    size = 0;
    while (true) {
        if (block == NULL)
            break;
        size += NuHeapBlock_GetSize(block);
        block = NuHeapBlock_GetNextFree(block);
    }
    return size;
}

void *NuHeapDefragAllocation(void *heap, void *ptr) {
    i32 i;
    NUHEAP *control = (NUHEAP *)heap;
    u8 *address = (u8 *)ptr;
    NUHEAPBLOCK *block;
    NUHEAPBLOCK *next;
    u32 size;
    u32 next_size;
    u8 *source;
    u8 *destination;
    void *result;
    NUHEAPBLOCK *free_head;
    if (ptr == NULL)
        return NULL;
    block = (NUHEAPBLOCK *)(address - sizeof(NUHEAPBLOCK));
    next = NuHeapBlock_GetNext(block);
    if (NuHeapBlock_GetInUse(next))
        return ptr;
    size = NuHeapBlock_GetSize(block);
    next_size = NuHeapBlock_GetSize(next);
    NuHeapBlock_RemoveFromFreeList(next);
    source = (u8 *)NuHeapBlock_GetUsableMemory(block);
    destination = address + next_size + 2 * sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *);
    for (i = size - 1; i >= 0; i--) {
        destination[i] = source[i];
    }
    // These offsets are relative to the supplied payload pointer in the
    // original, including the position of the replacement free header.
    block = (NUHEAPBLOCK *)(address + next_size + sizeof(NUHEAPBLOCK) + sizeof(NUHEAPBLOCK *));
    NuHeapBlock_SetInUse(block, 1);
    NuHeapBlock_SetSize(block, size);
    NuHeapBlock_SetNextFree(block, NULL);
    NuHeapBlock_SetPrevFree(block, NULL);
    NuHeapBlock_WriteFooter(block);
    result = NuHeapBlock_GetUsableMemory(block);
    block = (NUHEAPBLOCK *)address;
    NuHeapBlock_SetInUse(block, 0);
    NuHeapBlock_SetSize(block, next_size);
    NuHeapBlock_SetNextFree(block, NULL);
    NuHeapBlock_SetPrevFree(block, NULL);
    NuHeapBlock_WriteFooter(block);
    free_head = (NUHEAPBLOCK *)((u8 *)control->base + sizeof(NUHEAP));
    if (free_head->next_free != NULL) {
        NuHeapBlock_SetPrevFree(free_head->next_free, block);
    }
    NuHeapBlock_SetNextFree(block, free_head->next_free);
    NuHeapBlock_SetPrevFree(block, free_head);
    NuHeapBlock_SetNextFree(free_head, block);
    return result;
}

static void NuDatFileDecodeInit() {
    unpack_file_info = NULL;
    unpack_file_odi = NULL;
    unpack_file_pos = -1;
}

static void NuDatFileDecodeNewFile(NUDATFILEINFO *file, NUDATOPENFILEINFO *open_file) {
    unpack_file_info = file;
    unpack_file_odi = open_file;
    unpack_file_pos = file->start;
    decode_buffer_left = 0;
}

static void NuDatFileDecodeNext() {
    char char_buf[12];
    i32 int_buf[3];
    i32 compressed_size;
    NUDATFILEINFO *info;
    NUDATOPENFILEINFO *open_info;

    switch (unpack_file_info->compression_mode) {
        case 2:
            NuFileRead(unpack_file_odi->dat_file, char_buf, 0xc);

            read_buffer_decoded_size = ExplodeBufferSize(char_buf);
            read_buffer_size = ExplodeCompressedSize(char_buf) - 0xc;
            NuFileRead(unpack_file_odi->dat_file, read_buffer, read_buffer_size);

            unpack_file_info->pos += read_buffer_size + 0xc;
            unpack_file_odi->pos = unpack_file_info->pos;
            break;
        case 3:
            NuFileRead(unpack_file_odi->dat_file, int_buf, 0xc);

            read_buffer_decoded_size = int_buf[2];
            read_buffer_size = int_buf[1];
            NuFileRead(unpack_file_odi->dat_file, read_buffer, read_buffer_size);

            unpack_file_info->pos += read_buffer_size + 0xc;
            unpack_file_odi->pos = unpack_file_info->pos;
    }
}

static void NuFileEndianSwap16(void *value) {
    u8 *value_ptr;
    u16 swapped;
    u8 *swap_ptr;

    value_ptr = (u8 *)value;
    swap_ptr = (u8 *)&swapped;
    swap_ptr[0] = value_ptr[1];
    swap_ptr[1] = value_ptr[0];
    *((u16 *)value) = swapped;
}

static void NuFileEndianSwap32(void *value) {
    u8 *value_ptr;
    u32 swapped;
    u8 *swap_ptr;

    value_ptr = (u8 *)value;
    swap_ptr = (u8 *)&swapped;

    swap_ptr[0] = value_ptr[3];
    swap_ptr[1] = value_ptr[2];
    swap_ptr[2] = value_ptr[1];
    swap_ptr[3] = value_ptr[0];

    *((u32 *)value) = swapped;
}

extern "C" {
    i32 NuFileSwapEndianOnWrite(i32 enabled) {
        i32 previous = NuFile_SwapEndianOnWrite;
        NuFile_SwapEndianOnWrite = enabled;
        return previous;
    }

    i32 NuFileGetEndianSwap(void) {
        return NuFile_SwapEndianOnWrite;
    }

}

void NuFileUpCase(NUFILE_DEVICE *device, char *filepath) {
    char separator;

    if (device != NULL) {
        separator = device->dir_separator;
    } else {
        separator = '\\';
    }

    if (DontDoFileUpperCaseHack) {
        for (; *filepath != '\0'; filepath++) {
            if (*filepath == '\\' || *filepath == '/') {
                *filepath = separator;
            }
        }
    } else {
        for (; *filepath != '\0'; filepath++) {
            switch (*filepath) {
                case 'a' ... 'z':
                    *filepath -= 0x20;
                    break;
                case '/':
                case '\\':
                    *filepath = separator;
                    break;
            }
        }
    }
}

void NuFileCorrectSlashes(NUFILE_DEVICE *device, char *path) {
    char separator;

    if (device != NULL) {
        separator = device->dir_separator;
    } else {
        separator = '\\';
    }

    for (; *path != '\0'; path++) {
        if (*path == '\\' || *path == '/') {
            *path = separator;
        }
    }
}

void NuFileReldirFix(NUFILE_DEVICE *device, char *path) {
    char *cursor;
    char *inner;

    cursor = inner = path;

    while (*cursor != '\0') {
        if (*cursor == device->dir_separator && cursor[1] == '.' && cursor[2] == '.' &&
            cursor[3] == device->dir_separator) {
            inner = cursor;

            while (inner > path) {
                inner--;
                if (*inner == device->dir_separator) {
                    cursor += 3;
                    break;
                }

                if (*inner == ':') {
                    inner++;
                    cursor += 4;
                    break;
                }
            }
        }

        *inner = *cursor;
        inner++;
        cursor++;
    }

    *inner = '\0';
}

extern "C" {
extern "C" i32 NuFileGetLastError(void) {
    return nufile_lasterror;
}

}

NUFILE_DEVICE *AddDevice(NUFILE_DEVICE *device) {
    devices[numdevices] = *device;
    NuStrCpy(devices[numdevices].cur_dir, default_device->cur_dir);
    NuStrCpy(devices[numdevices].sys_dir, default_device->sys_dir);
    NuStrCpy(devices[numdevices].dll_dir, default_device->dll_dir);
    ++numdevices;
    return &devices[numdevices - 1];
}

NUFILE_DEVICE *NuFileFindDevice(i32 id, i32 unit) {
    if (id == -3) {
        return default_device;
    }
    if (id == -2 || id == -1) {
        id = 2;
    }
    for (i32 index = 0; index < numdevices; ++index) {
        if (devices[index].id == id && (unit < 0 || devices[index].unit_id == unit)) {
            return &devices[index];
        }
    }
    return NULL;
}

i32 NuFileEnumerateDevices(NUFILE_DEVICE **result) {
    i32 count = 0;
    for (i32 index = 0; index < numdevices; ++index) {
        if ((devices[index].interrogate_fn(&devices[index]) & NUFILE_DEVICE_STATUS_EXCLUDE_FROM_ENUMERATION) != 0) {
            continue;
        }
        enum_devices[count] = devices[index];
        ++count;
    }
    *result = enum_devices;
    enum_numdevices = count;
    return count;
}

i32 NuFileGetDevices(NUFILE_DEVICE **result) {
    if (result != NULL) {
        *result = enum_devices;
    }
    return enum_numdevices;
}

i32 NuFileRefreshDevices(NUFILE_DEVICE **result) {
    for (i32 index = 0; index < numdevices; ++index) {
        if (devices[index].id == 1) {
            i32 port = devices[index].params[0];
            i32 slot = devices[index].params[1];
            if (NuMcCheckCardPresent(port, slot) != 0) {
                if (NuMcCheckCardFormatted(port, slot) != 0) {
                    devices[index].attr |= 8;
                }
                devices[index].free_space = NuMcCheckCardFreeSpace(port, slot);
            } else {
                devices[index].id = -1;
            }
        }
    }
    if (result != NULL) {
        *result = devices;
    }
    return numdevices;
}

NUFILE_DEVICE *NuFileGetDeviceFromPath(char *path) {
    NUFILE_DEVICE *device;
    i32 i;
    i32 device_idx;

    device = NULL;

    for (i = 0; i < 8; i++) {
        if (path[i] == ':') {
            break;
        }
    }

    if (i < 8) {
        for (device_idx = 0; device_idx < numdevices; device_idx++) {
            if (NuStrNICmp(path, devices[device_idx].name, devices[device_idx].match_len) == 0) {
                device = &devices[device_idx];
                break;
            }
        }
    }

    return device;
}

i32 NuFileInitEx(i32 device_id, i32 reboot_iop, i32 eject) {
    NuPSFileInitDevices(device_id, reboot_iop, eject);

    memset(memfiles, 0, sizeof(memfiles));
    memset(dat_file_infos, 0, sizeof(dat_file_infos));

    file_criticalsection = NuThreadCreateCriticalSection();

    NuFileExtInit();
    NuDatFileDecodeInit();

    return device_id;
}

void NuFileInit(i32 device_id) {
    NuFileInitEx(device_id, 1, 0);
}

extern "C" {
    i32 NuFileGetMediaMode(void) {
        STUBBED();
        return 0;
    }

    NUFILE_DEVICE *NuFileGetCurrentDevice(void) {
        return default_device;
    }

    void NuFileSetCurrentDevice(NUFILE_DEVICE *device) {
        default_device = device;
    }

    i32 NuFileGetCurrentPath(char *dest) {
        return default_device->format_name_fn(default_device, dest, "", 0x7fff);
    }

    i32 NuFileGetCurrentDirectory(char *dest) {
        return NuStrCpy(dest, default_device->cur_dir);
    }

    void NuFileSetCurrentDirectory(char *path) {
        i32 length;
        char separator[2] = "\\";
        if (*path != '\0') {
            length = NuStrCpy(default_device->cur_dir, path);
            if (length != 0 && path[length - 1] != '\\' && path[length - 1] != '/') {
                separator[0] = default_device->dir_separator;
                NuStrCat(default_device->cur_dir, separator);
            }
        } else {
            default_device->cur_dir[0] = '\0';
        }
    }

    i32 NuFileGetCurrentSysPath(char *dest) {
        return default_device->format_name_fn(default_device, dest, "", 0x7fff);
    }

    void NuFileSetCurrentSysDirectory(char *path) {
        char *source;
        char *dest;
        char *separator = "\\";
        if (*path != '\0') {
            NuStrCpy(default_device->sys_dir, path);
            NuStrCat(default_device->sys_dir, separator);
        } else {
            NuStrCpy(default_device->sys_dir, "sys");
            source = iop_image_name + 5;
            dest = default_device->sys_dir + strlen(default_device->sys_dir);
            while (*source != '.') {
                *dest = *source;
                ++dest;
                ++source;
            }
            *dest++ = '\\';
            *dest = '\0';
        }
        NuFileUpCase(default_device, default_device->sys_dir);
    }

    void NuFileGetCurrentDllPath(char *dest) {
        NuStrCpy(dest, default_device->root);
        NuStrCat(dest, default_device->dll_dir);
        NuFileUpCase(default_device, dest + NuStrLen(default_device->root));
    }

    void NuFileSetCurrentDllDirectory(char *path) {
        char *separator = "\\";
        if (*path != '\0') {
            NuStrCpy(default_device->dll_dir, path);
            NuStrCat(default_device->dll_dir, separator);
        } else {
            default_device->dll_dir[0] = '\0';
        }
    }

    i32 NuFileFormatName(char *dest, char *name, i32 capacity) {
        return default_device->format_name_fn(default_device, dest, name, capacity);
    }

}

i32 NuMemFileOpenSize(NUFILE file) {
    file -= 0x400;

    return memfiles[file].end - memfiles[file].buffer;
}

i32 NuMcFileOpenSize(NUFILE file) {
    file -= 0x1000;
    return NuMcOpenSize(file);
}

i64 NuFileOpenSize(NUFILE file) {
#ifdef ANDROID
    if (file >= 0x2000) {
        return NuFileAndroidAPK::GetFileSize(file);
    }
#endif

    if (file >= 0x1000) {
        return NuMcFileOpenSize(file);
    }

    if (file >= 0x800) {
        return NuDatFileOpenSize(file);
    }

    if (file >= 0x400) {
        return NuMemFileOpenSize(file);
    }

    file -= 1;

    return file_info[file].file_len;
}

NUFILE NuFileOpenDF(char *filepath, NUFILEMODE mode, NUDATHDR *header, i32 _unused) {
    LOG_DEBUG("filepath=%s, mode=%d", filepath, mode);

    char buf[256];
    i64 len;
    NUFILE_DEVICE *device;
    i32 mc_slot;
    i32 mc_port;
    i32 ps_index;
    i32 file_index;
    NUFILE file_handle;

    device = NuFileGetDeviceFromPath(filepath);
    if (device == NULL) {
        if (mode != NUFILE_WRITE && mode != NUFILE_APPEND) {
            file_handle = 0;

            if (header != NULL) {
                file_handle = NuDatFileOpen(header, filepath, mode);
            } else {
#ifdef ANDROID
                if (g_apkFileDevice != NULL) {
                    file_handle = NuFileAndroidAPK::OpenFile(filepath, NuFile::OpenMode::READ);
                }
#endif
            }

            if (file_handle > 0) {
                return file_handle;
            }
        }

        device = default_device;
    }

    if ((device->attr & 1) != 0 && mode == NUFILE_WRITE && mode == NUFILE_APPEND) {
        device = &host_device;
    }

    (*device->format_name_fn)(device, buf, filepath, 0x100);

    if (device->id == 2) {
        NuStrCat(buf, ";1");
    }

    if (device->id == 1) {
        mc_port = buf[2] - 0x30;
        mc_slot = buf[3] - 0x30;

        file_index = NuMcOpen(mc_port, mc_slot, buf + device->name_len, mode, 0);
        if (file_index >= 0) {
            file_index += 0x1000;
        } else {
            file_index = 0;
        }

        return file_index;
    } else {
        file_index = NuPSFileOpen(buf, mode);
        if (file_index < 0) {
            return 0;
        }

        ps_index = file_index;

        memset(&file_info[ps_index], 0, sizeof(FILEINFO));

        file_info[ps_index].handle = file_index;
        file_info[ps_index].mode = mode;

        if (mode != NUFILE_WRITE) {
            if (file_index <= 16) {
                do {
                    len = NuFileSeek(ps_index + 1, 0, NUFILE_SEEK_END);
                } while (len < 0);

                file_info[ps_index].file_len = len;

                if (mode == NUFILE_READ_NOWAIT) {
                    while (NuFileStatus(ps_index + 1) != 0) {
                    }
                }

                do {
                    len = NuFileSeek(ps_index + 1, 0, NUFILE_SEEK_START);
                } while (len < 0);

                if (mode == NUFILE_READ_NOWAIT) {
                    while (NuFileStatus(ps_index + 1) != 0) {
                    }
                }
            }
        } else {
            file_info[ps_index].file_len = 0;
        }

        if (mode == NUFILE_READ) {
            file_info[ps_index].use_buf = nufile_buffering_enabled;
        }
    }

    return ps_index + 1;
}

NUFILE NuFileOpen(char *filepath, NUFILEMODE mode) {
    return NuFileOpenDF(filepath, mode, curr_dat, 0);
}

void NuFileClose(NUFILE file) {
    FILEINFO *info;

#ifdef ANDROID
    if (file >= 0x2000) {
        NuFileAndroidAPK::CloseFile(file);
        return;
    }
#endif

    if (file >= 0x1000) {
        NuMcClose(file - 0x1000, 0);
        return;
    }

    if (file >= 0x400) {
        NuMemFileClose(file);
        return;
    }

    file -= 1;

    while (NuPSFileClose(file) < 0) {
    }

    info = &file_info[file];
    if (info->buf != NULL) {
        info->buf->file_info = NULL;
    }

    memset(info, 0, sizeof(fileinfo_s));
}

i32 NuFileStatus(NUFILE file) {
    i32 _unused;
    i32 _unused2;
    NUDATFILEINFO *info;
    NUDATOPENFILEINFO *open_info;

    _unused = 0;

    if (file >= 0x2000) {
        do {
        } while (true);
    }

    if (file >= 0x400) {
        return 0;
    }

    if (file >= 0x800) {
        file -= 0x800;
        info = dat_file_infos + file;
        open_info = &info->hdr->open_files[info->open_file_idx];

        return NuFileStatus(open_info->dat_file);
    }

    file -= 1;
    _unused2 = 0;

    return _unused;
}

static void AquireFileBuffer(FILEINFO *info) {
    i32 i;
    i32 buf_idx;
    i32 time;
    FILEBUFF *buf;
    i32 bytes_read;

    buf_idx = 0;
    time = file_time_count;

    for (i = 0; i < 4; i++) {
        if (file_buff[i].time < time) {
            time = file_buff[i].time;
            buf_idx = i;
        }
    }

    buf = &file_buff[buf_idx];
    if (buf->file_info != NULL) {
        buf->file_info->buf = NULL;
    }

    buf->file_info = info;
    buf->time = file_time_count++;
    info->buf = buf;

    if (info->buf_len != 0) {
        NuPSFileLSeek(info->handle, -info->buf_len, NUFILE_SEEK_CURRENT);
        bytes_read = NuPSFileRead(info->handle, buf->data, info->buf_len);
    }
}

NUFILE NuMemFileOpen(void *buf, i32 buf_size, NUFILEMODE mode) {
    i32 i;

    if (buf_size > 0 && (mode == NUFILE_READ || mode == NUFILE_WRITE)) {
        for (i = 0; i < 20; i++) {
            if (!memfiles[i].used) {
                memfiles[i].buffer = (char *)buf;
                memfiles[i].end = (char *)buf + buf_size - 1;
                memfiles[i].ptr = memfiles[i].buffer;
                memfiles[i].mode = mode;
                memfiles[i].used = 1;

                return i + 0x400;
            }
        }
    }

    return 0;
}

void MemFileBoundsCheck(i32 file) {
    i32 remaining = memfiles[file].end - memfiles[file].ptr + 1;
}

void NuMemFileClose(NUFILE file) {
    if (file >= 0x2000) {
        do {
        } while (true);
    }

    if (file >= 0x800) {
        NuDatFileClose(file);
    } else {
        file -= 0x400;
        memfiles[file].used = 0;
    }
}

i64 NuMemFilePos(NUFILE file) {
    if (file >= 0x800) {
        return NuDatFilePos(file);
    }

    file -= 0x400;

    return (i64)(memfiles[file].ptr - memfiles[file].buffer);
}

void *NuMemFileAddr(NUFILE file) {
    file -= 0x400;

    return (void *)memfiles[file].ptr;
}

i32 NuMemFileRead(NUFILE file, void *buf, i32 size) {
    i32 remaining;

    if (file >= 0x800) {
        return NuDatFileRead(file, buf, size);
    }

    file -= 0x400;

    size = MIN(memfiles[file].end - memfiles[file].ptr + 1, size);

    if (size != 0) {
        memcpy(buf, memfiles[file].ptr, size);

        memfiles[file].ptr = memfiles[file].ptr + size;
    }

    return size;
}

i32 NuMemFileWrite(NUFILE file, void *data, i32 size) {
    file -= 0x400;

    size = MIN(memfiles[file].end - memfiles[file].ptr + 1, size);

    if (size != 0) {
        memcpy(memfiles[file].ptr, data, size);

        memfiles[file].ptr += size;
    }

    return size;
}

i64 NuMemFileSeek(NUFILE file, i64 offset, NUFILESEEK whence) {
    if (file >= 0x800) {
        return NuDatFileSeek(file, offset, whence);
    }

    file -= 0x400;

    switch (whence) {
        default:
            memfiles[file].ptr = memfiles[file].buffer + offset;
            break;
        case NUFILE_SEEK_CURRENT:
            memfiles[file].ptr = memfiles[file].ptr + offset;
            break;
        case NUFILE_SEEK_END:
            memfiles[file].ptr = memfiles[file].end - offset;
            break;
    }

    return (i64)(memfiles[file].ptr - memfiles[file].buffer);
}

i64 NuFilePos(NUFILE file) {
    FILEINFO *info;
    i64 pos;

#ifdef ANDROID
    if (file >= 0x2000) {
        return NuFileAndroidAPK::GetFilePos(file);
    }
#endif

    if (file >= 0x1000) {
        pos = NuMcSeek(file - 0x1000, 0, NUFILE_SEEK_END, 0);
        return pos;
    }

    if (file >= 0x400) {
        return NuMemFilePos(file);
    }

    file -= 1;
    info = &file_info[file];

    if (info->use_buf) {
        return info->read_pos;
    }

    do {
        pos = NuPSFileLSeek(file, 0, NUFILE_SEEK_CURRENT);
    } while (pos < 0);

    return pos;
}

extern "C" {
    i32 NuFileEOF(NUFILE file) {
        i64 position = NuFilePos(file);
        i64 size = NuFileOpenSize(file);
        return position >= size;
    }

}

i64 NuFileSeek(NUFILE file, i64 offset, NUFILESEEK whence) {
    FILEINFO *info;
    i64 pos;

#ifdef ANDROID
    NuFile::SeekOrigin::T native_mode;

    if (file >= 0x2000) {
        switch (whence) {
            default:
                native_mode = NuFile::SeekOrigin::START;
                break;
            case NUFILE_SEEK_CURRENT:
                native_mode = NuFile::SeekOrigin::CURRENT;
                break;
            case NUFILE_SEEK_END:
                native_mode = NuFile::SeekOrigin::END;
                break;
        }

        return NuFileAndroidAPK::SeekFile(file, offset, native_mode);
    }
#endif

    if (file >= 0x1000) {
        pos = (i64)NuMcSeek(file - 0x1000, offset, whence, 0);
        return pos;
    }

    if (file >= 0x400) {
        pos = NuMemFileSeek(file, (i32)offset, whence);
        return pos;
    }

    file -= 1;
    info = &file_info[file];

    if (info->use_buf) {
        switch (whence) {
            default:
                info->read_pos = offset;
                break;
            case NUFILE_SEEK_CURRENT:
                info->read_pos += offset;
                break;
            case NUFILE_SEEK_END:
                info->read_pos = info->file_len - offset;
                break;
        }
    } else {
        pos = NuPSFileLSeek(file, offset, whence);
        return pos;
    }

    return info->read_pos;
}

i32 NuFileExistQuiet(char *filepath) {
    NUFILE file = 0;
    if (filepath == NULL || *filepath == '\0') {
        return 0;
    }
    i32 buffering = nufile_buffering_enabled;
    nufile_buffering_enabled = 0;
    if (curr_dat != NULL) {
        i32 index = NuDatFileFindTree(curr_dat, filepath);
        if (index >= 0) {
            nufile_buffering_enabled = buffering;
            return curr_dat->file_info[index].decompressed_len;
        }
    }
    file = NuFileOpenDF(filepath, NUFILE_READ, curr_dat, 1);
    if (file != 0) {
        NuFileClose(file);
    }
    nufile_buffering_enabled = buffering;
    return file != 0;
}

i64 NuFileSize(char *filepath) {
    i32 file;
    i64 pos;
    i32 buffering;

    buffering = nufile_buffering_enabled;
    nufile_buffering_enabled = 0;

    if (curr_dat != NULL) {
        file = NuDatFileFindTree(curr_dat, filepath);
        if (file >= 0) {
            return (u64)curr_dat->file_info[file].decompressed_len;
        }
    }

    pos = -1;

    if (filepath != NULL && *filepath != '\0') {
        i32 file;

        file = NuFileOpen(filepath, NUFILE_READ);
        if (file != 0) {
            do {
                pos = NuFileSeek(file, 0, NUFILE_SEEK_END);
            } while (pos < 0);

            pos = NuFilePos(file);
            NuFileClose(file);
        }
    }

    nufile_buffering_enabled = buffering;

    return pos;
}

extern "C" {
    void *NuFileLoad(char *path) {
        i32 buffering = nufile_buffering_enabled;
        nufile_buffering_enabled = 0;
        void *data = NULL;
        NUFILE file = NuFileOpen(path, NUFILE_READ);
        if (file != 0) {
            i32 size = NuFileOpenSize(file);
            if (size > 0) {
                data = NU_ALLOC(size, 4, 1, "", 0);
                if (data != NULL) {
                    NuFileRead(file, data, size);
                }
            }
            NuFileClose(file);
        }
        nufile_buffering_enabled = buffering;
        return data;
    }

}

i32 NuFileLoadBuffer(char *filepath, void *buf, i32 buf_size) {
    NUFILE file;
    i32 iVar2;
    i32 lVar1;
    i32 loaded;

    nufile_lasterror = 0;
    loaded = 0;

    if (curr_dat != NULL) {
        loaded = NuDatFileLoadBuffer(curr_dat, filepath, buf, buf_size);

        if (nufile_lasterror == -1) {
            return 0;
        }
    }

    if (loaded == 0) {
        if (NuFileExists(filepath)) {
            file = NuFileOpen(filepath, NUFILE_READ);
            if (file != 0) {
                if (nufile_try_packed) {
                    loaded = NuPPLoadBuffer(file, buf, buf_size);
                } else {
                    loaded = NuFileOpenSize(file);

                    if (loaded <= buf_size && loaded != 0) {
                        while (NuFileRead(file, buf, loaded) < 0) {
                            while (NuFileSeek(file, 0, NUFILE_SEEK_START) < 0) {
                            }
                        }
                    } else {
                        nufile_lasterror = -1;
                        loaded = 0;
                    }
                }

                NuFileClose(file);
            } else {
                nufile_lasterror = -3;
            }
        } else {
            nufile_lasterror = -2;
        }
    }

    return loaded;
}

i32 NuFileLoadBufferVP(char *filepath, VARIPTR *buf, VARIPTR *buf_end) {
    i32 len;

    len = NuFileLoadBuffer(filepath, buf->void_ptr, buf_end->addr - buf->addr);

    buf->addr += len;

    return len;
}

extern "C" {
    i32 NuFileCopyEx(char *dest, char *source, void *buffer, i32 capacity) {
        i32 remaining;
        i32 copied = 0;
        NUFILE input = NuFileOpen(source, NUFILE_READ);
        NUFILE output = NuFileOpen(dest, NUFILE_WRITE);
        if (input != 0 && output != 0) {
            remaining = NuFileOpenSize(input);
            while (remaining != 0) {
                i32 count = NuFileRead(input, buffer, remaining < capacity ? remaining : capacity);
                NuFileWrite(output, buffer, count);
                copied += count;
                remaining -= count;
            }
        }
        if (input != 0) {
            NuFileClose(input);
        }
        if (output != 0) {
            NuFileClose(output);
        }
        return copied;
    }

    i32 NuFileCopy(char *dest, char *source) {
        char buffer[1024];
        return NuFileCopyEx(dest, source, buffer, sizeof(buffer));
    }

    i32 NuFileFormat(char *path) {
        i32 result = 0;
        NUFILE_DEVICE *device = NuFileGetDeviceFromPath(path);
        if (device != NULL && device->id == 1) {
            i32 port = path[2] - '0';
            i32 slot = path[3] - '0';
            result = NuMcFormat(port, slot);
        }
        return result;
    }

}

i32 NuFileRead(NUFILE file, void *buf, i32 size) {
    FILEINFO *info;
    i32 bytes_read;
    char *char_buf;
    i32 available;

#ifdef ANDROID
    if (file >= 0x2000) {
        return NuFileAndroidAPK::ReadFile(file, buf, size);
    }
#endif

    if (file >= 0x1000) {
        return NuMcRead(file - 0x1000, buf, size, 0);
    }

    if (file >= 0x400) {
        return NuMemFileRead(file, buf, size);
    }

    file -= 1;
    info = &file_info[file];

    if (info->use_buf) {
        if (info->buf == NULL) {
            AquireFileBuffer(info);
        }

        bytes_read = 0;
        char_buf = (char *)buf;

        while (size > 0) {
            if (info->read_pos >= info->file_len) {
                return bytes_read;
            }

            if (info->read_pos < info->buf_start || info->read_pos >= info->buf_start + info->buf_len) {
                if (info->read_pos != info->file_pos) {
                    NuPSFileLSeek(info->handle, info->read_pos, NUFILE_SEEK_START);
                    info->file_pos = info->read_pos;
                }

                info->buf_len = NuPSFileRead(info->handle, info->buf->data, 0x400);
                info->buf_start = info->file_pos;
                info->file_pos += info->buf_len;
            }

            available = MIN(info->buf_len + (i32)(info->buf_start - info->read_pos), size);
            if (available != 0) {
                u32 to_read;
                void *file_buf;

                to_read = available;
                file_buf = info->buf;
                memcpy(char_buf, (void *)((usize)file_buf + (info->read_pos - info->buf_start) + 8), to_read);
            }

            char_buf += available;
            bytes_read += available;
            size -= available;
            info->read_pos += available;
        }

        return bytes_read;
    } else {
        return NuPSFileRead(file, buf, size);
    }
}

f32 NuFileReadFloat(NUFILE file) {
    f32 value;
    NuFileRead(file, &value, sizeof(f32));
    return value;
}

i32 NuFileReadInt(NUFILE file) {
    i32 value;
    NuFileRead(file, &value, sizeof(i32));
    return value;
}

u32 NuFileReadUnsignedInt(NUFILE file) {
    u32 value;
    NuFileRead(file, &value, sizeof(u32));
    return value;
}

u16 NuFileReadUnsignedShort(NUFILE file) {
    u16 value;
    NuFileRead(file, &value, sizeof(u16));
    return value;
}

i16 NuFileReadShort(NUFILE file) {
    i16 value;
    NuFileRead(file, &value, sizeof(i16));
    return value;
}

u8 NuFileReadUnsignedChar(NUFILE file) {
    u8 value = 0;
    NuFileRead(file, &value, sizeof(u8));
    return value;
}

i8 NuFileReadChar(NUFILE file) {
    i8 value = 0;
    NuFileRead(file, &value, sizeof(i8));
    return value;
}

u16 NuFileReadWChar(NUFILE file) {
    i16 value = 0;
    NuFileRead(file, &value, sizeof(u16));
    return value;
}

i32 NuFileWrite(NUFILE file, void *data, i32 size) {
    if (file >= 0x1000) {
        return NuMcWrite(file - 0x1000, data, size, 0);
    }

    if (file >= 0x400) {
        return NuMemFileWrite(file, data, size);
    }

    file -= 1;

    return NuPSFileWrite(file, data, size);
}

i32 NuFileWriteFloat(NUFILE file, float value) {
    float tmp = value;
    if (NuFile_SwapEndianOnWrite != 0) {
        NuFileEndianSwap32(&tmp);
    }
    return NuFileWrite(file, &tmp, sizeof(tmp));
}

i32 NuFileWriteInt(NUFILE file, i32 value) {
    i32 tmp;

    tmp = value;
    if (NuFile_SwapEndianOnWrite != 0) {
        NuFileEndianSwap32(&tmp);
    }

    return NuFileWrite(file, &tmp, sizeof(i32));
}

u32 NuFileWriteUnsignedInt(NUFILE file, u32 value) {
    u32 tmp;

    tmp = value;
    if (NuFile_SwapEndianOnWrite != 0) {
        NuFileEndianSwap32(&tmp);
    }

    return NuFileWrite(file, &tmp, sizeof(u32));
}

i32 NuFileWriteShort(NUFILE file, i16 value) {
    i16 tmp = value;
    if (NuFile_SwapEndianOnWrite != 0) {
        NuFileEndianSwap16(&tmp);
    }
    return NuFileWrite(file, &tmp, sizeof(tmp));
}

i32 NuFileWriteUnsignedShort(NUFILE file, u16 value) {
    u16 tmp = value;
    if (NuFile_SwapEndianOnWrite != 0) {
        NuFileEndianSwap16(&tmp);
    }
    return NuFileWrite(file, &tmp, sizeof(tmp));
}

i32 NuFileWriteChar(NUFILE file, i8 value) {
    return NuFileWrite(file, &value, sizeof(value));
}

i32 NuFileWriteUnsignedChar(NUFILE file, u8 value) {
    return NuFileWrite(file, &value, sizeof(value));
}

extern "C" {
    void NuFileWriteString(NUFILE file, const char *text) {
        i32 length = strlen(text);
        NuFileWrite(file, const_cast<char *>(text), length);
    }

    i32 NuFileWriteStringV(NUFILE file, const char *format, ...) {
        i32 length;
        va_list args;
        char text[1024];
        va_start(args, format);
        vsprintf(text, format, args);
        va_end(args);
        length = strlen(text);
        return NuFileWrite(file, text, length);
    }

    i32 NuFileBeginBlkRead(NUFILE file, i32 expected_tag) {
        i32 index = blkcnt++;
        i64 position = NuFilePos(file);
        if (index > 1 && nufile_blkinfo[index - 1].position + nufile_blkinfo[index - 1].size <= position) {
            --blkcnt;
            return 0;
        }
        nufile_blkinfo[index].position = NuFilePos(file);
        NuFileRead(file, &nufile_blkinfo[index], 8);
        if (nufile_blkinfo[index].size < 0) {
            nufile_blkinfo[index].size = -nufile_blkinfo[index].size;
        }
        return nufile_blkinfo[index].tag;
    }

    void NuFileEndBlkRead(NUFILE file) {
        i32 index = --blkcnt;
        i64 position = NuFilePos(file);
        if (nufile_blkinfo[index].position + nufile_blkinfo[index].size != position) {
            NuFileSeek(file, nufile_blkinfo[index].position + nufile_blkinfo[index].size, NUFILE_SEEK_START);
        }
    }

    i32 NuFileGetBlkSize(void) {
        return nufile_blkinfo[blkcnt - 1].size;
    }

    void NuFileBeginBlkWrite(NUFILE file, i32 tag, i32 negative_size) {
        i32 index = wblkcnt++;
        nufile_wblkinfo[index].position = NuFilePos(file);
        nufile_wblkinfo[index].tag = tag;
        if (negative_size == 1) {
            nufile_wblkinfo[index].size_sign = -1;
        } else {
            nufile_wblkinfo[index].size_sign = 1;
        }
        NuFileWriteInt(file, tag);
        NuFileWriteInt(file, tag);
    }

    void NuFileEndBlkWrite(NUFILE file) {
        i64 position;
        i32 index;
        i32 padding;
        i32 size;
        i32 zero = 0;
        index = --wblkcnt;
        position = NuFilePos(file);
        size = position - nufile_wblkinfo[index].position;
        padding = iff_padsize - size % iff_padsize;
        if (padding != iff_padsize) {
            position += padding;
            size = size + padding;
            // The original supplies this four-byte local even when padding exceeds four.
            NuFileWrite(file, &zero, padding);
        }
        if (nufile_wblkinfo[index].size_sign < 0) {
            size = -size;
        }
        NuFileSeek(file, nufile_wblkinfo[index].position + 4, NUFILE_SEEK_START);
        NuFileWrite(file, &size, 4);
        NuFileSeek(file, position, NUFILE_SEEK_START);
    }

    i32 NuFileAppendPath(char *dest, char *path, char *name) {
        NuStrCpy(dest, path);
        i32 length = strlen(dest);
        char *end = dest + length;
        // The original advances past the terminator before writing the separator.
        if (*end++ != '\\') {
            *end++ = '\\';
        }
        NuStrCpy(end, name);
        return NuStrLen(dest);
    }

    i32 NuFileExtractPath(char *dest, char *path) {
        char *end = strrchr(path, '\\');
        if (end != NULL) {
            ++end;
        } else {
            end = path;
        }
        i32 length = end - path;
        strncpy(dest, path, length);
        dest[length] = '\0';
        return length;
    }

    i32 NuFileExtractFilename(char *dest, char *path) {
        char *start = strrchr(path, '\\');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        NuStrCpy(dest, start);
        return NuStrLen(dest);
    }

    i32 NuFileExtractFile(char *dest, char *path) {
        char *start = strrchr(path, '\\');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        char *end = strrchr(path, '.');
        if (end == NULL) {
            end = path + strlen(path);
        }
        i32 length = end - start;
        strncpy(dest, start, length);
        dest[length] = '\0';
        return length;
    }

    i32 NuFileExtractExt(char *dest, char *path) {
        char *start = strrchr(path, '.');
        if (start != NULL) {
            ++start;
        } else {
            start = path;
        }
        NuStrCpy(dest, start);
        return NuStrLen(dest);
    }

    void NuFileInitAddress(i32 capacity) {
        if (capacity != MAX_FILE_ADDRESS) {
            if (addr_ref != NULL)
                NU_FREE(addr_ref);
            if (addr_pos != NULL)
                NU_FREE(addr_pos);
            MAX_FILE_ADDRESS = capacity;
            addr_ref = (NuFileAddress *)NU_ALLOC(MAX_FILE_ADDRESS * sizeof(NuFileAddress), 4, 1, "", 0);
            addr_pos = (NuFileAddress *)NU_ALLOC(MAX_FILE_ADDRESS * sizeof(NuFileAddress), 4, 1, "", 0);
        }
        ref_count = 0;
        pos_count = 0;
    }

    void NuFileTidyAddress(void) {
        if (addr_ref != NULL) {
            NU_FREE(addr_ref);
            addr_ref = NULL;
        }
        if (addr_pos != NULL) {
            NU_FREE(addr_pos);
            addr_pos = NULL;
        }
        MAX_FILE_ADDRESS = 0;
    }

    void NuFileWriteAddress(NUFILE file, void *address) {
        static i32 pad;
        if (address != NULL) {
            addr_ref[ref_count].address = address;
            addr_ref[ref_count].position = NuFilePos(file);
            ++ref_count;
        }
        NuFileWrite(file, &pad, 4);
    }

    void NuFileSetAddress(NUFILE file, void *address) {
        i32 index;
        if (address != NULL) {
            for (index = 0; index < pos_count; ++index) {
                if (addr_pos[index].address == address)
                    break;
            }
            if (index == pos_count)
                ++pos_count;
            addr_pos[index].address = address;
            addr_pos[index].position = NuFilePos(file);
        }
    }

    void NuFilePatchAddress(NUFILE file) {
        i32 ref;
        i32 pos;
        i64 offset;
        for (ref = 0; ref < ref_count; ++ref) {
            for (pos = 0; pos < pos_count; ++pos) {
                if (addr_ref[ref].address == addr_pos[pos].address) {
                    offset = addr_pos[pos].position - addr_ref[ref].position;
                    NuFileSeek(file, addr_ref[ref].position, NUFILE_SEEK_START);
                    NuFileWrite(file, &offset, 4);
                }
            }
        }
        NuFileSeek(file, 0, NUFILE_SEEK_END);
    }

    i32 NuFileAlign(NUFILE file, i32 mask) {
        u8 padding = 0xfe;
        i32 count = 0;
        i64 position = NuFilePos(file);
        while ((position & mask) != 0) {
            ++position;
            ++count;
            NuFileWrite(file, &padding, 1);
        }
        return count;
    }

    i32 NuFileAlignRead(NUFILE file, i32 mask) {
        i32 count = 0;
        i64 position = NuFilePos(file);
        while ((position & mask) != 0) {
            ++position;
            ++count;
            NuFileReadChar(file);
        }
        return count;
    }

    i32 NuFileIsNewer(nufile_info_s *first, nufile_info_s *second) {
        if (second->year > first->year)
            return 1;
        if (second->year < first->year)
            return 0;
        if (second->month > first->month)
            return 1;
        if (second->month < first->month)
            return 0;
        if (second->day > first->day)
            return 1;
        if (second->day < first->day)
            return 0;
        if (second->hour > first->hour)
            return 1;
        if (second->hour < first->hour)
            return 0;
        if (second->minute > first->minute)
            return 1;
        if (second->minute < first->minute)
            return 0;
        if (second->second > first->second)
            return 1;
        if (second->second < first->second)
            return 0;
        return 0;
    }

}

i64 NuDatCalcPos(nudathdr_s *header, i32 index) {
    i64 pos = index;

    if (header->version < -1) {
        pos <<= 8;
    }

    pos += datsys_offset;

    return pos;
}

extern "C" {
    i32 NuDatGetFileInfo(NUDATHDR *header, char *name, i64 *position, i32 *length) {
        if (header == NULL) {
            return -1;
        }

        const i32 index = NuDatFileFindTree(header, name);
        if (index >= 0) {
            const i64 file_position = NuDatCalcPos(header, header->file_info[index].file_offset);
            if (nufile_lsn_allowed != 0 && header->unknown2 != 0) {
                if (position != NULL) {
                    *position = header->unknown2 + static_cast<i32>(file_position / 0x800);
                }
                if (length != NULL) {
                    *length = header->file_info[index].file_len;
                }
                return index;
            }
            if (position != NULL) {
                *position = file_position;
            }
            if (length != NULL) {
                *length = header->file_info[index].file_len;
            }
            return index;
        }
        return -1;
    }

}

i32 NuDatFileLoadBuffer(NUDATHDR *dat, char *name, void *dest, i32 buf_size) {
    NUFILE file;
    char *buf;

    nufile_lasterror = 0;

    file = NuDatFileOpen(dat, name, NUFILE_READ);

    if (file != 0) {
        i32 size;

        if (dat->mode == 3) {
            while (NuFileStatus(file) != 0) {
            }
        }

        if (dat_file_infos[file - 0x800].compression_mode != 0) {
            size = dat_file_infos[file - 0x800].decompressed_len;
        } else {
            size = dat_file_infos[file - 0x800].file_len;
        }

        buf = (char *)dest;

        if (size <= buf_size && size != 0) {
            LOG_DEBUG("Loading %d bytes from dat file", size);

            while (NuDatFileRead(file, buf, size) < 0) {
                while (NuDatFileSeek(file, 0, NUFILE_SEEK_START) < 0) {
                }
            }

            if (dat->mode == 3) {
                while (NuFileStatus(file) != 0) {
                }
            }

            NuFileClose(file);

            return size;
        }

        if (size != 0) {
            nufile_lasterror = -1;
        }

        NuFileClose(file);
    }

    return 0;
}

i64 NuDatFilePos(NUFILE file) {
    NUDATFILEINFO *info;

    file -= 0x800;
    info = dat_file_infos + file;

    return info->pos - info->start;
}

i32 NuDatFileRead(NUFILE file, void *buf, i32 size) {
    NUDATFILEINFO *info;
    NUDATOPENFILEINFO *open_file;
    i32 inflated_size;
    i32 total_read;
    i32 bytes_read;

    file -= 0x800;
    info = &dat_file_infos[file];
    open_file = &info->hdr->open_files[info->open_file_idx];

    if (info->compression_mode != 0) {
        total_read = 0;

        while (size != 0) {
            if (decode_buffer_left == 0) {
                NuDatFileDecodeNext();

                if (read_buffer_size == read_buffer_decoded_size) {
                    memcpy(decode_buffer, read_buffer, read_buffer_size);
                } else if (info->compression_mode == 2) {
                    ExplodeBufferNoHeader(read_buffer, decode_buffer, read_buffer_size, read_buffer_decoded_size);
                } else if (info->compression_mode == 3) {
                    inflated_size =
                        InflateBuffer(decode_buffer, read_buffer_decoded_size, read_buffer, read_buffer_size);
                }

                decode_buffer_pos = 0;
                decode_buffer_left = read_buffer_decoded_size;
            }

            bytes_read = MIN(size, decode_buffer_left);
            memcpy(buf, decode_buffer + decode_buffer_pos, bytes_read);
            buf = (void *)((char *)buf + bytes_read);

            decode_buffer_pos += bytes_read;
            decode_buffer_left -= bytes_read;
            size -= bytes_read;
            total_read += bytes_read;
        }

        return total_read;
    }

    if (info->pos != open_file->pos) {
        NuFileSeek(open_file->dat_file, info->pos, NUFILE_SEEK_START);
        open_file->pos = info->pos;
    }

    bytes_read = MIN(size, MAX(info->file_len + info->start - info->pos, 0));
    if (bytes_read != 0) {
        total_read = NuFileRead(open_file->dat_file, buf, bytes_read);
        if (total_read > -1) {
            info->pos += total_read;
            open_file->pos = info->pos;
        }

        return total_read;
    }

    return 0;
}

i64 NuDatFileSeek(NUFILE file, i64 offset, NUFILESEEK whence) {
    NUDATFILEINFO *info;
    NUDATHDR *hdr;
    NUDATOPENFILEINFO *open_info;
    i64 offset_in_dat;

    file -= 0x800;
    info = &dat_file_infos[file];
    open_info = &info->hdr->open_files[info->open_file_idx];

    switch (whence) {
        default:
            offset_in_dat = offset + info->start;
            break;
        case NUFILE_SEEK_CURRENT:
            offset_in_dat = offset + info->pos;
            break;
        case NUFILE_SEEK_END:
            offset_in_dat = info->start + info->file_len - offset;
            break;
    }

    info->pos = NuFileSeek(open_info->dat_file, offset_in_dat, NUFILE_SEEK_START);
    open_info->pos = info->pos;

    return info->pos;
}

static u32 NameToHash(char *name) {
    u32 constant = 0x811c9dc5;
    u32 prime = 0x199933;

    u32 hash = constant;
    for (; *name != '\0'; name++) {
        hash ^= *name;
        hash *= prime;
    }

    return hash;
}

isize BinarySearch(u32 element, u32 *array, usize length) {
    isize start = 0;
    isize end = length - 1;

    while (start <= end) {
        isize index = (start + end) / 2;
        if (array[index] == element) {
            return index;
        } else if (element > array[index]) {
            start = index + 1;
        } else {
            end = index - 1;
        }
    }

    return -1;
}

i32 NuDatFileFindHash(nudathdr_s *header, char *name) {
    LOG_DEBUG("header=%p name=%s", header, name);

    u32 hash = NameToHash(name);

    i32 r = BinarySearch(hash, header->hash_idxs, header->file_count);

    if (r != -1) {
        return r;
    }

    if (header->hash_count != 0) {
        VARIPTR ptr;

        ptr.char_ptr = header->hashes;

        for (i32 i = 0; i < header->hash_count; i++) {
            if (NuStrCmp(ptr.char_ptr, name) == 0) {
                ptr.addr += NuStrLen(ptr.char_ptr) + 1;

                if ((ptr.addr & 1) != 0) {
                    ptr.addr++;
                }

                return (i32)*ptr.i16_ptr;
            }

            ptr.addr += NuStrLen(ptr.char_ptr) + 3;

            if ((ptr.addr & 1) != 0) {
                ptr.addr++;
            }
        }
    }

    return -1;
}

i32 NuDatFileFindTree(nudathdr_s *header, char *name) {
    char buf[256];
    char *backslash;
    i32 node_idx;

    LOG_DEBUG("header=%p name=%s", header, name);

    if (*name == '@') {
        name = name + 4;
    }

    NuStrCpy(buf, name);
    NuFileUpCase(0, buf);

    name = buf;

    if (header->file_tree == NULL) {
        return NuDatFileFindHash(header, name);
    }

    node_idx = header->file_tree->child_idx;

    backslash = strchr(name, L'\\');
    if (backslash != NULL) {
        *backslash = '\0';
    }

    do {
        if (NuStrICmp(header->file_tree[node_idx].name, name) == 0) {
            if (backslash == NULL) {
                if (header->file_tree[node_idx].child_idx < 1) {
                    return -header->file_tree[node_idx].child_idx;
                }

                return -1;
            }

            node_idx = header->file_tree[node_idx].child_idx;

            name = backslash + 1;

            backslash = strchr(name, L'\\');
            if (backslash != NULL) {
                *backslash = '\0';
            }
        } else {
            node_idx = header->file_tree[node_idx].sibling_idx;
        }
    } while (node_idx != 0);

    return -1;
}

void NuDatFileClose(NUFILE file) {
    NUDATFILEINFO *info;
    NUDATHDR *hdr;

    file -= 0x800;
    info = dat_file_infos + file;
    hdr = info->hdr;

    if (info->open_file_idx >= 0) {
        hdr->open_files[info->open_file_idx].info_idx = -1;
    }

    info->is_used = 0;
}

i32 NuDatFileGetFreeInfo(void) {
    i32 found;
    i32 i;

    found = -1;

    NuThreadCriticalSectionBegin(file_criticalsection);

    for (i = 0; i < 20; i++) {
        if (!dat_file_infos[i].is_used) {
            dat_file_infos[i].is_used = 1;
            found = i;
            break;
        }
    }

    NuThreadCriticalSectionEnd(file_criticalsection);

    return found;
}

i32 NuDatFileGetFreeHandleIX(NUDATHDR *hdr, i32 info_idx) {
    i32 file_idx;
    i32 i;

    file_idx = -1;

    NuThreadCriticalSectionBegin(file_criticalsection);

    for (i = 0; i < 20; i++) {
        if (hdr->open_files[i].info_idx == -1) {
            hdr->open_files[i].info_idx = info_idx;
            file_idx = i;
            break;
        }
    }

    NuThreadCriticalSectionEnd(file_criticalsection);

    return file_idx;
}

static i32 OpenDatFileBase(NUDATHDR *hdr, i32 file_idx) {
    NUDATOPENFILEINFO *open_file;

    open_file = hdr->open_files + file_idx;
    if (open_file->dat_file == 0) {
        open_file->dat_file = NuFileOpenDF(hdr->name, (NUFILEMODE)hdr->mode, NULL, 0);
        open_file->pos = 0;
    }

    return open_file->dat_file;
}

NUFILE NuDatFileOpen(NUDATHDR *hdr, char *filepath, NUFILEMODE mode) {
    i32 node_idx;
    i32 info_idx;
    i32 file_idx;
    NUDATOPENFILEINFO *open_info;
    NUDATFILEINFO *info;
    i64 start;
    i64 seek_pos;

    if (mode == NUFILE_READ) {
        node_idx = NuDatFileFindTree(hdr, filepath);
        if (node_idx > -1 && hdr->file_info[node_idx].file_len != 0) {
            info_idx = NuDatFileGetFreeInfo();
            if (info_idx != -1) {
                file_idx = NuDatFileGetFreeHandleIX(hdr, info_idx);
                if (file_idx != -1) {
                    open_info = &hdr->open_files[file_idx];

                    NuThreadCriticalSectionBegin(file_criticalsection);

                    OpenDatFileBase(hdr, file_idx);

                    dat_file_infos[info_idx].hdr = hdr;
                    dat_file_infos[info_idx].start = NuDatCalcPos(hdr, hdr->file_info[node_idx].file_offset);
                    dat_file_infos[info_idx].pos = dat_file_infos[info_idx].start;

                    dat_file_infos[info_idx].file_len = hdr->file_info[node_idx].file_len;
                    dat_file_infos[info_idx].decompressed_len = hdr->file_info[node_idx].decompressed_len;
                    dat_file_infos[info_idx].compression_mode = hdr->file_info[node_idx].compression_mode;

                    dat_file_infos[info_idx].open_file_idx = file_idx;

                    open_info->pos = dat_file_infos[info_idx].pos;

                    do {
                        seek_pos = NuFileSeek(open_info->dat_file, dat_file_infos[info_idx].start, NUFILE_SEEK_START);
                    } while (seek_pos < 0);

                    if (dat_file_infos[info_idx].compression_mode == 2 ||
                        dat_file_infos[info_idx].compression_mode == 3) {
                        NuDatFileDecodeNewFile(&dat_file_infos[info_idx], open_info);
                    }

                    NuThreadCriticalSectionEnd(file_criticalsection);

                    return info_idx + 0x800;
                }

                dat_file_infos[info_idx].is_used = 0;
            }
        }
    }

    return 0;
}

i32 NuDatFileOpenSize(NUFILE file) {
    NUDATFILEINFO *info;

    file -= 0x800;
    info = &dat_file_infos[file];

    if (info->compression_mode != 0) {
        return info->decompressed_len;

    } else {
        return info->file_len;
    }
}

extern "C" {
    void NuDatClose(NUDATHDR *header) {
        for (i32 index = 0; index < 20; ++index) {
            if (header->open_files[index].dat_file != 0) {
                NuFileClose(header->open_files[index].dat_file);
            }
            if (header->open_files[index].info_idx >= 0) {
                dat_file_infos[header->open_files[index].info_idx].is_used = 0;
            }
        }
        if (header->unknown != 0) {
            NU_FREE(header);
        }
    }

}

static void APIEndianSwap(void *data, i32 count, i32 size) {
}

NUDATHDR *NuDatOpenEx(char *filepath, VARIPTR *buf, i32 *_unused, i16 mode) {
    char _unused2[256];
    i32 path_len;
    i32 i;
    NUFILE file;
    i64 seek_offset;
    i32 file_len;
    i32 bytes_read;
    i32 _unused4;
    i32 read_buf[2];
    i32 total_read;

    NUDATHDR *hdr;
    NUDFNODE_V1 *old_ver;
    VARIPTR hash;
    u32 hash_idx;
    i32 idx_to_swap;
    u32 tmp_idx;
    NUDATFINFO tmp;
    i32 dfnodev2_size;
    i32 dfnodev1_size;
    i32 buffering;
    i32 _unused3;

    _unused3 = 0;
    total_read = 0;

    buffering = nufile_buffering_enabled;

    file = NuFileOpenDF(filepath, (NUFILEMODE)mode, NULL, 0);
    if (file != 0) {
        i32 n;
        bytes_read = 0;

        file_len = NuFileOpenSize(file);
        bytes_read = NuFileRead(file, read_buf, 8);
        APIEndianSwap(read_buf, 2, 4);

        seek_offset = (i64)read_buf[0];
        if (seek_offset < 0) {
            seek_offset *= -0x100;
        }

        NuFileSeek(file, seek_offset, NUFILE_SEEK_START);

        path_len = ALIGN(NuStrLen(filepath) + 1, 0x10);

        hdr = (NUDATHDR *)buf->void_ptr;
        buf->addr += sizeof(NUDATHDR);
        memset(hdr, 0, sizeof(NUDATHDR));

        hdr->unknown = 1;

        hdr->name = (char *)buf->void_ptr;
        buf->addr += path_len;
        NuStrCpy(hdr->name, filepath);

        bytes_read = NuFileRead(file, &hdr->version, 4);
        total_read += bytes_read;
        APIEndianSwap(&hdr->version, 1, 4);

        bytes_read = NuFileRead(file, &hdr->file_count, 4);
        total_read += bytes_read;
        APIEndianSwap(&hdr->file_count, 1, 4);

        hdr->file_info = (NUDATFINFO *)ALIGN(buf->addr, 0x20);
        buf->addr = ALIGN(buf->addr, 0x20);
        buf->addr += hdr->file_count * sizeof(NUDATFINFO);
        bytes_read = NuFileRead(file, hdr->file_info, hdr->file_count * sizeof(NUDATFINFO));
        total_read += bytes_read;
        for (i = 0; i < hdr->file_count; i++) {
            APIEndianSwap(&hdr->file_info[i].file_offset, 1, 4);
            APIEndianSwap(&hdr->file_info[i].file_len, 1, 4);
            APIEndianSwap(&hdr->file_info[i].decompressed_len, 1, 4);
        }

        bytes_read = NuFileRead(file, &hdr->tree_node_count, 4);
        total_read += bytes_read;
        APIEndianSwap(&hdr->tree_node_count, 1, 4);

        hdr->file_tree = (NUDFNODE *)buf->void_ptr;
        buf->addr += hdr->tree_node_count * sizeof(NUDFNODE);

        if (hdr->version < -4) {
            // Using `sizeof()` causes this not to match.
            dfnodev2_size = hdr->tree_node_count * 0xc;
            bytes_read = NuFileRead(file, hdr->file_tree, dfnodev2_size);
            total_read += bytes_read;
        } else {
            dfnodev1_size = hdr->tree_node_count * sizeof(NUDFNODE_V1);
            bytes_read = NuFileRead(file, hdr->file_tree, dfnodev1_size);
            total_read += bytes_read;
            old_ver = (NUDFNODE_V1 *)hdr->file_tree;

            for (n = hdr->tree_node_count - 1; n > 0; n--) {
                hdr->file_tree[n].unknown = 0;
                hdr->file_tree[n].unknown2 = 0;
                hdr->file_tree[n].name = old_ver[n].name;
                hdr->file_tree[n].sibling_idx = old_ver[n].sibling_idx;
                hdr->file_tree[n].child_idx = old_ver[n].child_idx;
            }
        }

        for (n = 0; n < hdr->tree_node_count; n++) {
            APIEndianSwap(&hdr->file_tree[n].child_idx, 1, 2);
            APIEndianSwap(&hdr->file_tree[n].sibling_idx, 1, 2);
            APIEndianSwap(&hdr->file_tree[n].name, 1, 4);

            if (hdr->version < -4) {
                APIEndianSwap(&hdr->file_tree[n].unknown, 1, 2);
                APIEndianSwap(&hdr->file_tree[n].unknown2, 1, 2);
            }
        }

        bytes_read = NuFileRead(file, &hdr->leaf_names_len, 4);
        total_read += bytes_read;
        APIEndianSwap(&hdr->leaf_names_len, 1, 4);

        hdr->leaf_names = buf->char_ptr;
        buf->addr += hdr->leaf_names_len;
        bytes_read = NuFileRead(file, hdr->leaf_names, hdr->leaf_names_len);
        total_read += bytes_read;

        for (n = 0; n < hdr->tree_node_count; n++) {
            _unused4 = 0;
            hdr->file_tree[n].name = reinterpret_cast<char *>(reinterpret_cast<usize>(hdr->file_tree[n].name) +
                                                              (reinterpret_cast<usize>(hdr->leaf_names) - _unused4));
        }

        hdr->file_tree[0].name = NULL;

        buf->addr -= hdr->leaf_names_len;
        buf->addr = buf->addr - hdr->tree_node_count * sizeof(NUDFNODE);
        hdr->file_tree = NULL;
        hdr->leaf_names = NULL;

        hdr->hash_idxs = NULL;
        hdr->hash_count = 0;
        hdr->hashes_len = 0;
        hdr->hashes = NULL;

        if (hdr->version < -1) {
            hdr->hash_idxs = (u32 *)ALIGN(buf->addr, 0x20);
            buf->addr = ALIGN(buf->addr, 0x20);
            buf->addr += hdr->file_count * sizeof(i32);
            bytes_read = NuFileRead(file, hdr->hash_idxs, hdr->file_count * sizeof(i32));
            total_read += bytes_read;

            for (n = 0; n < hdr->file_count; n++) {
                APIEndianSwap(&hdr->hash_idxs[n], 1, 4);
            }

            bytes_read = NuFileRead(file, &hdr->hash_count, 4);
            total_read += bytes_read;
            APIEndianSwap(&hdr->hash_count, 1, 4);

            bytes_read = NuFileRead(file, &hdr->hashes_len, 4);
            total_read += bytes_read;
            APIEndianSwap(&hdr->hashes_len, 1, 4);

            hdr->hashes = (char *)ALIGN(buf->addr, 0x20);
            buf->addr = ALIGN(buf->addr, 0x20);
            buf->addr += hdr->hashes_len;
            bytes_read = NuFileRead(file, hdr->hashes, hdr->hashes_len);
            total_read += bytes_read;

            hash.char_ptr = hdr->hashes;
            for (n = 0; n < hdr->hash_count; n++) {
                hash.addr += NuStrLen(hash.char_ptr) + 1;
                if ((hash.addr & 1) != 0) {
                    hash.addr += 1;
                }

                APIEndianSwap(hash.char_ptr, 1, 2);
                hash.addr += 2;
            }
        }

        for (n = 0; n < 20; n++) {
            hdr->open_files[n].info_idx = -1;
            hdr->open_files[n].dat_file = 0;
            hdr->open_files[n].pos = 0;
        }

        hdr->open_files[0].dat_file = file;
        hdr->open_files[0].pos = 0;

        hdr->mode = mode;

        if (hdr->version > -3) {
            for (i = 0; i < hdr->file_count - 1; i++) {
                hash_idx = hdr->hash_idxs[i];
                idx_to_swap = i;

                for (i32 j = i + 1; j < hdr->file_count; j++) {
                    if (hdr->hash_idxs[j] <= hash_idx) {
                        hash_idx = hdr->hash_idxs[j];
                        idx_to_swap = j;
                    }
                }

                if (i != idx_to_swap) {
                    tmp_idx = hdr->hash_idxs[i];
                    hdr->hash_idxs[i] = hdr->hash_idxs[idx_to_swap];
                    hdr->hash_idxs[idx_to_swap] = tmp_idx;

                    tmp = hdr->file_info[i];
                    hdr->file_info[i] = hdr->file_info[idx_to_swap];
                    hdr->file_info[idx_to_swap] = tmp;
                }
            }
        }

        return hdr;
    }

    nufile_buffering_enabled = buffering;

    return NULL;
}

NUDATHDR *NuDatOpen(char *filepath, VARIPTR *buf, i32 *_unused) {
    return NuDatOpenEx(filepath, buf, _unused, 0);
}

NUDATHDR *NuDatSet(NUDATHDR *header) {
    NUDATHDR *dat = curr_dat;
    curr_dat = header;
    return dat;
}

extern "C" {
extern "C" NUDATHDR *NuDatGet(void) {
    return curr_dat;
}

}

static i32 MatchExtension(char *extension, char *path_end, i32 path_len) {
    while (*extension != '\0') {
        --path_end;
        if (path_len == 0) {
            return 0;
        }
        char lower = NuToLower(*path_end);
        if (*extension != lower) {
            return 0;
        }
        ++extension;
        --path_len;
    }
    return 1;
}

static void AddExtension(char *extension, i32 type, i32 platform) {
    char *ext;
    i32 len;
    FILEEXTINFO *next;

    next = &extensions[num_extensions];

    len = NuStrLen(extension);
    next->len = len;

    ext = next->extension;
    while (len != 0) {
        len--;
        *ext = extension[len];
        ext++;
    }

    *ext = '\0';

    next->type = type;
    next->platform = platform;
    num_extensions++;
}

static void NuFileExtInit(void) {
    static i32 first_time = 1;

    if (!first_time) {
        return;
    }

    first_time = 0;

    memset(extensions, 0, sizeof(extensions));
    num_extensions = 0;

    AddExtension(".ca3", NUFILETYPE_ANIM, X360_PLATFORM);
    AddExtension(".cbs", NUFILETYPE_BSANIM, X360_PLATFORM);
    AddExtension("_360.ghg", NUFILETYPE_CHARACTER, X360_PLATFORM);
    AddExtension("_360.gsc", NUFILETYPE_SCENE, X360_PLATFORM);
    AddExtension("_360.tex", NUFILETYPE_TEXTURE, X360_PLATFORM);
    AddExtension(".cc2", NUFILETYPE_CUTSCENE, X360_PLATFORM);
    AddExtension(".wavx", NUFILETYPE_SFX, X360_PLATFORM);
    AddExtension(".wavm", NUFILETYPE_SOUNDSTREAM, X360_PLATFORM);
    AddExtension(".wmv", NUFILETYPE_VIDEO, X360_PLATFORM);

    AddExtension(".ca3", NUFILETYPE_ANIM, PS3_PLATFORM);
    AddExtension(".cbs", NUFILETYPE_BSANIM, PS3_PLATFORM);
    AddExtension("_ps3.ghg", NUFILETYPE_CHARACTER, PS3_PLATFORM);
    AddExtension("_ps3.gsc", NUFILETYPE_SCENE, PS3_PLATFORM);
    AddExtension("_ps3.tex", NUFILETYPE_TEXTURE, PS3_PLATFORM);
    AddExtension(".cc2", NUFILETYPE_CUTSCENE, PS3_PLATFORM);
    AddExtension(".msf", NUFILETYPE_SFX, PS3_PLATFORM);
    AddExtension(".mib", NUFILETYPE_SOUNDSTREAM, PS3_PLATFORM);
    AddExtension(".pam", NUFILETYPE_VIDEO, PS3_PLATFORM);

    AddExtension(".an3", NUFILETYPE_ANIM, PC_PLATFORM);
    AddExtension(".bsa", NUFILETYPE_BSANIM, PC_PLATFORM);

#ifndef ANDROID
    AddExtension("_pc.ghg", NUFILETYPE_CHARACTER, PC_PLATFORM);
    AddExtension("_pc.gsc", NUFILETYPE_SCENE, PC_PLATFORM);
#endif

    AddExtension("_pc.dds", NUFILETYPE_TEXTURE, PC_PLATFORM);
    AddExtension(".cu2", NUFILETYPE_CUTSCENE, PC_PLATFORM);
    AddExtension(".wav", NUFILETYPE_SFX, PC_PLATFORM);
    AddExtension(".mib", NUFILETYPE_SOUNDSTREAM, PC_PLATFORM);
    AddExtension(".bik", NUFILETYPE_VIDEO, PC_PLATFORM);

#ifdef ANDROID
    AddExtension("_ios.gsc", NUFILETYPE_SCENE, PC_PLATFORM);
    AddExtension("_lr_ios.ghg", NUFILETYPE_CHARACTER, PC_PLATFORM);
#endif

    AddExtension(".an3", NUFILETYPE_ANIM, DEFAULT_PLATFORM);
    AddExtension(".bsa", NUFILETYPE_BSANIM, DEFAULT_PLATFORM);
    AddExtension(".ghg", NUFILETYPE_CHARACTER, DEFAULT_PLATFORM);
    AddExtension(".gsc", NUFILETYPE_SCENE, DEFAULT_PLATFORM);
    AddExtension(".pnt", NUFILETYPE_TEXTURE, DEFAULT_PLATFORM);
    AddExtension(".cu2", NUFILETYPE_CUTSCENE, DEFAULT_PLATFORM);
    AddExtension(".vag", NUFILETYPE_SFX, DEFAULT_PLATFORM);
    AddExtension(".mib", NUFILETYPE_SOUNDSTREAM, DEFAULT_PLATFORM);
    AddExtension(".pss", NUFILETYPE_VIDEO, DEFAULT_PLATFORM);

    AddExtension(".ca3", NUFILETYPE_ANIM, GAMECUBE_PLATFORM);
    AddExtension(".cbs", NUFILETYPE_BSANIM, GAMECUBE_PLATFORM);
    AddExtension(".chg", NUFILETYPE_CHARACTER, GAMECUBE_PLATFORM);
    AddExtension(".csc", NUFILETYPE_SCENE, GAMECUBE_PLATFORM);
    AddExtension(".ctx", NUFILETYPE_TEXTURE, GAMECUBE_PLATFORM);
    AddExtension(".cc2", NUFILETYPE_CUTSCENE, GAMECUBE_PLATFORM);
    AddExtension(".dsp", NUFILETYPE_SFX, GAMECUBE_PLATFORM);
    AddExtension(".gcm", NUFILETYPE_SOUNDSTREAM, GAMECUBE_PLATFORM);
    AddExtension(".h4m", NUFILETYPE_VIDEO, GAMECUBE_PLATFORM);

    AddExtension(".an3", NUFILETYPE_ANIM, XBOX_PLATFORM);
    AddExtension(".bsa", NUFILETYPE_BSANIM, XBOX_PLATFORM);
    AddExtension(".hx2", NUFILETYPE_CHARACTER, XBOX_PLATFORM);
    AddExtension(".nx2", NUFILETYPE_SCENE, XBOX_PLATFORM);
    AddExtension(".dds", NUFILETYPE_TEXTURE, XBOX_PLATFORM);
    AddExtension(".cu2", NUFILETYPE_CUTSCENE, XBOX_PLATFORM);
    AddExtension(".wavx", NUFILETYPE_SFX, XBOX_PLATFORM);
    AddExtension(".wavm", NUFILETYPE_SOUNDSTREAM, XBOX_PLATFORM);
    AddExtension(".wmv", NUFILETYPE_VIDEO, XBOX_PLATFORM);

    AddExtension(".an3", NUFILETYPE_ANIM, PSP_PLATFORM);
    AddExtension(".bsa", NUFILETYPE_BSANIM, PSP_PLATFORM);
    AddExtension(".phg", NUFILETYPE_CHARACTER, PSP_PLATFORM);
    AddExtension(".psc", NUFILETYPE_SCENE, PSP_PLATFORM);
    AddExtension(".pnt", NUFILETYPE_TEXTURE, PSP_PLATFORM);
    AddExtension(".cu2", NUFILETYPE_CUTSCENE, PSP_PLATFORM);
    AddExtension(".vag", NUFILETYPE_SFX, PSP_PLATFORM);
    AddExtension(".at3", NUFILETYPE_SOUNDSTREAM, PSP_PLATFORM);
    AddExtension(".pss", NUFILETYPE_VIDEO, PSP_PLATFORM);
}

static FILEEXTINFO *NuFileExtGetInfo(char *path, i32 path_len) {
    if (path_len == 0) {
        path_len = NuStrLen(path);
    }

    char *path_end = path + path_len;
    FILEEXTINFO *info = extensions;
    while (info->extension[0] != '\0') {
        if (MatchExtension(info->extension, path_end, path_len) != 0) {
            return info;
        }
        ++info;
    }
    return NULL;
}

i32 NuFileExtGetType(char *path, i32 path_len) {
    FILEEXTINFO *info = NuFileExtGetInfo(path, path_len);
    if (info != NULL)
        return info->type;
    return NUFILETYPE_UNKNOWN;
}

i32 NuFileExtConvert(char *dest, char *path) {
    i32 path_len = NuStrCpy(dest, path);
    FILEEXTINFO *source = NuFileExtGetInfo(path, path_len);
    if (source != NULL) {
        i32 source_type = source->type;
        if (source->platform == PC_PLATFORM) {
            return 1;
        }

        char *extension;
        char *out;
        FILEEXTINFO *target;
        target = extensions;
        for (; target->extension[0] != '\0'; ++target) {
            if (target->platform == PC_PLATFORM && target->type == source_type) {
                out = dest + path_len - source->len + target->len;
                extension = target->extension;
                *out = '\0';
                for (; *extension != '\0'; ++extension) {
                    *--out = *extension;
                }
                return 1;
            }
        }
    }
    return 0;
}

i32 NuFileExtGetExt(char *dest, i32 dest_size, NUFILETYPE type) {
    FILEEXTINFO *info = extensions;
    while (info->extension[0] != '\0') {
        if (info->platform == PC_PLATFORM && info->type == type) {
            if (info->len > dest_size) {
                return 0;
            }

            i32 i;
            const char *source;
            source = info->extension + info->len;
            i = 0;
            for (; i < info->len; ++i) {
                --source;
                dest[i] = *source;
            }
            dest[i] = '\0';
            return 1;
        }
        ++info;
    }
    return 0;
}

i32 NuFileExtRemove(char *dest, char *path) {
    i32 length = NuStrCpy(dest, path);
    FILEEXTINFO *info = NuFileExtGetInfo(path, length);
    if (info != NULL) {
        dest[length - info->len] = '\0';
        return info->type;
    }
    return NUFILETYPE_UNKNOWN;
}

static NUFILEPAKITEM *GetItems(NUFILEPAKHDR *hdr) {
    switch (hdr->type) {
        case 0x12345678:
            return (NUFILEPAKITEM *)((NUFILEPAKHDR_V0 *)hdr + 1);
        default:
            return (NUFILEPAKITEM *)((NUFILEPAKHDR_V1 *)hdr + 1);
        case 0x1234567a:
            return (NUFILEPAKITEM *)(hdr + 1);
    }
}

static u32 CalcChecksum(void *data, i32 len) {
    unsigned char *bytes;
    u32 sum;

    sum = 0x12345678;
    bytes = (unsigned char *)data;

    while (len != 0) {
        len--;
        sum += *(bytes++);
    }

    return sum;
}

static void DecryptData(NUFILEPAKHDR *hdr, u32 data_len, unsigned char *key, i32 key_len) {
    unsigned char *bytes;
    i32 i;

    data_len -= sizeof(NUFILEPAKHDR);
    i = 0;
    bytes = (unsigned char *)(hdr + 1);

    while (data_len != 0) {
        data_len--;
        *bytes = *bytes - key[i++];

        if (i >= key_len) {
            i = 0;
        }

        bytes++;
    }

    hdr->flags &= ~1;
}

NUFILEPAK_ERROR NuFilePakGetLastError(void) {
    return fpk_err;
}

void *NuFilePakLoadKey(char *filepath, VARIPTR *buf, VARIPTR buf_end, i32 alignment, unsigned char *key, u32 key_len) {
    NUFILEPAKHDR *hdr;
    i32 len;
    u32 stored_sum;

    fpk_err = NUFILEPAK_ERROR_NONE;

    if (alignment > 0x2000) {
        fpk_err = NUFILEPAK_ERROR_ALIGNMENT;
        return NULL;
    }

    alignment = alignment >= 4 ? alignment : 4;
    buf->addr = ALIGN(buf->addr, alignment);
    hdr = (NUFILEPAKHDR *)buf->void_ptr;

    len = NuFileLoadBufferVP(filepath, buf, &buf_end);
    if (len == 0) {
        fpk_err = NUFILEPAK_ERROR_FILENOTFOUND;
        return NULL;
    }

    if (len != hdr->file_size) {
        fpk_err = NUFILEPAK_ERROR_ALIGNMENT;
        return NULL;
    }

    switch (hdr->type) {
        // Version 1
        case 0x12345679:
            stored_sum = hdr->checksum;
            hdr->checksum = 0;
            if (CalcChecksum((void *)hdr, len) != stored_sum) {
                fpk_err = NUFILEPAK_ERROR_CHECKSUM;
                return NULL;
            }
            break;
        // Version 2
        case 0x1234567a:
            stored_sum = hdr->checksum;
            hdr->checksum = 0;
            if (CalcChecksum((void *)hdr, len) != stored_sum) {
                fpk_err = NUFILEPAK_ERROR_CHECKSUM;
                return NULL;
            }

            if ((hdr->flags & 1) != 0) {
                if (key == NULL) {
                    fpk_err = NUFILEPAK_ERROR_KEYMISMATCH;
                    return NULL;
                }

                DecryptData(hdr, len, key, key_len);

                stored_sum = hdr->decrypted_checksum;
                hdr->decrypted_checksum = 0;
                if (CalcChecksum((void *)hdr, len) != stored_sum) {
                    fpk_err = NUFILEPAK_ERROR_KEYMISMATCH;
                    return NULL;
                }
            }
            break;
        // Version 0
        case 0x12345678:
            break;
        default:
            fpk_err = NUFILEPAK_ERROR_UNSUPPORTEDTYPE;
            return NULL;
            break;
    }

    return (void *)hdr;
}

void *NuFilePakLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end, i32 alignment) {
    return NuFilePakLoadKey(filepath, buf, buf_end, alignment, NULL, 0);
}

i32 NuFilePakCondense(void *hdr_ptr) {
    // This operation uses the engine's signed-size C++ move provider.
    extern void memmove(void *dest, const void *source, i32 size);
    fpk_err = NUFILEPAK_ERROR_NONE;

    NUFILEPAKHDR *hdr = static_cast<NUFILEPAKHDR *>(hdr_ptr);
    NUFILEPAKITEM *items = GetItems(hdr);
    i32 data = static_cast<i32>(reinterpret_cast<uintptr_t>(hdr_ptr));
    i32 removed_size = 0;
    for (u32 i = 0; i < hdr->item_count; ++i) {
        if (!items[i].attr.removed) {
            if (items[i].attr.required) {
                if (removed_size != 0) {
                    if (items[i].alignment != 0) {
                        removed_size &= -items[i].alignment;
                    }
                    if (removed_size != 0) {
                        memmove(reinterpret_cast<void *>(data + items[i].data_offset - removed_size),
                                reinterpret_cast<void *>(data + items[i].data_offset), items[i].size);
                        items[i].data_offset -= removed_size;
                    }
                }
            } else {
                items[i].attr.removed = 1;
                removed_size += items[i].size;
                removed_size = (removed_size + (items[i].alignment - 1)) & -items[i].alignment;
            }
        }
    }
    hdr->file_size -= removed_size;
    return removed_size;
}

void NuFilePakRelocate(void *hdr_ptr, void *dest) {
    fpk_err = NUFILEPAK_ERROR_NONE;
    NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
    memmove(dest, hdr_ptr, static_cast<size_t>(hdr->file_size));
}

i32 NuFilePakGetItem(void *hdr_ptr, char *item_name) {
    u32 i;
    NUFILEPAKITEM *items;
    NUFILEPAKHDR_V0 *hdr_v0;
    NUFILEPAKHDR *hdr;

    fpk_err = NUFILEPAK_ERROR_NONE;

    hdr_v0 = (NUFILEPAKHDR_V0 *)hdr_ptr;
    hdr = (NUFILEPAKHDR *)hdr_ptr;
    items = GetItems(hdr);

    for (i = 0; hdr->item_count > i; i++) {
        if (NuStrICmp((char *)hdr_v0 + items[i].name_offset, item_name) == 0) {
            if (items[i].attr.removed) {
                return 0;
            }

            return i + 1;
        }
    }

    return 0;
}

i32 NuFilePakGetItemInfo(void *hdr_ptr, i32 item_handle, void **addr, i32 *size) {
    NUFILEPAKHDR *hdr;
    NUFILEPAKITEM *item;

    fpk_err = NUFILEPAK_ERROR_NONE;

    hdr = (NUFILEPAKHDR *)hdr_ptr;
    item_handle -= 1;

    if (item_handle > -1 && item_handle < hdr->item_count) {
        item = GetItems(hdr);
        item = &item[item_handle];

        if (!item->attr.removed) {
            if (addr != NULL) {
                *addr = (void *)((char *)hdr_ptr + item->data_offset);
            }

            if (size != NULL) {
                *size = item->size;
            }

            return 1;
        }
    }

    return 0;
}

i32 NuFilePakSetItemRequired(void *hdr_ptr, i32 item_handle, i32 required) {
    fpk_err = NUFILEPAK_ERROR_NONE;

    NUFILEPAKHDR *hdr = static_cast<NUFILEPAKHDR *>(hdr_ptr);
    --item_handle;
    if (item_handle >= 0 && item_handle < hdr->item_count) {
        NUFILEPAKITEM *items = GetItems(hdr);
        items = &items[item_handle];
        if (!items->attr.removed) {
            items->attr.required = required;
        }
        return 1;
    }
    return 0;
}

u32 NuFilePakGetSize(void *hdr_ptr) {
    fpk_err = NUFILEPAK_ERROR_NONE;
    NUFILEPAKHDR *hdr = (NUFILEPAKHDR *)hdr_ptr;
    return hdr->file_size;
}

char NuGetChar(NUFPAR *parser) {
    char c;
    i32 len;
    i32 length;

    i32 read_to = parser->buf_end >= 0 ? parser->buf_end : 0;

    if (parser->char_pos > parser->buf_end) {
        if (parser->buf_end + 1 <= parser->size) {
            len = NuFileRead(parser->file_handle, parser->file_buf,
                             parser->size - read_to <= 0x1000 ? parser->size - read_to : 0x1000);

            parser->buf_start = parser->buf_end + 1;
            parser->buf_end += len;
        } else {
            return '\0';
        }

        if (len == 0) {
            return '\0';
        }
    }

    c = parser->file_buf[parser->char_pos - parser->buf_start];
    parser->char_pos++;

    return c;
}

NUWCHAR NuGetWChar(NUFPAR *parser) {
    unsigned char lo;
    unsigned char hi;
    NUWCHAR c;

    lo = NuGetChar(parser);
    hi = NuGetChar(parser);

    c = lo + (hi << 8);

    return c;
}

i32 NuFParGetLineW(NUFPAR *parser) {
    NUWCHAR c;
    i32 len;
    NUWCHAR *line;
    i32 at_end_of_line;
    i32 is_done;
    i32 in_quoted_text;

    in_quoted_text = 0;
    line = (NUWCHAR *)parser->line_buf;
    len = 0;
    parser->line_pos = 0;
    parser->line_num++;
    is_done = false;

    do {
        c = NuGetWChar(parser);
        switch (c) {
            case '\r':
                NuGetWChar(parser);
            case '\n':
                if (len == 0 || !is_done) {
                    parser->line_num++;
                    is_done = false;

                    break;
                }
            case '\0':
                line[len] = '\0';
                return len;
            case '"':
                in_quoted_text = 1 - in_quoted_text;
                is_done = true;
                line[CLAMP_WIDE_WORD(len)] = c;
                len++;
                break;
            case ';':
                if (!in_quoted_text) {
                    if (len == 0 || !is_done) {
                        at_end_of_line = false;

                        do {
                            c = NuGetWChar(parser);
                            switch (c) {
                                case '\r':
                                    NuGetWChar(parser);
                                case '\n':
                                case '\0':
                                    at_end_of_line = true;
                                    break;
                                default:
                                    break;
                            }
                        } while (!at_end_of_line);

                        len = 0;
                        parser->line_pos = 0;
                        parser->line_num++;
                        is_done = false;
                    } else {
                        do {
                            c = NuGetWChar(parser);
                            switch (c) {
                                case '\r':
                                    NuGetWChar(parser);
                                case '\n':
                                case '\0':
                                    line[len] = '\0';
                                    return len;
                                default:
                                    break;
                            }
                        } while (true);

                        return len;
                    }

                    break;
                }
            default:
                is_done = true;
            case '\t':
            case ' ':
                line[CLAMP_WIDE_WORD(len)] = c;
                len++;
                break;
        }
    } while (true);
}

i32 NuFParGetLine(NUFPAR *parser) {
    char c;
    i32 len;
    i32 at_end_of_line;
    i32 is_done;
    i32 in_quoted_text;

    in_quoted_text = 0;

    if (parser->is_utf16) {
        return NuFParGetLineW(parser);
    }

    len = 0;
    parser->line_pos = 0;
    parser->line_num++;
    is_done = false;

    do {
        c = NuGetChar(parser);
        switch (c) {
            case '\r':
                NuGetChar(parser);
            case '\n':
                if (len == 0 || !is_done) {
                    parser->line_num++;
                    is_done = false;

                    break;
                }
            case '\0':
                parser->line_buf[len] = '\0';
                return len;
            case '"':
                in_quoted_text = 1 - in_quoted_text;
                is_done = true;
                parser->line_buf[CLAMP_LINE(len)] = c;
                len++;
                break;
            case ';':
                if (!in_quoted_text) {
                    if (len == 0 || !is_done) {
                        at_end_of_line = false;

                        do {
                            c = NuGetChar(parser);
                            switch (c) {
                                case '\r':
                                    NuGetChar(parser);
                                case '\n':
                                case '\0':
                                    at_end_of_line = true;
                                    break;
                                default:
                                    break;
                            }
                        } while (!at_end_of_line);

                        len = 0;
                        parser->line_pos = 0;
                        parser->line_num++;
                        is_done = false;
                    } else {
                        do {
                            c = NuGetChar(parser);
                            switch (c) {
                                case '\r':
                                    NuGetChar(parser);
                                case '\n':
                                case '\0':
                                    parser->line_buf[len] = '\0';
                                    return len;
                                default:
                                    break;
                            }
                        } while (true);

                        return len;
                    }

                    break;
                }
            default:
                is_done = true;
            case '\t':
            case ' ':
                parser->line_buf[CLAMP_LINE(len)] = c;
                len++;
                break;
        }
    } while (true);
}

void NuFParUnGetWord(NUFPAR *parser) {
    parser->line_pos = old_line_pos;
}

i32 NuFParGetWordW(NUFPAR *parser) {
    NUWCHAR *line = (NUWCHAR *)parser->line_buf;
    NUWCHAR *word = (NUWCHAR *)parser->word_buf;
    i32 len = 0;
    i32 in_quoted_text = 0;

    old_line_pos = parser->line_pos;

    NUWCHAR c;

    while (line[CLAMP_WIDE_LINE(parser->line_pos)] != 0) {
        c = line[parser->line_pos];

        if (parser->separator_tokens != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_tokens, c) != NULL) {
                if (len == 0) {
                    word[len] = c;
                    len++;
                    parser->line_pos++;
                }

                word[CLAMP_WIDE_WORD(len)] = '\0';
                return len;
            }
        }

        if (parser->separator_list != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_list, c) != NULL) {
                c = ' ';
            }
        }

        switch (c & 0xff) {
            case ' ':
            case ',':
            case '\t':
                if (!in_quoted_text) {
                    if (len != 0) {
                        word[CLAMP_WIDE_WORD(len)] = '\0';
                        return len;
                    }

                    break;
                }
            default:
                if (c == '"') {
                    if (in_quoted_text && line[parser->line_pos + 1] == '"') {
                        word[CLAMP_WIDE_WORD(len)] = c;
                        len++;
                        parser->line_pos++;
                    } else {
                        in_quoted_text = 1 - in_quoted_text;
                    }
                } else {
                    word[CLAMP_WIDE_WORD(len)] = c;
                    len++;
                }

                break;
        }

        parser->line_pos++;
    }

    word[CLAMP_WIDE_WORD(len)] = '\0';
    return len;
}

i32 NuFParGetWord(NUFPAR *parser) {
    i32 len;
    i32 in_quoted_text;
    i32 found_quotes = 0;

    if (parser->is_utf16) {
        return NuFParGetWordW(parser);
    }

    len = 0;
    in_quoted_text = 0;

    old_line_pos = parser->line_pos;

    while (parser->line_buf[CLAMP_LINE(parser->line_pos)] != 0) {
        char c = parser->line_buf[parser->line_pos];

        if (parser->separator_tokens != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_tokens, c) != NULL) {
                if (len == 0) {
                    parser->word_buf[len] = c;
                    len++;
                    parser->line_pos++;
                }

                parser->word_buf[CLAMP_WORD(len)] = '\0';

                return len;
            }
        }

        if (parser->separator_list != NULL && !in_quoted_text) {
            if (NuStrChr(parser->separator_list, c) != NULL) {
                c = ' ';
            }
        }

        switch (c) {
            case ' ':
            case ',':
            case '\t':
                if (!in_quoted_text) {
                    if (len != 0) {
                        parser->word_buf[CLAMP_WORD(len)] = '\0';
                        return len;
                    }

                    break;
                }
            default:
                if (c == '"') {
                    in_quoted_text = 1 - in_quoted_text;
                    found_quotes = 1;
                } else {
                    parser->word_buf[CLAMP_WORD(len)] = c;
                    len++;
                }

                break;
        }

        parser->line_pos++;

        if (found_quotes && in_quoted_text == 0 && len == 0) {
            break;
        }
    }

    parser->word_buf[CLAMP_WORD(len)] = '\0';
    return len;
}

i32 NuFParGetInt(NUFPAR *parser) {
    char buf[64];

    NuFParGetWord(parser);
    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] != '\0') {
        if (buf[0] == '$') {
            return NuHexStringToI(buf + 1);
        } else if (buf[0] == '0' && (buf[1] == 'x' || buf[1] == 'X')) {
            return NuHexStringToI(buf + 2);
        } else {
            return NuAToI(buf);
        }
    } else {
        return 0;
    }
}

f32 NuFParGetFloat(NUFPAR *parser) {
    char buf[64];

    NuFParGetWord(parser);
    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] != '\0') {
        return NuAToF(buf);
    } else {
        return 0.0f;
    }
}

void NuFParGetOptionalFloat(NUFPAR *parser, f32 *value) {
    char buf[64];
    NuFParGetWord(parser);
    if (parser->is_utf16)
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    else
        NuStrCpy(buf, parser->word_buf);
    if (value != NULL && buf[0] != '\0') {
        *value = NuAToF(buf);
        if (*value == 0.0f)
            NuFParUnGetWord(parser);
    }
}

void NuFParGetOptionalInt(NUFPAR *parser, i32 *value) {
    char buf[64];
    NuFParGetWord(parser);
    if (parser->is_utf16)
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    else
        NuStrCpy(buf, parser->word_buf);
    if (value != NULL && buf[0] != '\0') {
        *value = NuAToI(buf);
        if (*value == 0)
            NuFParUnGetWord(parser);
    }
}

i32 NuFParGetIntRDP(NUFPAR *parser) {
    char buf[64];

    NuFParGetWord(parser);
    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] != '\0') {
        return NuRDPI(buf);
    } else {
        return 0;
    }
}

f32 NuFParGetFloatRDP(NUFPAR *parser) {
    char buf[64];

    NuFParGetWord(parser);
    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] != '\0') {
        return NuRDPF(buf);
    } else {
        return 0.0f;
    }
}

void NuFParGetPos(NUFPAR *parser, NUFPARPOS *position) {
    position->line_num = parser->line_num;
    position->line_pos = parser->line_pos;
    position->char_pos = parser->char_pos;
    position->buf_start = parser->buf_start;
    position->buf_end = parser->buf_end;
}

void NuFParSetPos(NUFPAR *parser, NUFPARPOS *position) {
    parser->char_pos = position->char_pos;
    parser->line_pos = position->line_pos;
    if (parser->char_pos < parser->buf_start || parser->char_pos > parser->buf_end)
        parser->buf_end = parser->char_pos - 1;
}

i32 NuFParPushCom(NUFPAR *parser, NUFPCOMJMP *commands) {
    if (parser->command_pos >= 7) {
        return -1;
    }

    parser->command_pos++;

    parser->command_stack.jump[parser->command_pos] = commands;
    parser->command_stack2.jump[parser->command_pos] = NULL;

    return parser->command_pos;
}

i32 NuFParPushComCTX(NUFPAR *parser, NUFPCOMJMPCTX *commands) {
    if (parser->command_pos >= 7) {
        return -1;
    }

    parser->command_pos++;

    parser->command_stack.jump_ctx[parser->command_pos] = commands;
    parser->command_stack2.jump_ctx[parser->command_pos] = NULL;

    return parser->command_pos;
}

i32 NuFParPushCom2(NUFPAR *parser, NUFPCOMJMP *commands, NUFPCOMJMP *commands2) {
    if (parser->command_pos >= 7) {
        return -1;
    }

    parser->command_pos++;

    parser->command_stack.jump[parser->command_pos] = commands;
    parser->command_stack2.jump[parser->command_pos] = commands2;

    return parser->command_pos;
}

i32 NuFParPushComCTX2(NUFPAR *parser, NUFPCOMJMPCTX *commands, NUFPCOMJMPCTX *commands2) {
    if (parser->command_pos >= 7) {
        return -1;
    }

    parser->command_pos++;

    parser->command_stack.jump_ctx[parser->command_pos] = commands;
    parser->command_stack2.jump_ctx[parser->command_pos] = commands2;

    return parser->command_pos;
}

void NuFParPopCom(NUFPAR *parser) {
    if (parser->command_pos > -1) {
        parser->command_pos--;
    }
}

i32 NuFParInterpretWord(NUFPAR *parser) {
    char buf[64];
    i32 i;

    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] == '\0') {
        return 0;
    }

    if (buf[0] == ';') {
        return 0;
    }

    if (parser->command_pos > -1) {
        for (i = 0; parser->command_stack.jump[parser->command_pos][i].fn_name != NULL; i++) {
            if (NuStrICmp(parser->command_stack.jump[parser->command_pos][i].fn_name, buf) == 0) {
                parser->command_stack.jump[parser->command_pos][i].fn(parser);

                return 1;
            }
        }

        if (parser->command_stack2.jump[parser->command_pos] != NULL) {
            for (i = 0; parser->command_stack2.jump[parser->command_pos][i].fn_name != NULL; i++) {
                if (NuStrICmp(parser->command_stack2.jump[parser->command_pos][i].fn_name, buf) == 0) {
                    parser->command_stack2.jump[parser->command_pos][i].fn(parser);

                    return 1;
                }
            }
        }
    }

    if (fnInterpreterError != NULL) {
        (*fnInterpreterError)(parser);
    }

    return 0;
}

i32 NuFParInterpretWordCTX(NUFPAR *parser, void *ctx) {
    char buf[64];
    i32 i;

    if (parser->is_utf16) {
        NuUnicodeToAscii(buf, (NUWCHAR16 *)parser->word_buf);
    } else {
        NuStrCpy(buf, parser->word_buf);
    }

    if (buf[0] == '\0') {
        return 0;
    }

    if (buf[0] == ';') {
        return 0;
    }

    if (parser->command_pos > -1) {
        for (i = 0; parser->command_stack.jump_ctx[parser->command_pos][i].fn_name != NULL; i++) {
            if (NuStrICmp(parser->command_stack.jump_ctx[parser->command_pos][i].fn_name, buf) == 0) {
                parser->command_stack.jump_ctx[parser->command_pos][i].fn(parser, ctx);

                return 1;
            }
        }

        if (parser->command_stack2.jump_ctx[parser->command_pos] != NULL) {
            for (i = 0; parser->command_stack2.jump_ctx[parser->command_pos][i].fn_name != NULL; i++) {
                if (NuStrICmp(parser->command_stack2.jump_ctx[parser->command_pos][i].fn_name, buf) == 0) {
                    parser->command_stack2.jump_ctx[parser->command_pos][i].fn(parser, ctx);

                    return 1;
                }
            }
        }
    }

    if (fnInterpreterError != NULL) {
        (*fnInterpreterError)(parser);
    }

    return 0;
}

nufpcomfn *NuFParSetInterpreterErrorHandler(nufpcomfn *fn) {
    nufpcomfn *old = fnInterpreterError;

    fnInterpreterError = fn;

    return old;
}

void NuFParSuspend(NUFPAR *parser) {
    if (parser->file_handle >= 0x400 && parser->file_handle < 0x800) {
        return;
    }

    if (parser->file_name[0] == '\0' || parser->file_handle == 0) {
        // Error logging goes here.
    }

    NuFileClose(parser->file_handle);

    parser->file_handle = 0;
}

void NuFParResume(NUFPAR *parser) {
    if (parser->file_handle >= 0x400 && parser->file_handle < 0x800) {
        return;
    }

    parser->file_handle = NuFileOpen(parser->file_name, NUFILE_READ);

    NuFileSeek(parser->file_handle, parser->char_pos, NUFILE_SEEK_START);

    parser->buf_end = parser->char_pos - 1;
}

void NuFParInit(i32 max_count, i32 buf_size) {
    fpars = _fpars;
    memset(fpars, 0, PARSER_COUNT * sizeof(NUFPAR));

    fparbuffers = _fparbuffers;
}

static NUFPAR *NuAllocFPar() {
    if (fpars == NULL) {
        NuFParInit(PARSER_COUNT, PARSER_BUF_SIZE);
    }

    for (i32 i = 0; i < PARSER_COUNT; i++) {

        if (fpars[i].file_buf == NULL) {
            memset(&fpars[i], 0, sizeof(NUFPAR));

            fpars[i].file_buf = &fparbuffers[i * PARSER_BUF_SIZE];
            memset(fpars[i].file_buf, 0, PARSER_BUF_SIZE);

            return &fpars[i];
        }
    }

    return NULL;
}

static void NuFreeFPar(NUFPAR *parser) {
    parser->file_buf = NULL;
}

void NuFParClose(NUFPAR *parser) {
    NuFreeFPar(parser);
}

NUFPAR *NuFParOpen(NUFILE file_handle) {
    NUFPAR *parser = NuAllocFPar();
    if (parser != NULL) {
        parser->file_handle = file_handle;
        parser->buf_end = -1;
        parser->line_num = -1;
        parser->command_pos = -1;
        parser->separator_list = NULL;
        parser->separator_tokens = NULL;
        parser->line_buf = parser->line_buf_store;
        parser->word_buf = parser->word_buf_store;
        parser->line_buf_size = 0x200;
        parser->word_buf_size = 0x200;

        NUWCHAR first = NuFileReadWChar(file_handle);
        parser->is_utf16 = 0;
        parser->is_utf8 = 0;

        unsigned char next;
        if (first == 0xfffe) {
            parser->is_utf16 = 2;
        } else if (first == 0xfeff) {
            parser->is_utf16 = 1;
        } else if (first == 0xbbef) {
            next = NuFileReadChar(file_handle);
            if (next == 0xbf) {
                parser->is_utf8 = 1;
            }
        } else if (first == 0xefbb) {
            next = NuFileReadChar(file_handle);
            if (next == 0xbf) {
                parser->is_utf8 = 2;
            }
        }

        if (parser->is_utf16 == 0 && parser->is_utf8 == 0) {
            NuFileSeek(file_handle, 0, NUFILE_SEEK_START);
        }

        parser->size = NuFileOpenSize(file_handle);
    }

    return parser;
}

void NuFParDestroy(NUFPAR *parser) {
    NUFILE file_handle = parser->file_handle;

    NuFParClose(parser);
    NuFileClose(file_handle);
}

NUFPAR *NuFParCreate(char *filename) {
    NUFILE file_handle;

    if (NuFileExists(filename)) {
        file_handle = NuFileOpen(filename, NUFILE_READ);
        if (file_handle != 0) {
            NUFPAR *parser = NuFParOpen(file_handle);
            if (parser != NULL) {
                NuStrCpy(parser->file_name, filename);
                return parser;
            }

            NuFileClose(file_handle);
        }
    }

    return NULL;
}

NUFPAR *NuFParCreateGivenFH(char *filename, NUFILE file_handle) {
    if (file_handle != 0) {
        NUFPAR *parser = NuFParOpen(file_handle);
        if (parser != NULL) {
            NuStrCpy(parser->file_name, filename);
            return parser;
        }

        NuFileClose(file_handle);
    }

    return NULL;
}

NUFPAR *NuFParCreateMem(char *name, char *buffer, i32 bufferSize) {
    if (bufferSize != 0 && buffer != NULL) {
        NUFILE file_handle = NuMemFileOpen(buffer, bufferSize, NUFILE_READ);
        if (file_handle != 0) {
            NUFPAR *parser = NuFParOpen(file_handle);
            if (parser != NULL) {
                NuStrCpy(parser->file_name, name);
                return parser;
            }

            NuFileClose(file_handle);
        }
    }

    return NULL;
}

int NuLinkedListCheck(NULISTHDR *list) {
    NULISTLNK *node;
    int count;
    NULISTLNK *previous = NULL;
    count = 0;
    node = list->head;
    while (node != NULL) {
        count++;
        previous = node;
        node = node->next;
    }
    return count;
}

void NuLinkedListAppend(NULISTHDR *list, NULISTLNK *node) {
    node->next = NULL;
    node->prev = list->tail;

    if (list->tail != NULL) {
        list->tail->next = node;
    }

    list->tail = node;

    if (list->head == NULL) {
        list->head = node;
    }
}

void NuLinkedListInsert(NULISTHDR *list, NULISTLNK *node) {
    node->next = list->head;
    node->prev = NULL;

    if (list->head != NULL) {
        list->head->prev = node;
    }

    if (list->tail == NULL) {
        list->tail = node;
    }

    list->head = node;
}

void NuLinkedListInsertBefore(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
    if (position != NULL) {
        node->next = position;
        node->prev = position->prev;
        if (position->prev != NULL) {
            position->prev->next = node;
        } else {
            list->head = node;
        }
        position->prev = node;
    } else {
        NuLinkedListInsert(list, node);
    }
}

void NuLinkedListInsertAfter(NULISTHDR *list, NULISTLNK *position, NULISTLNK *node) {
    if (position != NULL) {
        node->prev = position;
        node->next = position->next;
        if (position->next != NULL) {
            position->next->prev = node;
        } else {
            list->tail = node;
        }
        position->next = node;
    } else {
        NuLinkedListAppend(list, node);
    }
}

void NuLinkedListRemove(NULISTHDR *list, NULISTLNK *node) {
    if (node->next != NULL) {
        node->next->prev = node->prev;
    } else {
        list->tail = node->prev;
    }

    if (node->prev != NULL) {
        node->prev->next = node->next;
    } else {
        list->head = node->next;
    }
}

NULISTLNK *NuLinkedListGetHead(NULISTHDR *list) {
    return list->head;
}

NULISTLNK *NuLinkedListGetTail(NULISTHDR *list) {
    return list->tail;
}

NULISTLNK *NuLinkedListGetNext(NULISTHDR *list, NULISTLNK *node) {
    if (node != NULL) {
        return node->next;
    } else {
        return list->head;
    }
}

NULISTLNK *NuLinkedListGetPrev(NULISTHDR *list, NULISTLNK *node) {
    if (node != NULL) {
        return node->prev;
    } else {
        return list->tail;
    }
}

NUMEMBLK *NuMemBlkCreate(i32 element_size, i32 count, u32 alignment_mask) {
    NUMEMBLKLINK *link;
    i32 i;
    u32 header_size;
    u32 stride;
    NUMEMBLK *pool;
    usize data;
    u32 stride_words;

    if (element_size < sizeof(NUMEMBLKLINK)) {
        element_size = sizeof(NUMEMBLKLINK);
    }
    header_size = (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
    stride = (element_size + alignment_mask) & ~alignment_mask;
    pool = (NUMEMBLK *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(header_size + stride * count, 4, 1, "", 0);
    data = (usize)pool;
    data = (usize)((NUMEMBLK *)data + 1);
    data += alignment_mask;
    data &= ~(usize)alignment_mask;
    pool->free_list = (NUMEMBLKLINK *)data;
    pool->stride = stride;
    pool->capacity = count;
    pool->free_count = count;
    pool->flags = 0;
    stride_words = stride / sizeof(u32);
    link = pool->free_list;
    for (i = 0; i < count - 1; i++) {
        link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
        link = link->next;
    }
    link->next = NULL;
    return pool;
}

NUMEMBLK *NuMemBlkCreateEx(i32 element_size, i32 count, u32 alignment_mask, void *storage) {
    NUMEMBLKLINK *link;
    i32 i;
    u32 header_size;
    u32 stride;
    NUMEMBLK *pool;
    usize data;
    u32 stride_words;

    if (element_size < sizeof(NUMEMBLKLINK)) {
        element_size = sizeof(NUMEMBLKLINK);
    }
    header_size = (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
    stride = (element_size + alignment_mask) & ~alignment_mask;
    pool = (NUMEMBLK *)storage;
    data = (usize)pool;
    data += (sizeof(NUMEMBLK) + alignment_mask) & ~alignment_mask;
    pool->stride = stride;
    pool->capacity = count;
    pool->free_count = count;
    pool->flags = NUMEMBLK_EXTERNAL_STORAGE;
    if (count != 0) {
        pool->free_list = (NUMEMBLKLINK *)data;
        stride_words = stride / sizeof(u32);
        link = pool->free_list;
        for (i = 0; i < count - 1; i++) {
            link->next = (NUMEMBLKLINK *)((u32 *)link + stride_words);
            link = link->next;
        }
        link->next = NULL;
    } else {
        pool->free_list = NULL;
    }
    return pool;
}

NUMEMBLK *NuMemBlkCreateVari(i32 element_size, i32 count, u32 alignment_mask, VARIPTR *buffer) {
    i32 size = NuMemBlkSize(element_size, count, alignment_mask);
    buffer->addr += alignment_mask;
    buffer->addr &= ~(usize)alignment_mask;
    NUMEMBLK *pool = NuMemBlkCreateEx(element_size, count, alignment_mask, buffer->void_ptr);
    buffer->addr += size;
    return pool;
}

i32 NuMemBlkSize(i32 element_size, i32 count, i32 alignment_mask) {
    i32 header_size = (16 + alignment_mask) & ~alignment_mask;
    i32 stride = (element_size + alignment_mask) & ~alignment_mask;
    i32 size = header_size + stride * count;
    return size;
}

void NuMemBlkDestroy(NUMEMBLK *pool) {
    if (!(pool->flags & NUMEMBLK_EXTERNAL_STORAGE)) {
        NU_FREE(pool);
    }
}

void NuMemBlkCheckFreeList(NUMEMBLK *pool) {
    NUMEMBLKLINK *link = pool->free_list;
    i32 count = 0;
    while (link != NULL) {
        link = link->next;
        count++;
    }
}

void *NuMemBlkAlloc(NUMEMBLK *pool) {
    NUMEMBLKLINK *block;
    if ((block = pool->free_list) != NULL) {
        pool->free_list = pool->free_list->next;
        pool->free_count--;
        memset(block, -1, pool->stride);
    }
    return block;
}

void NuMemBlkFree(NUMEMBLK *pool, void *block) {
    NUMEMBLKLINK *link = (NUMEMBLKLINK *)block;
    pool->free_count++;
    memset(block, -2, pool->stride);
    link->next = pool->free_list;
    pool->free_list = link;
}

void NuAllocHighInit(usize buffer, u32 size) {
    // Preserve the low-bit masks emitted by the original, including its
    // unusual treatment of unaligned inputs.
    if (buffer & 15) {
        buffer = (buffer + 15) & 15;
        size -= 15;
    }
    if (size & 15) {
        size = (size - 15) & 15;
    }
    freep = (NUMEMHIGHBLOCK *)buffer;
    freep->next = freep;
    freep->units = size;
}

void *NuAllocHigh(u32 size) {
    NUMEMHIGHBLOCK *block;
    NUMEMHIGHBLOCK *previous;
    u32 units;
    units = (size + sizeof(NUMEMHIGHBLOCK) - 1) / sizeof(NUMEMHIGHBLOCK) + 1;
    previous = freep;
    block = previous->next;
    while (true) {
        if (block->units >= units) {
            if (block->units == units) {
                previous->next = block->next;
            } else {
                block->units -= units;
                block += block->units;
                block->units = units;
            }
            freep = previous;
            return block + 1;
        }
        if (block == freep)
            return NULL;
        previous = block;
        block = block->next;
    }
}

void NuFreeHigh(void *ptr) {
    NUMEMHIGHBLOCK *previous;
    NUMEMHIGHBLOCK *block = (NUMEMHIGHBLOCK *)ptr - 1;
    previous = freep;
    while (!(block > previous && block < previous->next)) {
        if (previous >= previous->next && (block > previous || block < previous->next))
            break;
        previous = previous->next;
    }
    if (block + block->units == previous->next) {
        block->units += previous->next->units;
        block->next = previous->next->next;
    } else {
        block->next = previous->next;
    }
    if (previous + previous->units == block) {
        previous->units += block->units;
        previous->next = block->next;
    } else {
        previous->next = block;
    }
    freep = previous;
}

static int ElOverlaps(nuqtdim_s *a, nuqtdim_s *b) {
    if (b->x0 < a->x1 && b->x1 > a->x0 && b->y1 < a->y0 && b->y0 > a->y1)
        return 1;
    return 0;
}

static i32 InsertData(nuqthdr_s *header, i32 index, void *item) {
    u8 *destination = header->entries[index].data;
    if (destination == NULL) {
        // The original tests the remaining capacity in this direction and copies
        // through the cached null pointer, even after assigning the entry data.
        if (header->data_capacity - header->data_used <= header->element_size) {
            header->entries[index].data = header->data + header->data_used;
            memcpy(destination, item, header->element_size);
            header->data_used += header->element_size;
            ++header->entries[index].count;
            return 1;
        }
    } else {
        u8 *source = header->data + header->data_used;
        u8 *target = source + header->element_size;
        if (header->data + header->data_capacity >= target) {
            while (source != destination) {
                --target;
                --source;
                *target = *source;
            }
            memcpy(destination, item, header->element_size);
            header->data_used += header->element_size;
            ++header->entries[index].count;
            return 1;
        }
    }
    return 0;
}

static void RemoveData(nuqthdr_s *header, char *data, i32 count) {
    char *source;
    char *destination;
    i32 length = header->element_size * count;
    char *end = (char *)header->data + header->data_used;
    destination = data;
    source = destination + length;
    while (source < end) {
        *destination = *source;
        ++destination;
        ++source;
    }
}

static i32 AddNode(nuqthdr_s *header, i32 child) {
    i32 index = 0;
    if (header->entry_count < header->entry_capacity) {
        index = header->entry_count;
        memset(&header->entries[index], 0, sizeof(nuqtentry_s));
        header->entries[index].child = child;
        header->entries[index].count = -1;
        ++header->entry_count;
    }
    return index;
}

static i32 AddElementR(nuqthdr_s *header, i32 index, nuqtdim_s *bounds, nuqtdim_s *item_bounds, void *item, i32 depth) {
    i32 result = 0;
    --depth;
    nuqtdim_s q0 = {bounds->x0, (bounds->x0 + bounds->x1) / 2.0f, bounds->y0, (bounds->y0 + bounds->y1) / 2.0f};
    nuqtdim_s q1 = {(bounds->x0 + bounds->x1) / 2.0f, bounds->x1, bounds->y0, (bounds->y0 + bounds->y1) / 2.0f};
    nuqtdim_s q2 = {bounds->x0, (bounds->x0 + bounds->x1) / 2.0f, (bounds->y0 + bounds->y1) / 2.0f, bounds->y1};
    nuqtdim_s q3 = {(bounds->x0 + bounds->x1) / 2.0f, bounds->x1, (bounds->y0 + bounds->y1) / 2.0f, bounds->y1};
    if (header->entries[index].count >= 0) {
        if (header->entries[index].count < header->field_30 || depth == 0) {
            result = InsertData(header, index, item);
            return result;
        }

        u8 *old_data = header->entries[index].data;
        header->entries[index].children[0] = AddNode(header, index);
        header->entries[index].children[1] = AddNode(header, index);
        header->entries[index].children[2] = AddNode(header, index);
        header->entries[index].children[3] = AddNode(header, index);
        // The original rejects a nonzero first child, unlike the other three children.
        if (header->entries[index].children[0] != 0 || header->entries[index].children[1] == 0 ||
            header->entries[index].children[2] == 0 || header->entries[index].children[3] == 0) {
            header->entries[index].data = old_data;
            result = InsertData(header, index, item);
            return result;
        }
        i32 i;
        u8 *old_item = old_data;
        for (i = 0; i < header->entries[index].count; ++i) {
            // The original tests the incoming item's bounds when redistributing
            // existing data, and uses the saved data pointer for each recursive item.
            if (ElOverlaps(&q0, item_bounds))
                result = AddElementR(header, header->entries[index].children[0], &q0, item_bounds, old_item, depth);
            if (ElOverlaps(&q1, item_bounds))
                result = AddElementR(header, header->entries[index].children[1], &q1, item_bounds, old_item, depth);
            if (ElOverlaps(&q2, item_bounds))
                result = AddElementR(header, header->entries[index].children[2], &q2, item_bounds, old_item, depth);
            if (ElOverlaps(&q3, item_bounds))
                result = AddElementR(header, header->entries[index].children[3], &q3, item_bounds, old_item, depth);
            old_item += header->element_size;
        }
        RemoveData(header, reinterpret_cast<char *>(old_data), header->entries[index].count);
        header->entries[index].count = -1;
    }
    if (header->entries[index].count < 0) {
        if (ElOverlaps(&q0, item_bounds)) {
            if (header->entries[index].children[0] != 0)
                result = AddElementR(header, header->entries[index].children[0], &q0, item_bounds, item, depth);
        }
        if (ElOverlaps(&q1, item_bounds)) {
            if (header->entries[index].children[1] != 0)
                result = AddElementR(header, header->entries[index].children[1], &q1, item_bounds, item, depth);
        }
        if (ElOverlaps(&q2, item_bounds)) {
            if (header->entries[index].children[2] != 0)
                result = AddElementR(header, header->entries[index].children[2], &q2, item_bounds, item, depth);
        }
        if (ElOverlaps(&q3, item_bounds)) {
            if (header->entries[index].children[3] != 0)
                result = AddElementR(header, header->entries[index].children[3], &q3, item_bounds, item, depth);
        }
    }
    return result;
}

extern "C" {
extern "C" void NuQTAddElement(nuqthdr_s *header, void *item, f32 x0, f32 x1, f32 y0, f32 y1) {
    u16 root = 0;
    nuqtdim_s bounds;
    bounds.y0 = header->bounds_values[1];
    bounds.y1 = header->bounds_values[2];
    bounds.x0 = header->bounds_values[3];
    bounds.x1 = header->bounds_values[4];
    nuqtdim_s item_bounds;
    item_bounds.y0 = y0;
    item_bounds.y1 = y1;
    item_bounds.x0 = x0;
    item_bounds.x1 = x1;
    AddElementR(header, root, &bounds, &item_bounds, item, header->field_34);
}

extern "C" i32 NuQTCreate(i32 entry_capacity, i32 data_capacity, i32 element_size, i32 field_34, i32 field_30,
                          f32 field_04, f32 field_0c, f32 field_08, f32 field_10, VARIPTR *cursor, VARIPTR *end) {
    cursor->addr = (cursor->addr + 15) & ~15;
    i32 required_size = sizeof(nuqthdr_s) + sizeof(nuqtentry_s) * entry_capacity + 16 + data_capacity;
    if (cursor->addr + required_size <= end->addr) {
        nuqthdr_s *header = reinterpret_cast<nuqthdr_s *>(cursor->void_ptr);
        cursor->addr += sizeof(nuqthdr_s);
        header->field_00[0] = 0;
        header->entry_count = 0;
        header->entry_capacity = entry_capacity;
        header->data_used = 0;
        header->data_capacity = data_capacity;
        header->bounds_values[1] = field_04;
        header->bounds_values[3] = field_0c;
        header->bounds_values[2] = field_08;
        header->bounds_values[4] = field_10;
        header->element_size = element_size;
        header->field_34 = field_34;
        header->field_30 = field_30;
        header->entries = reinterpret_cast<nuqtentry_s *>(cursor->void_ptr);
        cursor->addr += sizeof(nuqtentry_s) * entry_capacity;
        cursor->addr = (cursor->addr + 15) & ~15;
        header->data = cursor->u8_ptr;
        cursor->addr += data_capacity;
    }
    return 0;
}

}

static void NuQTUnfixAddress(nuqthdr_s *header) {
    uintptr_t base = -(uintptr_t)header;
    for (i32 index = 0; index < header->entry_count; ++index) {
        if (header->entries[index].count > 0)
            header->entries[index].data = header->entries[index].data + base;
    }
    header->entries = (nuqtentry_s *)((u8 *)header->entries + base);
    header->data = header->data + base;
}

static void NuQTFixAddress(nuqthdr_s *header) {
    uintptr_t base = (uintptr_t)header;
    header->entries = (nuqtentry_s *)((u8 *)header->entries + base);
    header->data = header->data + base;
    for (i32 index = 0; index < header->entry_count; ++index) {
        if (header->entries[index].count > 0)
            header->entries[index].data = header->entries[index].data + base;
    }
}

extern "C" {
extern "C" nuqthdr_s *NuQTRead(char *path, VARIPTR *cursor, VARIPTR *end) {
    nuqthdr_s *header;
    i32 size;

    cursor->addr = (cursor->addr + 15) & ~15;
    header = reinterpret_cast<nuqthdr_s *>(cursor->void_ptr);
    size = NuFileLoadBuffer(path, cursor->void_ptr, end->addr - cursor->addr);
    if (size != 0) {
        cursor->addr += size;
        NuQTFixAddress(header);
        return header;
    }
    return NULL;
}

extern "C" i32 NuQTWrite(char *path, nuqthdr_s *header) {
    NUFILE file = NuFileOpen(path, NUFILE_WRITE);
    if (file != 0) {
        usize size = 0 - reinterpret_cast<usize>(header);
        size += reinterpret_cast<usize>(header->data);
        size += header->data_capacity;
        NuQTUnfixAddress(header);
        NuFileWrite(file, header, static_cast<i32>(size));
        NuQTFixAddress(header);
        NuFileClose(file);
        return 1;
    }
    return 0;
}

}

static i32 isnumordot(char c) {
    if ((c >= '0' && c <= '9') || c == '.') {
        return 1;
    }

    return 0;
}

static void get_tok(RDPFCTXT *ctx) {
    char var_name_buf[256];
    f32 multiplier;
    i32 var_name_pos;
    i32 ret;

    // In theory, this should consume whitespace, but instead it just transforms
    // spaces into exclamation points and tabs into line feeds. It _does_ make
    // the loop end and the token matching code below ignores whitespace anyhow.
    while (true) {
        if (ctx->buf[ctx->pos] == '\0') {
            ctx->cur_tok = TOK_END;

            return;
        }

        if (ctx->buf[ctx->pos] == ' ') {
            // NOTE: This was probably meant to increment position only, but it
            // doesn't. See the comment on the loop.
            ctx->buf[ctx->pos]++;
            continue;
        }

        if (ctx->buf[ctx->pos] == '\t') {
            ctx->buf[ctx->pos]++;
            continue;
        }

        break;
    }

    switch (ctx->buf[ctx->pos]) {
        case ';':
            ctx->cur_tok = TOK_END;
            break;
        case '+':
            ctx->cur_tok = TOK_ADD;
            ctx->pos++;
            break;
        case '-':
            ctx->cur_tok = TOK_SUB;
            ctx->pos++;
            break;
        case '*':
            ctx->cur_tok = TOK_MUL;
            ctx->pos++;
            break;
        case '/':
            ctx->cur_tok = TOK_DIV;
            ctx->pos++;
            break;
        case '(':
            ctx->cur_tok = TOK_LPAREN;
            ctx->pos++;
            break;
        case ')':
            ctx->cur_tok = TOK_RPAREN;
            ctx->pos++;
            break;
        default:
            break;
        case '.':
        case '0' ... '9':
            ctx->cur_tok = TOK_NUM;
            ctx->cur_val = 0.0f;

            multiplier = 0.0f;

            while (isnumordot(ctx->buf[ctx->pos])) {
                if (ctx->buf[ctx->pos] == '.') {
                    multiplier = 0.1f;
                } else if (multiplier != 0.0f) {
                    ctx->cur_val += (f32)(ctx->buf[ctx->pos] - 0x30) * multiplier;
                    multiplier /= 10.0f;
                } else {
                    ctx->cur_val *= 10.0f;
                    ctx->cur_val += (f32)(ctx->buf[ctx->pos] - 0x30);
                }

                ctx->pos++;
            }
            break;
        case 'A' ... 'Z':
        case '_':
        case 'a' ... 'z':
            var_name_pos = 0;

            while (NuIsAlNum(ctx->buf[ctx->pos])) {
                var_name_buf[var_name_pos] = ctx->buf[ctx->pos];
                ctx->pos++;
                var_name_pos++;
            }

            var_name_buf[var_name_pos] = '\0';

            ctx->cur_tok = TOK_NUM;
            ctx->cur_val = 0.0f;

            ret = (*ctx->get_var_fn)(var_name_buf, &ctx->cur_val, NULL);

            return;
    }
}

static f32 prim_rdpf(RDPFCTXT *ctx) {
    f32 expr;

    switch (ctx->cur_tok) {
        case TOK_NUM:
        case TOK_VAR:
            get_tok(ctx);

            return ctx->cur_val;
        case TOK_SUB:
            get_tok(ctx);

            return -prim_rdpf(ctx);
        case TOK_LPAREN:
            get_tok(ctx);
            expr = f_expr(ctx);
            get_tok(ctx);

            return expr;
        case TOK_END:
            return 1.0f;
        default:
            return 0.0f;
    }
}

static f32 term_rdpf(RDPFCTXT *ctx) {
    f32 term = prim_rdpf(ctx);

    while (true) {
        switch (ctx->cur_tok) {
            case TOK_MUL:
                get_tok(ctx);

                term *= prim_rdpf(ctx);
                break;
            case TOK_DIV:
                get_tok(ctx);

                term /= prim_rdpf(ctx);
                break;
            default:
                return term;
        }
    }
}

static f32 f_expr(RDPFCTXT *ctx) {
    f32 expr = term_rdpf(ctx);

    while (true) {
        switch (ctx->cur_tok) {
            case TOK_ADD:
                get_tok(ctx);

                expr += term_rdpf(ctx);
                break;
            case TOK_SUB:
                get_tok(ctx);

                expr -= term_rdpf(ctx);
                break;
            default:
                return expr;
        }
    }
}

f32 NuRDPFVar(char *input, nurdpgetvarfn *get_var_fn) {
    RDPFCTXT ctx;
    f32 ret;

    ctx.buf = input;
    ctx.pos = 0;
    ctx.get_var_fn = get_var_fn;

    get_tok(&ctx);

    ret = f_expr(&ctx);

    return ret;
}

f32 NuRDPF(char *input) {
    return NuRDPFVar(input, NULL);
}

i32 NuStrCpyWC(char *dst, const char *src, const char *replacement) {
    i32 copied = 0;
    if (src == NULL) {
        *dst = '\0';
    } else {
        do {
            if (*src != '*') {
                *dst = *src;
                ++dst;
                ++copied;
            } else {
                const char *cursor;
                if ((cursor = replacement) != NULL) {
                    while (cursor != NULL && *cursor != '\0') {
                        *dst = *cursor;
                        ++dst;
                        ++cursor;
                        ++copied;
                    }
                }
            }
        } while (*src++ != '\0');
    }
    return copied;
}

i32 NuStrICmpWC(char *pattern, char *text, char *wildcard) {
    i32 index;
    char a, b;
    i32 suffix_length, text_length, wildcard_length;
    if (pattern == NULL)
        return -1;
    if (text == NULL)
        return 1;
    while (*pattern != '\0' && *pattern != '*') {
        a = NuToUpper((u8)*pattern);
        b = NuToUpper((u8)*text);
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        ++pattern;
        ++text;
    }
    if (*pattern == '*') {
        ++pattern;
        suffix_length = NuStrLen(pattern);
        text_length = NuStrLen(text);
        wildcard_length = text_length - suffix_length;
        if (wildcard_length < 0)
            return 1;
        if (wildcard != NULL) {
            for (index = 0; index < wildcard_length; ++index)
                wildcard[index] = text[index];
            wildcard[index] = '\0';
        }
        text += text_length - suffix_length;
    } else if (wildcard != NULL) {
        *wildcard = '\0';
    }
    while (*pattern != '\0') {
        a = NuToUpper((u8)*pattern);
        b = NuToUpper((u8)*text);
        if (a > b)
            return 1;
        if (a < b)
            return -1;
        ++pattern;
        ++text;
    }
    return 0;
}

void NuStrCat(char *str, const char *ext) {
    while (*str != '\0') {
        str++;
    }

    if (ext != NULL) {
        do {
            *str = *ext;
            str++;
        } while (*(ext++) != '\0');
    }
}

i32 NuStrNCat(char *str, const char *ext, i32 n) {
    while (*str != '\0')
        ++str;
    i32 copied = 0;
    if (ext != NULL) {
        do {
            if (n == 0)
                break;
            *str = *ext;
            ++str;
            ++copied;
            --n;
        } while (*ext++ != '\0');
    }
    return copied;
}

i32 NuStrCpy(char *dst, const char *src) {
    char *dst_start = dst;

    if (src != NULL) {
        while (*src != '\0') {
            *dst = *src;
            dst++;
            src++;
        }
    }

    *dst = '\0';

    return dst - dst_start;
}

i32 NuStrNCpy(char *dst, const char *src, i32 n) {
    i32 count = 0;

    if (src == NULL) {
        *dst = '\0';
        return 0;
    }

    do {
        n--;
        if (n < 1) {
            *dst = '\0';
            count++;

            break;
        }

        *dst = *src;
        dst++;

        count++;
    } while (*(src++) != '\0');

    return count - 1;
}

char *NuStrChr(char *src, char c) {
    while (*src != '\0') {
        if (*src == c) {
            return src;
        }

        src++;
    }

    return NULL;
}

char *NuStrRChr(char *src, char c) {
    char *ptr = src;

    while (*ptr != '\0') {
        ptr++;
    }

    while (ptr >= src) {
        if (*ptr == c) {
            return ptr;
        }

        ptr--;
    }

    return NULL;
}

i32 NuStrLen(const char *str) {
    i32 count = 0;

    while (*str != '\0') {
        count++;
        str++;
    }

    return count;
}

i32 NuStrLenU(NUWCHAR8 *str) {
    i32 offset = 0;
    i32 characters = 0;
    while (str[offset] != 0) {
        ++characters;
        ++offset;
        while (str[offset] >= 0x80 && str[offset] <= 0xbf)
            ++offset;
    }
    return characters;
}

i32 NuStrFindPosU(NUWCHAR8 *str, i32 character_index) {
    i32 offset = 0;
    i32 characters = 0;
    if (character_index == 0)
        return 0;
    while (str[offset] != 0) {
        ++characters;
        ++offset;
        while (str[offset] >= 0x80 && str[offset] <= 0xbf)
            ++offset;
        if (character_index == characters)
            break;
    }
    return offset;
}

#ifdef __ANDROID__
extern "C" {
// The original library links its own byte-wise strstr over bionic's.
extern "C" char *strstr(const char *haystack, const char *needle) {
    while (*haystack != '\0') {
        const char *haystack_cursor = haystack;
        const char *needle_cursor = needle;
        while (*needle_cursor != '\0') {
            if (*haystack_cursor == '\0' || *haystack_cursor != *needle_cursor) {
                break;
            }
            haystack_cursor++;
            needle_cursor++;
        }
        if (*needle_cursor == '\0') {
            return const_cast<char *>(haystack);
        }
        haystack++;
    }
    return NULL;
}

}
#endif

char *NuStrIStr(char *str, char *sub) {
    char *str_cursor;
    char *sub_cursor;

    while (*str != '\0') {
        str_cursor = str;
        sub_cursor = sub;

        while (*sub_cursor != '\0') {
            if (*str_cursor == '\0') {
                break;
            }

            if (NuToUpper(*str_cursor) != NuToUpper(*sub_cursor)) {
                break;
            }

            str_cursor++;
            sub_cursor++;
        }

        if (*sub_cursor == '\0') {
            return str;
        }

        str++;
    }

    return NULL;
}

void NuStrTrap(const char *text, const char *match) {
    if (NuStrCmp(text, match) == 0) {
        global_tr++;
    }
}

i32 NuStrCmp(const char *a, const char *b) {
    char a_cursor;
    char b_cursor;

    if (a == NULL) {
        return -1;
    }

    if (b == NULL) {
        return 1;
    }

    do {
        a_cursor = *a;
        b_cursor = *b;

        if (a_cursor > b_cursor) {
            return 1;
        }

        if (a_cursor < b_cursor) {
            return -1;
        }

        a++;
        b++;
    } while (a_cursor != '\0' && b_cursor != '\0');

    return 0;
}

i32 NuStrICmp(const char *a, const char *b) {
    char a_cursor;
    char b_cursor;

    if (a == NULL) {
        return -1;
    }

    if (b == NULL) {
        return 1;
    }

    do {
        a_cursor = NuToUpper(*a);
        b_cursor = NuToUpper(*b);

        if (a_cursor > b_cursor) {
            return 1;
        }

        if (a_cursor < b_cursor) {
            return -1;
        }

        a++;
        b++;
    } while (a_cursor != '\0' && b_cursor != '\0');

    return 0;
}

i32 NuStrNCmp(const char *a, const char *b, i32 n) {
    char a_cursor;
    char b_cursor;

    if (a == NULL) {
        return -1;
    }

    if (b == NULL) {
        return 1;
    }

    if (n == 0) {
        return 0;
    }

    if (n == -1) {
        n = NuStrLen(a);
    } else if (n == -2) {
        n = NuStrLen(b);
    }

    do {
        a_cursor = *a;
        b_cursor = *b;

        if (a_cursor > b_cursor) {
            return 1;
        }

        if (a_cursor < b_cursor) {
            return -1;
        }

        a++;
        b++;
        n--;
    } while (a_cursor != '\0' && b_cursor != '\0' && n != 0);

    return 0;
}

i32 NuStrNICmp(const char *a, const char *b, i32 n) {
    char a_cursor;
    char b_cursor;

    if (a == NULL) {
        return -1;
    }

    if (b == NULL) {
        return 1;
    }

    if (n == 0) {
        return 0;
    }

    if (n == -1) {
        n = NuStrLen(a);
    } else if (n == -2) {
        n = NuStrLen(b);
    }

    do {
        a_cursor = NuToUpper(*a);
        b_cursor = NuToUpper(*b);

        if (a_cursor > b_cursor) {
            return 1;
        }

        if (a_cursor < b_cursor) {
            return -1;
        }

        a++;
        b++;
        n--;
    } while (a_cursor != '\0' && b_cursor != '\0' && n != 0);

    return 0;
}

void NuStrUpr(char *dst, const char *src) {
    for (; *src != '\0'; src++) {
        *dst = NuToUpper(*src);
        dst++;
    }

    *dst = *src;
}

void NuStrLwr(char *dst, const char *src) {
    while (*src != '\0') {
        *dst = NuToLower(*src);
        ++dst;
        ++src;
    }
    *dst = *src;
}

i32 NuHexStringToI(char *string) {
    i32 value = 0;

    for (; *string != '\0'; string++) {
        value <<= 4;
        i32 upper = (i32)NuToUpper(*string);

        if (upper <= '9' && upper >= '0') {
            value |= upper - 0x30;
        } else if (upper <= 'F' && upper >= 'A') {
            value |= upper - 0x37;
        } else {
            return 0;
        }
    }

    return value;
}

NUWCHAR NuToUpperW(NUWCHAR c) {
    if (c >= 0x61 && c <= 0x7a) {
        c -= 0x20;
    } else if (c > 0xdf && c < 0x100) {
        c -= 0x20;
    }

    return c;
}

NUWCHAR NuToLowerW(NUWCHAR c) {
    if (c >= 0x41 && c <= 0x5a) {
        c += 0x20;
    } else if (c > 0xbf && c < 0xe0) {
        c += 0x20;
    }

    return c;
}

void NuStrCatW(NUWCHAR *str, const NUWCHAR *ext) {
    while (*str != 0)
        ++str;
    if (ext != NULL) {
        do {
            *str = *ext;
            ++str;
        } while (*ext++ != 0);
    }
}

i32 NuStrNCatW(NUWCHAR *str, const NUWCHAR *ext, i32 n) {
    while (*str != 0)
        ++str;
    i32 copied = 0;
    if (ext != NULL) {
        do {
            if (n == 0)
                break;
            *str = *ext;
            ++str;
            ++copied;
            --n;
        } while (*ext++ != 0);
    }
    return copied;
}

i32 NuStrCpyW(NUWCHAR *dst, NUWCHAR *src) {
    i32 len;

    len = 0;

    if (src == NULL) {
        *dst = 0;
    } else {
        do {
            *dst = *src;
            dst++;
            len++;
        } while (*src++ != 0);
    }

    return len;
}

i32 NuStrNCpyW(NUWCHAR *dst, const NUWCHAR *src, i32 n) {
    i32 copied = 0;
    if (src == NULL) {
        *dst = 0;
        return 0;
    }
    do {
        --n;
        if (n <= 0) {
            *dst = 0;
            ++copied;
            break;
        }
        *dst = *src;
        ++dst;
        ++copied;
    } while (*src++ != 0);
    return copied - 1;
}

NUWCHAR *NuStrChrW(NUWCHAR *str, NUWCHAR c) {
    while (*str != 0) {
        if (*str == c)
            return str;
        ++str;
    }
    return NULL;
}

NUWCHAR *NuStrRChrW(NUWCHAR *str, NUWCHAR c) {
    NUWCHAR *cursor = str;
    while (*cursor != 0)
        ++cursor;
    while (cursor >= str) {
        if (*cursor == c)
            return cursor;
        --cursor;
    }
    return NULL;
}

i32 NuStrLenW(const NUWCHAR *str) {
    i32 count = 0;

    while (*str != 0) {
        count++;
        str++;
    }

    return count;
}

NUWCHAR *NuStrStrW(NUWCHAR *str, const NUWCHAR *sub) {
    while (*str != 0) {
        NUWCHAR *cursor = str;
        const NUWCHAR *match = sub;
        while (*match != 0) {
            if (*cursor == 0)
                break;
            if (*cursor != *match)
                break;
            ++cursor;
            ++match;
        }
        if (*match == 0)
            return str;
        ++str;
    }
    return NULL;
}

NUWCHAR *NuStrIStrW(NUWCHAR *str, const NUWCHAR *sub) {
    while (*str != 0) {
        NUWCHAR *cursor = str;
        const NUWCHAR *match = sub;
        while (*match != 0) {
            if (*cursor == 0)
                break;
            if (NuToUpperW(*cursor) != NuToUpperW(*match))
                break;
            ++cursor;
            ++match;
        }
        if (*match == 0)
            return str;
        ++str;
    }
    return NULL;
}

i32 NuStrCmpW(const NUWCHAR *a, const NUWCHAR *b) {
    NUWCHAR ca;
    NUWCHAR cb;
    if (a == NULL)
        return -1;
    if (b == NULL)
        return 1;
    do {
        ca = *a;
        cb = *b;
        if (ca > cb)
            return 1;
        if (ca < cb)
            return -1;
        ++a;
        ++b;
    } while (ca != 0 && cb != 0);
    return 0;
}

i32 NuStrICmpW(NUWCHAR *a, NUWCHAR *b) {
    NUWCHAR a_cursor;
    NUWCHAR b_cursor;

    if (a == NULL) {
        return -1;
    }

    if (b == NULL) {
        return 1;
    }

    do {
        a_cursor = NuToUpperW(*a);
        b_cursor = NuToUpperW(*b);

        if (a_cursor > b_cursor) {
            return 1;
        }

        if (a_cursor < b_cursor) {
            return -1;
        }

        a++;
        b++;
    } while (a_cursor != '\0' && b_cursor != '\0');

    return 0;
}

i32 NuStrNCmpW(const NUWCHAR *a, const NUWCHAR *b, i32 n) {
    NUWCHAR ca, cb;
    if (a == NULL)
        return -1;
    if (b == NULL)
        return 1;
    if (n == 0)
        return 0;
    if (n == -1)
        n = NuStrLenW(a);
    else if (n == -2)
        n = NuStrLenW(b);
    do {
        ca = *a;
        cb = *b;
        if (ca > cb)
            return 1;
        if (ca < cb)
            return -1;
        ++a;
        ++b;
        --n;
    } while (ca != 0 && cb != 0 && n != 0);
    return 0;
}

i32 NuStrNICmpW(const NUWCHAR *a, const NUWCHAR *b, i32 n) {
    NUWCHAR ca, cb;
    if (a == NULL)
        return -1;
    if (b == NULL)
        return 1;
    if (n == 0)
        return 0;
    if (n == -1)
        n = NuStrLenW(a);
    else if (n == -2)
        n = NuStrLenW(b);
    do {
        ca = NuToUpperW(*a);
        cb = NuToUpperW(*b);
        if (ca > cb)
            return 1;
        if (ca < cb)
            return -1;
        ++a;
        ++b;
        --n;
    } while (ca != 0 && cb != 0 && n != 0);
    return 0;
}

void NuStrUprW(NUWCHAR *dst, const NUWCHAR *src) {
    while (*src != 0) {
        *dst = NuToUpperW(*src);
        ++dst;
        ++src;
    }
    *dst = *src;
}

void NuStrLwrW(NUWCHAR *dst, const NUWCHAR *src) {
    while (*src != 0) {
        *dst = NuToLowerW(*src);
        ++dst;
        ++src;
    }
    *dst = *src;
}

i32 NuVSPrintf(char *dest, const char *format, va_list args) {
    return vsprintf(dest, format, args);
}

#ifdef __ANDROID__
extern "C" {
// The original library supplies its own sprintf (NuSPrintf), which formats
// through vsprintf and returns the resulting string length. Native hosts keep
// their C library's sprintf.
extern "C" int sprintf(char *dest, const char *format, ...) {
    va_list args;
    va_start(args, format);
    vsprintf(dest, format, args);
    va_end(args);
    return NuStrLen(dest);
}

}
#endif

i32 NuSPrintfW(NUWCHAR *dest, NUWCHAR *format, ...) {
    // The original initializes this local but never reads it.
    i32 length = 0;
    va_list args;
    char ascii_format[256];
    char text[256];
    NuUnicodeToAscii(ascii_format, format);
    va_start(args, format);
    vsprintf(text, ascii_format, args);
    va_end(args);
    NuAsciiToUnicode(dest, text);
    return NuStrLenW(dest);
}

i32 NuStringTok(char *str, ...) {
    char *token;
    i32 value;
    va_list args;
    va_start(args, str);
    do {
        token = va_arg(args, char *);
        value = va_arg(args, i32);
        if (token != NULL) {
            if (NuStrICmp(str, token) == 0)
                break;
        }
    } while (token != NULL);
    va_end(args);
    return value;
}

i32 NuIsAlNum(char c) {
    switch (c) {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '0' ... '9':
        case '_':
            return 1;
            break;
        default:
            return 0;
    }
}

i32 NuIsAl(char c) {
    switch (c) {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
            return 1;
        default:
            return 0;
    }
}

char *NuIToA(i32 value, char *buffer, i32 radix) {
    char reversed[33];
    char *digit = reversed;
    char *out = buffer;
    if (value < 0) {
        value = -value;
        *out++ = '-';
    }
    do {
        *digit++ = value % radix + '0';
        value /= radix;
    } while (value != 0);
    while (digit != reversed)
        *out++ = *--digit;
    *out++ = '\0';
    return buffer;
}

f32 NuAToF(char *string) {
    f32 dividend = 0.0f;
    f32 divisor = 1.0f;

    char c = *string;
    string++;

    if (c == '-') {
        divisor = -1.0f;

        c = *string;
        string++;
    }

    while (c >= '0' && c <= '9') {
        dividend *= 10.0f;
        dividend += (f32)(c - 0x30);

        c = *string;
        string++;
    }

    if (c == '.') {
        c = *string;
        string++;

        while (c >= '0' && c <= '9') {
            divisor *= 10.0f;
            dividend *= 10.0f;
            dividend += (f32)(c - 0x30);

            c = *string;
            string++;
        }
    }

    return dividend / divisor;
}

i32 NuAToI(char *string) {
    i32 value = 0;
    i32 sign = 0;

    char c = *string;
    string++;

    if (c == '-') {
        sign = -1;

        c = *string;
        string++;
    }

    while (c >= '0' && c <= '9') {
        value = (value << 3) + 2 * value;
        value = value + (i32)c - 0x30;

        c = *string;
        string++;
    }

    if (sign != 0) {
        return value * sign;
    }

    return value;
}

i32 NuStrToL(char *str, char **end, i32 radix) {
    i32 value = 0;
    i32 sign = 0;
    char c = *str++;
    while (c == ' ' || c == '\t')
        c = *str++;
    if (c == '-') {
        sign = -1;
        c = *str++;
    } else if (c == '+')
        c = *str++;
    if (radix == 0) {
        radix = 10;
        if (c == '0' && (*str == 'x' || *str == 'X')) {
            radix = 16;
            ++str;
            c = *str;
        }
    }
    while (1) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= radix)
            break;
        value *= radix;
        value += c;
        c = *str++;
    }
    if (end != NULL)
        *end = str - 1;
    return sign != 0 ? value * sign : value;
}

i32 NuIsAlW(NUWCHAR c) {
    switch (c) {
        case 'A' ... 'Z':
        case 'a' ... 'z':
        case '_':
            return 1;
        default:
            return 0;
    }
}

NUWCHAR *NuIToAW(i32 value, NUWCHAR *buffer, i32 radix) {
    NUWCHAR reversed[33];
    NUWCHAR *digit = reversed;
    NUWCHAR *out = buffer;
    if (value < 0) {
        value = -value;
        *out++ = '-';
    }
    do {
        *digit++ = value % radix + '0';
        value /= radix;
    } while (value != 0);
    while (digit != reversed)
        *out++ = *--digit;
    *out++ = 0;
    return buffer;
}

f32 NuAToFW(const NUWCHAR *str) {
    f32 value = 0.0f;
    f32 divisor = 1.0f;
    NUWCHAR c = *str++;
    if (c == '-') {
        divisor = -1.0f;
        c = *str++;
    }
    while (c >= '0' && c <= '9') {
        value *= 10.0f;
        value += c - '0';
        c = *str++;
    }
    if (c == '.') {
        c = *str++;
        while (c >= '0' && c <= '9') {
            divisor *= 10.0f;
            value *= 10.0f;
            value += c - '0';
            c = *str++;
        }
    }
    return value / divisor;
}

i32 NuAToIW(const NUWCHAR *str) {
    i32 value = 0;
    i32 sign = 0;
    NUWCHAR c = *str++;
    if (c == '-') {
        sign = -1;
        c = *str++;
    }
    while (c >= '0' && c <= '9') {
        value = (value << 3) + (value << 1);
        value = value + c - '0';
        c = *str++;
    }
    return sign != 0 ? value * sign : value;
}

i32 NuStrToLW(NUWCHAR *str, NUWCHAR **end, i32 radix) {
    i32 value = 0;
    i32 sign = 0;
    NUWCHAR c = *str++;
    while (c == ' ' || c == '\t')
        c = *str++;
    if (c == '-') {
        sign = -1;
        c = *str++;
    } else if (c == '+')
        c = *str++;
    if (radix == 0) {
        radix = 10;
        if (c == '0' && (*str == 'x' || *str == 'X')) {
            radix = 16;
            ++str;
            c = *str;
        }
    }
    while (1) {
        if (c >= '0' && c <= '9')
            c -= '0';
        else if (c >= 'A' && c <= 'Z')
            c -= 'A' - 10;
        else if (c >= 'a' && c <= 'z')
            c -= 'a' - 10;
        else
            break;
        if (c >= radix)
            break;
        value *= radix;
        value += c;
        c = *str++;
    }
    if (end != NULL)
        *end = str - 1;
    return sign != 0 ? value * sign : value;
}

void NuStrToLower(char *str) {
    char *cursor = str;
    while (*cursor != 0) {
        *cursor = NuToLower(*cursor);
        ++cursor;
    }
}

u8 NuToUpper(u8 c) {
    if (c >= 'a' && c <= 'z') {
        c -= 0x20;
    } else if (c > 0xdf) {
        c -= 0x20;
    }

    return c;
}

u8 NuToLower(u8 c) {
    if (c >= 'A' && c <= 'Z') {
        c += 0x20;
    } else if (c > 0xbf && c < 0xe0) {
        c += 0x20;
    }

    return c;
}

void NuUnicodeToAscii(char *dst, NUWCHAR16 *src) {
    i32 pos;

    pos = 0;

    if (src == NULL) {
        return;
    }

    if (dst == NULL) {
        return;
    }

    *dst = '\0';

    for (; src[pos] != '\0'; pos++) {
        if ((src[pos] & 0xff00) != 0) {
            switch (src[pos]) {
                // U+2018 LEFT SINGLE QUOTATION MARK
                case 0x2018:
                    dst[pos] = 0x91;
                    break;
                // U+2019 RIGHT SINGLE QUOTATION MARK
                case 0x2019:
                    dst[pos] = 0x92;
                    break;
                // U+2013 EN DASH
                case 0x2013:
                    dst[pos] = 0x96;
                    break;
                // U+2026 HORIZONTAL ELLIPSIS
                case 0x2026:
                    dst[pos] = 0x85;
                    break;
                // U+2122 TRADE MARK SIGN
                case 0x2122:
                    dst[pos] = 0x99;
                    break;
                default:
                    dst[pos] = '?';
            }
        } else {
            dst[pos] = src[pos];
        }
    }

    dst[pos] = '\0';
}

void NuUnicodeToUTF8(NUWCHAR8 *dst, NUWCHAR16 *src) {
    if (src == NULL) {
        return;
    }

    if (dst == NULL) {
        return;
    }

    *dst = '\0';

    for (; *src != 0; src++) {
        if (*src < 0x80) {
            *dst++ = *src;
        } else if (*src < 0x800) {
            *dst++ = ((*src >> 0x6) & 0x1f) | 0xc0;
            *dst++ = (*src & 0x3f) | 0x80;
        } else {
            *dst++ = ((*src >> 0xc) & 0x1f) | 0xe0;
            *dst++ = ((*src >> 0x6) & 0x3f) | 0x80;
            *dst++ = (*src & 0x3f) | 0x80;
        }
    }

    *dst = '\0';
}

void NuAsciiToUnicode(NUWCHAR16 *dst, char *src) {
    i32 pos;

    pos = 0;
    *dst = 0;

    if (src == NULL) {
        return;
    }

    if (dst == NULL) {
        return;
    }

    for (; src[pos] != '\0'; pos++) {
        dst[pos] = (u8)src[pos];
    }

    dst[pos] = '\0';
}

void NuUTF8ToUnicode(NUWCHAR16 *dst, NUWCHAR8 *src) {
    if (src == NULL)
        return;
    if (dst == NULL)
        return;
    *dst = 0;
    while (*src != 0) {
        if (*src <= 0x7f) {
            *dst = *src;
            ++src;
        } else if (*src <= 0xdf) {
            *dst = (*src & 0x1f) << 6;
            ++src;
            *dst |= *src & 0x3f;
            ++src;
        } else if (*src <= 0xef) {
            *dst = (*src & 0x0f) << 12;
            ++src;
            *dst |= (*src & 0x3f) << 6;
            ++src;
            *dst |= *src & 0x3f;
            ++src;
        } else {
            *dst = '?';
            ++src;
        }
        ++dst;
    }
    *dst = 0;
}

NUWCHAR8 *NuUnicodeCharFromUTF8(NUWCHAR16 *dst, NUWCHAR8 *src) {
    NUWCHAR16 c[3];
    c[0] = *src++;
    if (c[0] <= 0x7f) {
        *dst = c[0];
    } else if ((c[0] & 0xe0) == 0xc0) {
        c[1] = *src++;
        *dst = ((c[0] & 0x1f) << 6) | (c[1] & 0x3f);
    } else if ((c[0] & 0xf0) == 0xe0) {
        c[1] = *src++;
        c[2] = *src++;
        *dst = (c[0] << 12) | ((c[1] & 0x3f) << 6) | (c[2] & 0x3f);
    }
    return src;
}

NUWCHAR8 *NuUTF8CharFromUnicode(NUWCHAR8 *dst, NUWCHAR16 character) {
    if (character <= 0x7f) {
        *dst++ = character;
    } else if (character <= 0x7ff) {
        *dst++ = static_cast<NUWCHAR8>(character >> 6) | 0xc0;
        *dst++ = static_cast<NUWCHAR8>(character & 0x3fu) | 0x80u;
    } else {
        *dst++ = static_cast<NUWCHAR8>(character >> 12) | 0xe0;
        *dst++ = static_cast<NUWCHAR8>((character >> 6) & 0x3fu) | 0x80u;
        *dst++ = static_cast<NUWCHAR8>(character & 0x3fu) | 0x80u;
    }
    return dst;
}

i32 GetLineW(u16 *line, i32 file) {
    i32 length = 0;
    i32 end_of_line;
    i32 has_text = 0;
    i32 quoted = 0;
    u16 character;

    for (;;) {
        character = NuFileReadWChar(file);
        switch (character) {
            case '\r':
                NuFileReadWChar(file);
                // Fall through.
            case '\n':
                if (length == 0 || has_text == 0) {
                    has_text = 0;
                    break;
                }
            case '\0':
                line[length] = 0;
                return length;

            case '"':
                quoted = 1 - quoted;
                has_text = 1;
                line[length++] = character;
                break;

            case ';':
                if (quoted == 0) {
                    if (length == 0 || has_text == 0) {
                        end_of_line = 0;
                        do {
                            character = NuFileReadWChar(file);
                            switch (character) {
                                case '\r':
                                    NuFileReadWChar(file);
                                case '\n':
                                case '\0':
                                    end_of_line = 1;
                                    break;
                                default:
                                    break;
                            }
                        } while (end_of_line == 0);
                        length = 0;
                        has_text = 0;
                        break;
                    }

                    do {
                        character = NuFileReadWChar(file);
                        switch (character) {
                            case '\r':
                                NuFileReadWChar(file);
                            case '\n':
                            case '\0':
                                line[length] = 0;
                                return length;
                            default:
                                break;
                        }
                    } while (true);
                }
            default:
                has_text = 1;
            case ' ':
            case '\t':
                if (has_text != 0)
                    line[length++] = character;
                break;
        }
    }
}

void NuStringTableSetFormat(i32 format) {
    nustring_format = format;
}

i32 NuStringTableGetFormat(void) {
    return nustring_format;
}

i32 NuStringTableLoadCSV(char *filepath, VARIPTR *buf, VARIPTR buf_end, char *label, char *language) {
    i32 parse_buf_size;
    NUSTRINGBANK *bank;
    NUWCHAR label_utf16[0x40];
    NUWCHAR lang_utf16[0x40];
    void *mem_buf;
    i32 bytes_read;
    char *word_buf;
    char *line_buf;
    char *tmp_buf;
    NUFILE mem_file;
    NUFPAR *parser;
    NUSTRING *strings;
    i32 column;
    i32 label_column;
    i32 lang_column;
    i32 len;
    i32 i;
    i32 max_strings;

    label_column = -1;
    lang_column = -1;
    i = 0;
    max_strings = 0;
    word_buf = NULL;
    line_buf = NULL;
    tmp_buf = NULL;
    parse_buf_size = 0x8000;
    strings = NULL;
    bank = &StringBank[CurrentStringBank];

    NuAsciiToUnicode(label_utf16, label);
    NuAsciiToUnicode(lang_utf16, language);

    bank->strings = NULL;
    bank->max_strings = 0;
    bank->string_count = 0;

    mem_buf = (void *)(buf_end.addr - 0x100000);

    bytes_read = NuFileLoadBuffer(filepath, mem_buf, 0x100000);

    word_buf = (char *)((usize)mem_buf + bytes_read);
    line_buf = (char *)((usize)mem_buf + bytes_read + parse_buf_size);
    tmp_buf = (char *)((usize)mem_buf + bytes_read + parse_buf_size + parse_buf_size);

    mem_file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);

    if (mem_file != 0) {
        parser = NuFParOpen(mem_file);

        if (parser != NULL) {
            parser->word_buf = word_buf;
            parser->line_buf = line_buf;

            parser->line_buf_size = parse_buf_size;
            parser->word_buf_size = parse_buf_size;

            while (NuFParGetLine(parser) != 0) {
                max_strings++;
            }

            NuFParClose(parser);
        }

        NuFileClose(mem_file);
    }

    strings = (NUSTRING *)ALIGN(buf->addr, 0x4);
    buf->addr = ALIGN(buf->addr, 0x4);
    buf->addr = buf->addr + max_strings * sizeof(NUSTRING);

    mem_file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);

    if (mem_file != 0) {
        parser = NuFParOpen(mem_file);

        if (parser != NULL) {
            parser->word_buf = word_buf;
            parser->line_buf = line_buf;

            parser->line_buf_size = parse_buf_size;
            parser->word_buf_size = parse_buf_size;

            while (NuFParGetLine(parser) != 0) {
                column = 0;

                while (NuFParGetWord(parser) != 0) {
                    if (NuStrICmpW((NUWCHAR *)parser->word_buf, label_utf16) == 0) {
                        label_column = column;
                    }

                    if (NuStrICmpW((NUWCHAR *)parser->word_buf, lang_utf16) == 0) {
                        lang_column = column;
                    }

                    column++;
                }

                if (label_column >= 0 && lang_column >= 0) {
                    break;
                }
            }

            if (label_column >= 0 && lang_column >= 0) {
                while (NuFParGetLine(parser) != 0) {
                    column = 0;

                    while (NuFParGetWord(parser) != 0) {
                        if (column == label_column) {
                            len = NuStrLenW((NUWCHAR *)parser->word_buf) + 1;

                            strings[i].id = buf->char_ptr;
                            buf->addr += len * sizeof(char);

                            NuUnicodeToAscii(strings[i].id, (NUWCHAR16 *)parser->word_buf);
                        }

                        if (column == lang_column) {
                            if (nustring_format == 1) {
                                len = NuStrLenW((NUWCHAR *)parser->word_buf) + 1;

                                strings[i].str_utf16 = (NUWCHAR *)ALIGN(buf->addr, 0x2);
                                buf->addr = ALIGN(buf->addr, 0x2);

                                // Using `sizeof(NUWCHAR)` here causees this not
                                // to match.
                                buf->addr += len * 2;

                                NuStrCpyW(strings[i].str_utf16, (NUWCHAR *)parser->word_buf);
                            } else {
                                NuUnicodeToUTF8((NUWCHAR8 *)tmp_buf, (NUWCHAR16 *)parser->word_buf);

                                // We can only do this after conversion, as the
                                // size in bytes of a UTF-8 string can't be
                                // easily predicted from its UTF-16 equivalent.
                                len = NuStrLen(tmp_buf) + 1;

                                // There's no need here to align to 2 bytes, but
                                // it's done anyhow.
                                strings[i].str_utf8 = (NUWCHAR8 *)ALIGN(buf->addr, 0x2);
                                buf->addr = ALIGN(buf->addr, 0x2);
                                buf->addr += len * sizeof(NUWCHAR8);

                                NuStrCpy((char *)strings[i].str_utf8, tmp_buf);
                            }
                        }

                        column++;

                        if (column > label_column && column > lang_column) {
                            if (strings[i].id != NULL && strings[i].str_utf16 != NULL) {
                                i++;
                            } else {
                                strings[i].id = NULL;
                                strings[i].str_utf16 = NULL;
                            }

                            break;
                        }
                    }
                }
            }

            NuFParClose(parser);

            bank->strings = strings;
            bank->string_count = i;
            bank->max_strings = max_strings;
        }

        NuFileClose(mem_file);
    }

    return bank->string_count;
}

i32 NuStringTableLoadTXT(char *filepath, VARIPTR *buf, VARIPTR buf_end) {
    i32 i;
    i32 max_strings;
    NUSTRINGBANK *bank = &StringBank[CurrentStringBank];
    NUSTRING *strings = NULL;
    char *tmp_buf = NULL;
    void *mem_buf;
    i32 bytes_read;
    NUFILE mem_file;
    NUFPAR *parser;
    i32 len;

    bank->strings = NULL;
    bank->max_strings = 0;
    bank->string_count = 0;
    i = max_strings = 0;
    mem_buf = (void *)(buf_end.addr - 0x100000);
    bytes_read = NuFileLoadBuffer(filepath, mem_buf, 0x100000);
    tmp_buf = (char *)((usize)mem_buf + bytes_read);
    mem_file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);
    if (mem_file != 0) {
        parser = NuFParOpen(mem_file);
        if (parser != NULL) {
            while (NuFParGetLine(parser) != 0) {
                max_strings++;
            }
            NuFParClose(parser);
        }
        NuFileClose(mem_file);
    }

    strings = (NUSTRING *)ALIGN(buf->addr, 0x4);
    buf->addr = ALIGN(buf->addr, 0x4);
    buf->addr += max_strings * sizeof(NUSTRING);
    mem_file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);
    if (mem_file != 0) {
        parser = NuFParOpen(mem_file);
        if (parser != NULL) {
            while (NuFParGetLine(parser) != 0) {
                NuFParGetWord(parser);
                NuFParGetWord(parser);
                len = NuStrLenW((NUWCHAR *)parser->word_buf) + 1;
                strings[i].id = buf->char_ptr;
                buf->addr += len;
                NuUnicodeToAscii(strings[i].id, (NUWCHAR16 *)parser->word_buf);
                NuFParGetWord(parser);
                len = NuStrLenW((NUWCHAR *)parser->word_buf) + 1;
                strings[i].str_utf16 = (NUWCHAR *)ALIGN(buf->addr, 0x2);
                buf->addr = ALIGN(buf->addr, 0x2);
                buf->addr += len * 2;
                NuStrCpyW(strings[i].str_utf16, (NUWCHAR *)parser->word_buf);

                // The original reserves and copies UTF-16 before storing the
                // selected output encoding, including when it is UTF-16 again.
                if (nustring_format == 1) {
                    len = NuStrLenW((NUWCHAR *)parser->word_buf) + 1;
                    strings[i].str_utf16 = (NUWCHAR *)ALIGN(buf->addr, 0x2);
                    buf->addr = ALIGN(buf->addr, 0x2);
                    buf->addr += len * 2;
                    NuStrCpyW(strings[i].str_utf16, (NUWCHAR *)parser->word_buf);
                } else {
                    NuUnicodeToUTF8((NUWCHAR8 *)tmp_buf, (NUWCHAR16 *)parser->word_buf);
                    len = NuStrLen(tmp_buf) + 1;
                    strings[i].str_utf8 = (NUWCHAR8 *)ALIGN(buf->addr, 0x2);
                    buf->addr = ALIGN(buf->addr, 0x2);
                    buf->addr += len;
                    NuStrCpy((char *)strings[i].str_utf8, tmp_buf);
                }
                i++;
            }
            NuFParClose(parser);
            bank->strings = strings;
            bank->string_count = i;
            bank->max_strings = max_strings;
        }
        NuFileClose(mem_file);
    }
    return bank->string_count;
}

void NuStringTableSetBank(i32 bank) {
    if (bank >= 0 && bank < 3) {
        CurrentStringBank = bank;
    }
}

i32 NuStringTableLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end) {
    i32 is_csv = 0;
    char *extension;
    if ((extension = NuStrRChr(filepath, '.')) != NULL) {
        if (NuStrICmp(extension, ".csv") == 0) {
            is_csv = 1;
        }
    }
    if (is_csv) {
        return NuStringTableLoadCSV(filepath, buf, buf_end, "LABEL", "ENGLISH");
    } else {
        return NuStringTableLoadTXT(filepath, buf, buf_end);
    }
}

void NuStringTableUnload(void) {
    StringBank[CurrentStringBank].strings = NULL;
    StringBank[CurrentStringBank].max_strings = 0;
    StringBank[CurrentStringBank].string_count = 0;
}

i32 NuStringTableGetIdByName(char *name) {
    i32 j;
    i32 i;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < StringBank[i].string_count; j++) {
            if (StringBank[i].strings[j].id != NULL && StringBank[i].strings[j].str_utf16 != NULL) {
                if (NuStrICmp(StringBank[i].strings[j].id, name) == 0) {
                    return j + (i << 24);
                }
            }
        }
    }

    return -1;
}

NUWCHAR *NuStringTableGetById(i32 id) {
    u32 bank = 0;

    if ((u32)id > 0xffffff && id != -1) {
        bank = (u32)id >> 24;
        id &= 0xffffff;
    }
    if (bank > 2) {
        return NULL;
    }
    if (id < 0) {
        return NULL;
    }
    if (id >= StringBank[bank].string_count) {
        return NULL;
    }
    return StringBank[bank].strings[id].str_utf16;
}

NUWCHAR *NuStringTableGetByName(char *name) {
    i32 j;
    i32 i;

    for (i = 0; i < 3; i++) {
        for (j = 0; j < StringBank[i].string_count; j++) {
            if (StringBank[i].strings[j].id != NULL && StringBank[i].strings[j].str_utf16 != NULL) {
                if (NuStrICmp(StringBank[i].strings[j].id, name) == 0) {
                    return StringBank[i].strings[j].str_utf16;
                }
            }
        }
    }

    // This is a little horrifying, as `NuBlankString` appears to be declared as
    // a string literal. This works out largely because of alignment
    // requirements.
    return (NUWCHAR *)NuBlankString;
}

void NuStringTableSaveCharacterList(char *filepath, char *scratch, i32 report_missing) {
    i32 i;
    i32 j;
    i32 count = 0;
    i32 overflow = 0;
    i32 missing_count = 0;
    char *used = NULL;
    NUWCHAR character = 0;
    i32 bank;
    NUFILE file;
    NUWCHAR bom;
    NUWCHAR characters[512];
    NUWCHAR missing[512];
    char message[512];

    used = scratch;
    memset(used, 0, 0x1fff);
    memset(characters, 0, 0x400);
    memset(message, 0, 0x200);
    bank = CurrentStringBank;
    for (i = 0; i < StringBank[bank].string_count; i++) {
        if (StringBank[bank].strings[i].str_utf16 != NULL) {
            for (j = 0; StringBank[bank].strings[i].str_utf16[j] != 0; j++) {
                character = StringBank[bank].strings[i].str_utf16[j];
                used[character >> 3] |= 1 << (character & 7);
            }
        }
    }
    for (character = 0; character != 0xffff; character++) {
        if ((used[character >> 3] >> (character & 7)) & 1) {
            characters[count] = character;
            count++;
            if (count > 511) {
                overflow = 1;
                break;
            }
        }
    }
    file = NuFileOpen(filepath, NUFILE_WRITE);
    if (file != 0) {
        bom = 0xfeff;
        NuFileWrite(file, &bom, 2);
        NuFileWrite(file, characters, NuStrLenW(characters) * 2);
        if (overflow) {
            NuAsciiToUnicode(characters, " [over 512 unique chars - too many!]");
            NuFileWrite(file, characters, count * 2);
        } else {
            sprintf(message, " [%d unique chars] ", count);
            NuAsciiToUnicode(characters, message);
            NuFileWrite(file, characters, NuStrLen(message) * 2);
        }
        // The original retains this reporting branch, but never populates
        // missing or increments missing_count.
        if (report_missing) {
            if (missing_count == 0) {
                NuStrCpy(message, " [no missing chars]");
            } else {
                NuFileWrite(file, missing, missing_count * 2);
                sprintf(message, " [%d missing chars]", missing_count);
            }
            NuAsciiToUnicode(characters, message);
            NuFileWrite(file, characters, NuStrLen(message) * 2);
        }
        NuFileClose(file);
    }
}

void NuStrFixExtPlatform(char *dst, char *src, char *ext, i32 dst_size, char *platform_string) {
    LOG_DEBUG("dst=%p, src=%s, ext=%s, dst_size=%d, platform_string=%s", dst, src, ext, dst_size,
              platform_string != NULL ? platform_string : "NULL");

    char *period;
    char *sep;
    char *win_sep;

    if (src == NULL || ext == NULL || dst == NULL) {
        return;
    }

    NuStrCpy(dst, src);

    period = NuStrRChr(dst, '.');

    sep = NuStrRChr(dst, '/');
    win_sep = NuStrRChr(dst, '\\');
    if (win_sep > sep) {
        sep = win_sep;
    }

    if (sep >= period) {
        NuStrCat(dst, "_");

        if (platform_string != NULL) {
            NuStrCat(dst, platform_string);
        }

        NuStrCat(dst, ".");
        NuStrCat(dst, ext);
    } else {
        *period = '\0';
        NuStrCat(period, "_");

        if (platform_string != NULL) {
            NuStrCat(period, platform_string);
        }

        NuStrCat(period, ".");
        NuStrCat(period, ext);
    }

    if (NuStrLen(dst) == dst_size - 1) {
        // Nothing here. Presumably some gated code in the original.
    }
}

void NuStrFixExt(char *dst, char *src, char *ext, i32 dst_size) {
    if (src == NULL || ext == NULL || dst == NULL)
        return;
    NuStrFixExtPlatform(dst, src, ext, dst_size, g_platformName);
    return;
}

void NuStrGetFilenameNoExt(char *dst, char *src) {
    char *extension = NuStrRChr(src, '.');
    if (extension == NULL)
        extension = src + (NuStrLen(src) - 1);
    char *separator = NuStrRChr(src, '/');
    char *backslash = NuStrRChr(src, '\\');
    if (backslash > separator)
        separator = backslash;
    if (separator == NULL)
        separator = src;
    // Preserve the original's unconditional advance, even for a bare filename.
    ++separator;
    i32 length = extension - separator + 1;
    NuStrNCpy(dst, separator, length);
}

void NuStrGetPath(char *dst, char *src) {
    char *separator = NuStrRChr(src, '/');
    char *backslash = NuStrRChr(src, '\\');
    if (backslash > separator)
        separator = backslash;
    if (separator == NULL) {
        *dst = 0;
    } else {
        i32 length = separator - src + 1;
        NuStrNCpy(dst, src, length);
    }
}

void NuStrGetExt(char *dst, char *src) {
    char *separator;
    char *extension = NuStrRChr(src, '.');
    separator = NuStrRChr(src, '/');
    char *backslash = NuStrRChr(src, '\\');
    if (backslash > separator)
        separator = backslash;
    if (extension == NULL || separator > extension) {
        *dst = 0;
    } else {
        char *end = src + NuStrLen(src);
        i32 length = end - extension;
        if (length == 0)
            *dst = 0;
        else
            NuStrNCpy(dst, extension + 1, length);
    }
}

void NuStrSubstituteString(char *dst, char *src, char *search, char *replacement) {
    i32 matched = 0;
    while (*src != 0) {
        while (search[matched] != 0) {
            if (NuToLower(src[matched]) != NuToLower(search[matched])) {
                matched = 0;
                break;
            }
            ++matched;
        }
        if (matched != 0) {
            src += matched;
            if (replacement != NULL) {
                matched = 0;
                while (replacement[matched] != 0) {
                    *dst = replacement[matched];
                    ++dst;
                    ++matched;
                }
            }
            matched = 0;
        } else {
            *dst = *src;
            ++dst;
            ++src;
        }
    }
    *dst = 0;
}

void NuStringFilterLoad(char *path, VARIPTR *buf, VARIPTR buf_end) {
    i32 count;
    i32 i;
    i32 length;
    i32 j;
    void *mem_buf = (void *)(buf_end.addr - 0x100000);
    i32 bytes_read = NuFileLoadBuffer(path, mem_buf, 0x100000);
    NUFILE file;
    NUFPAR *parser;
    NUWCHAR word[128];

    count = 0;
    file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);
    if (file != 0) {
        parser = NuFParOpen(file);
        if (parser != NULL) {
            do {
                if (NuFParGetWord(parser) > 0)
                    count++;
            } while (NuFParGetLine(parser) != 0);
            NuFParClose(parser);
        }
        NuFileClose(file);
    }
    BadWords = (NUWCHAR **)ALIGN(buf->addr, 4);
    buf->addr = ALIGN(buf->addr, 4);
    buf->addr += (count + 1) * sizeof(NUWCHAR *);
    memset(BadWords, 0, (count + 1) * sizeof(NUWCHAR *));
    i = 0;
    file = NuMemFileOpen(mem_buf, bytes_read, NUFILE_READ);
    if (file != 0) {
        parser = NuFParOpen(file);
        if (parser != NULL) {
            do {
                if (NuFParGetWord(parser) > 0) {
                    NuUTF8ToUnicode(word, (NUWCHAR8 *)parser->word_buf);
                    length = NuStrLenW(word);
                    if (length <= 3) {
                        for (j = length; j >= 0; j--)
                            word[j + 1] = word[j];
                        word[0] = '!';
                        length++;
                    }
                    length++;
                    BadWords[i] = (NUWCHAR *)ALIGN(buf->addr, 2);
                    buf->addr = ALIGN(buf->addr, 2);
                    buf->addr += length * 2;
                    NuStrLwrW(BadWords[i], word);
                    i++;
                }
            } while (NuFParGetLine(parser) != 0 && i < count);
            NuFParClose(parser);
        }
        NuFileClose(file);
    }
    NumBadWords = i;
}

static i32 NuStringCharEquiv(u16 a, u16 b) {
    i32 i = 0;
    if (a == b) {
        return 1;
    } else {
        while (bad_equiv[i][0] != 0) {
            if (bad_equiv[i][1] == a && bad_equiv[i][0] == b)
                return 1;
            if (bad_equiv[i][1] == b && bad_equiv[i][0] == a)
                return 1;
            i++;
        }
    }
    return 0;
}

static i32 NuStringIsFluff(u16 character) {
    i32 i = 0;
    while (bad_fluff[i] != 0) {
        if (bad_fluff[i] == character)
            return 1;
        i++;
    }
    return 0;
}

static u16 *NuStringBadSubString(const u16 *text, const u16 *word, i32 *length, i32 whole_word) {
    const u16 *cursor;
    const u16 *match;
    i32 boundary = 1;
    while (*text != 0) {
        cursor = text;
        match = word;
        if (boundary || !whole_word) {
            while (*match != 0) {
                if (*cursor == 0)
                    break;
                if (NuStringCharEquiv(*cursor, *match) == 0)
                    break;
                cursor++;
                while (*cursor != 0) {
                    if (!NuStringIsFluff(*cursor) && *cursor != *match)
                        break;
                    cursor++;
                }
                match++;
            }
            if (*match == 0) {
                if (!whole_word || cursor[1] == 0 || NuStringIsFluff(cursor[1])) {
                    *length = cursor - text;
                    return (u16 *)text;
                }
            }
        }
        if (NuStringIsFluff(*text))
            boundary = 1;
        else
            boundary = 0;
        text++;
    }
    return NULL;
}

i32 NuStringFilterBadWordsW(NUWCHAR *dest, NUWCHAR *source, NUWCHAR *replacement) {
    i32 i = 0;
    i32 changed = 0;
    i32 whole_word;
    NUWCHAR *word;
    NUWCHAR *match;
    i32 length;
    NUWCHAR lower[256];
    if (source != dest)
        NuStrCpyW(dest, source);
    NuStrLwrW(lower, dest);
    while (BadWords[i] != NULL) {
        if (BadWords[i][0] == '!') {
            word = BadWords[i] + 1;
            whole_word = 1;
        } else {
            word = BadWords[i];
            whole_word = 0;
        }
        match = NuStringBadSubString(lower, word, &length, whole_word);
        if (match != NULL) {
            NuStrCpyW(dest, replacement);
            changed = 1;
        }
        i++;
    }
    return changed;
}

i32 NuStringFilterBadWords(NUWCHAR8 *dest, NUWCHAR8 *source, NUWCHAR8 *replacement) {
    i32 changed;
    NUWCHAR text[128];
    NUWCHAR substitute[128];
    NuUTF8ToUnicode(text, source);
    NuUTF8ToUnicode(substitute, replacement);
    changed = NuStringFilterBadWordsW(text, text, substitute);
    NuUnicodeToUTF8(dest, text);
    return changed;
}

extern "C" {
extern "C" i32 NuPPGetSize(NUFILE file) {
    i32 size;
    i32 original_position;
    NUPPHEADER header;
    original_position = NuFileSeek(file, 0, NUFILE_SEEK_CURRENT);
    NuFileSeek(file, 0, NUFILE_SEEK_START);
    NuFileRead(file, &header, sizeof(header));
    if (header.magic == 0x02434e52) {
        header.unpacked_size = NUPP_SWAP32(header.unpacked_size);
        header.packed_size = NUPP_SWAP32(header.packed_size) + 6;
        size = header.unpacked_size;
    } else {
        size = NuFileOpenSize(file);
    }
    NuFileSeek(file, original_position, NUFILE_SEEK_START);
    return size;
}

}

i32 NuPPLoadBuffer(NUFILE file, void *buf, i32 buf_size) {
    i32 size;
    i32 unused = 0;
    i32 loaded;
    u8 *source;
    NUPPHEADER header;
    do {
        while (NuFileSeek(file, 0, NUFILE_SEEK_START) < 0) {
        }
        loaded = NuFileRead(file, &header, sizeof(header));
    } while (loaded < 0);
    if (header.magic == 0x02434e52) {
        header.unpacked_size = NUPP_SWAP32(header.unpacked_size);
        header.packed_size = NUPP_SWAP32(header.packed_size) + 18;
        source = static_cast<u8 *>(buf);
        source += header.unpacked_size + 256;
        source -= header.packed_size;
        do {
            while (NuFileSeek(file, 4, NUFILE_SEEK_START) < 0) {
            }
            loaded = NuFileRead(file, source, header.packed_size - 4);
        } while (loaded < 0);
        NuPPUnpack(source - 4, buf);
        size = header.unpacked_size;
    } else {
        size = NuFileOpenSize(file);
        do {
            while (NuFileSeek(file, 0, NUFILE_SEEK_START) < 0) {
            }
            loaded = NuFileRead(file, buf, size);
        } while (loaded < 0);
    }
    return size;
}

extern "C" {
extern "C" void NuPPUnpack(void *source, void *destination) {
    NUPPREG last;
    NUPPREG carry;
    NUPPREG saved_byte;
    NUPPADDR dest;
    NUPPREG temp;
    NUPPREG upper;
    NUPPREG bits;
    NUPPREG count;
    NUPPREG distance;
    NUPPREG length;
    NUPPADDR copy;
    NUPPADDR saved;
    NUPPADDR input;
    NUPPADDR input_end;
    NUPPADDR output;
    NUPPADDR output_end;
    // The original reserves an aligned scratch area, stores the unpacked size
    // at word 64, and places a zero restoration sentinel immediately before it.
    u16 saved_stack[66] __attribute__((aligned(32)));
    copy.value = 0x0;
    length.value = copy.value;
    upper.value = length.value;
    temp.value = upper.value;
    dest.value = temp.value;
    saved_byte.value = dest.value;
    carry.value = saved_byte.value;
    last.value = carry.value;
    saved.value = (uintptr_t)(saved_stack + 64);
    // NuPPLoadBuffer supplies the header starting at byte four; magic is skipped.
    source = (u8 *)source + 4;
    copy.value = (uintptr_t)source;
    dest.value = (uintptr_t)destination;
    length.value = (u8)(*(u8 *)(copy.value));
    last.value = (u8)(*(u8 *)((copy.value + 0x1)));
    temp.value = (u8)(*(u8 *)((copy.value + 0x2)));
    carry.value = (u8)(*(u8 *)((copy.value + 0x3)));
    length.value = (length.value << 0x18);
    last.value = (last.value << 0x10);
    temp.value = (temp.value << 0x8);
    length.value = (length.value | last.value);
    length.value = (length.value | temp.value);
    length.value = (length.value | carry.value);
    copy.value = (copy.value + 0x4);
    *(u32 *)(saved.value) = length.value;
    input.value = copy.value;
    input.value = (input.value + 0xa);
    output.value = dest.value;
    output_end.value = output.value;
    output_end.value = (output_end.value + length.value);
    length.value = (u8)(*(u8 *)(copy.value));
    last.value = (u8)(*(u8 *)((copy.value + 0x1)));
    temp.value = (u8)(*(u8 *)((copy.value + 0x2)));
    carry.value = (u8)(*(u8 *)((copy.value + 0x3)));
    length.value = (length.value << 0x18);
    last.value = (last.value << 0x10);
    temp.value = (temp.value << 0x8);
    length.value = (length.value | last.value);
    length.value = (length.value | temp.value);
    length.value = (length.value | carry.value);
    copy.value = (copy.value + 0x4);
    input_end.value = input.value;
    input_end.value = (input_end.value + length.value);
    saved.value = (saved.value - 0x2);
    *(u16 *)(saved.value) = 0x0;
    // RNC method 2 interleaves control bits with literal and distance bytes.
    bits.value = 0xffffff00;
    carry.value = 0x1;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto next_token;
refill_bulk_length:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto bulk_length_accumulate;
refill_match_length_bit:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_length_accumulate;
refill_match_length_extension:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_length_extension;
refill_match_length_low:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_length_low;
refill_distance_prefix:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_prefix;
refill_distance_high_bit:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_high_bit;
refill_distance_extension:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_extension;
refill_distance_low_bit:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_low_accumulate;
start_bulk_length:;
    count.value = 0x3;
    goto bulk_length_bit;
bulk_length_bit:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_bulk_length;
bulk_length_accumulate:;
    distance.value = (distance.value + distance.value);
    distance.value = (distance.value + carry.value);
    carry.value = (distance.value >> 0x10);
    carry.value = (carry.value & 0x1);
    count.value = (count.value - 0x1);
    if ((i32)(count.value) >= 0)
        goto bulk_length_bit;
    distance.value = (distance.value + 0x2);
copy_bulk_literals:;
    last.value = (u8)(*(u8 *)(input.value));
    temp.value = (u8)(*(u8 *)((input.value + 0x1)));
    *(u8 *)(output.value) = last.byte;
    output.bytes[0x1] = temp.byte;
    last.value = (u8)(*(u8 *)((input.value + 0x2)));
    temp.value = (u8)(*(u8 *)((input.value + 0x3)));
    output.bytes[0x2] = last.byte;
    output.bytes[0x3] = temp.byte;
    input.value = (input.value + 0x4);
    distance.value = (distance.value - 0x1);
    output.value = (output.value + 0x4);
    if (!((i32)(distance.value) < 0)) {
        goto copy_bulk_literals;
    } else {
        goto next_token;
    }
decode_match_length:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_match_length_bit;
match_length_accumulate:;
    count.value = (count.value + count.value);
    count.value = (count.value + carry.value);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_match_length_extension;
match_length_extension:;
    if (carry.value == 0)
        goto decode_match_distance;
    count.value = (count.value - 0x1);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_match_length_low;
match_length_low:;
    count.value = (count.value + count.value);
    count.value = (count.value + carry.value);
    carry.value = (count.value >> 0x10);
    carry.value = (carry.value & 0x1);
    last.value = (count.value - 0x9);
    if (!(last.value == 0)) {
        goto decode_match_distance;
    } else {
        goto start_bulk_length;
    }
decode_match_distance:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_distance_prefix;
distance_prefix:;
    if (carry.value == 0)
        goto read_distance_byte;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_distance_high_bit;
distance_high_bit:;
    distance.value = (distance.value + distance.value);
    distance.value = (distance.value + carry.value);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_distance_extension;
distance_extension:;
    if (carry.value != 0)
        goto distance_extended_prefix;
    if (distance.value != 0)
        goto distance_pack;
    distance.value = (distance.value + 0x1);
    goto distance_low_bit;
distance_low_bit:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_distance_low_bit;
distance_low_accumulate:;
    distance.value = (distance.value + distance.value);
    distance.value = (distance.value + carry.value);
    carry.value = (distance.value >> 0x10);
    carry.value = (carry.value & 0x1);
    goto distance_pack;
distance_pack:;
    last.value = (distance.value << 0x8);
    last.value = (last.value & 0xff00);
    distance.value = (distance.value >> 0x8);
    distance.value = (distance.value | last.value);
    goto read_distance_byte;
read_distance_byte:;
    upper.value = (distance.value & 0xff00);
    distance.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    distance.value = (distance.value | upper.value);
    copy.value = output.value;
    copy.value = (copy.value - distance.value);
    copy.value = (copy.value - 0x1);
    carry.value = (count.value & 0x1);
    count.value = (count.value >> 1);
    if (carry.value == 0)
        goto copy_pairs_setup;
    last.value = (u8)(*(u8 *)(copy.value));
    copy.value = (copy.value + 0x1);
    *(u8 *)(output.value) = last.byte;
    output.value = (output.value + 0x1);
    goto copy_pairs_setup;
copy_pairs_setup:;
    count.value = (count.value - 0x1);
    if (distance.value != 0)
        goto copy_back_reference;
    upper.value = (distance.value & 0xff00);
    distance.value = (u8)(*(u8 *)(copy.value));
    distance.value = (distance.value | upper.value);
copy_repeated_byte:;
    *(u8 *)(output.value) = distance.byte;
    count.value = (count.value - 0x1);
    output.bytes[0x1] = distance.byte;
    output.value = (output.value + 0x2);
    if (!((i32)(count.value) < 0)) {
        goto copy_repeated_byte;
    } else {
        goto next_token;
    }
copy_back_reference:;
    temp.value = (u8)(*(u8 *)((copy.value + 0x1)));
    last.value = (u8)(*(u8 *)(copy.value));
    output.bytes[0x1] = temp.byte;
    *(u8 *)(output.value) = last.byte;
    copy.value = (copy.value + 0x2);
    count.value = (count.value - 0x1);
    output.value = (output.value + 0x2);
    if (!((i32)(count.value) < 0)) {
        goto copy_back_reference;
    } else {
        goto next_token;
    }
refill_token:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    if (!(carry.value != 0)) {
        goto literal_copy;
    } else {
        goto decode_match;
    }
literal_copy:;
    last.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    *(u8 *)(output.value) = last.byte;
    output.value = (output.value + 0x1);
    goto next_token;
next_token:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    if (carry.value != 0)
        goto match_token_boundary;
    last.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    *(u8 *)(output.value) = last.byte;
    output.value = (output.value + 0x1);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    if (!(carry.value == 0)) {
        goto match_token_boundary;
    } else {
        goto literal_copy;
    }
match_token_boundary:;
    bits.value = (bits.value & 0xff);
    if (!(bits.value == 0)) {
        goto decode_match;
    } else {
        goto refill_token;
    }
decode_match:;
    count.value = 0x2;
    distance.value = 0x0;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_match_kind;
match_kind:;
    if (carry.value == 0)
        goto decode_match_length;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_short_kind;
match_short_kind:;
    if (carry.value == 0)
        goto read_distance_byte;
    count.value = (count.value + 0x1);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_long_kind;
match_long_kind:;
    if (carry.value == 0)
        goto decode_match_distance;
    count.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    if (count.value != 0)
        goto match_long_length;
    count.value = (count.value + 0x8);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (!(bits.value == 0)) {
        goto block_end;
    } else {
        goto refill_block_end;
    }
match_long_length:;
    count.value = (count.value + 0x8);
    goto decode_match_distance;
distance_extended_prefix:;
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_extended_bit;
distance_extended_bit:;
    distance.value = (distance.value + distance.value);
    distance.value = (distance.value + carry.value);
    carry.value = (distance.value >> 0x10);
    carry.value = (carry.value & 0x1);
    distance.value = (distance.value | 0x4);
    bits.value = (bits.value + bits.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    bits.value = (bits.value & 0xff);
    if (bits.value == 0)
        goto refill_extended_low;
distance_extended_low:;
    if (!(carry.value == 0)) {
        goto distance_pack;
    } else {
        goto distance_low_bit;
    }
refill_match_kind:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_kind;
refill_short_kind:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_short_kind;
refill_long_kind:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto match_long_kind;
refill_extended_bit:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_extended_bit;
refill_extended_low:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
    goto distance_extended_low;
refill_block_end:;
    bits.value = (u8)(*(u8 *)(input.value));
    input.value = (input.value + 0x1);
    bits.value = (bits.value + bits.value);
    bits.value = (bits.value + carry.value);
    carry.value = (bits.value >> 0x8);
    carry.value = (carry.value & 0x1);
block_end:;
    if (carry.value != 0)
        goto next_token;
    length.value = (u16)(*(u16 *)(saved.value));
    saved.value = (saved.value + 0x2);
    if (length.value == 0)
        goto unpack_return;
// Retained from the original, although this decoder only pushes a zero sentinel.
restore_saved_bytes:;
    saved_byte.value = (u16)(*(u16 *)(saved.value));
    saved.value = (saved.value + 0x2);
    *(u8 *)(output.value) = saved_byte.byte;
    output.value = (output.value + 0x1);
    length.value = (length.value - 0x1);
    if (!(length.value == 0)) {
        goto restore_saved_bytes;
    } else {
        goto unpack_return;
    }
unpack_return:;
    return;
}

}

void NuTimeWait(f32 milliseconds) {
    f32 elapsed = 0.0f;
    NUTIME start;
    NUTIME current;
    NUTIME difference;

    NuTimeGet(&start);
    while (elapsed < milliseconds) {
        NuTimeGet(&current);
        NuTimeSub(&difference, &current, &start);
        elapsed = NuTimeMilliSeconds(&difference);
    }
}

extern "C" {
extern "C" void NuTimeStartFrame(void) {
    frameStartTime = NuTimeGetTime();
}

extern "C" u32 NuTimeGetStartFrame(void) {
    return frameStartTime;
}

extern "C" u32 NuTimeGetSinceStartFrame(void) {
    u32 now = NuTimeGetTime();
    return now - frameStartTime;
}

}

f32 NuTimeGetFrameTime(void) {
    return nuapi.frametime;
}

void NuTimeForceFrameTime(f32 frame_time) {
    nuapi.forced_frame_time = frame_time;
}

void NuTimeGet(NUTIME *t) {
    NuTimeGetTicksPS(&t->low, (u32 *)&t->high);
}

void NuTimeSub(NUTIME *t, NUTIME *a, NUTIME *b) {
    if (a->low >= b->low) {
        t->low = a->low - b->low;
        t->high = a->high - b->high;
    } else {
        t->low = a->low - b->low;
        t->high = a->high - 1 - b->high;
    }
}

f32 NuTimeSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 seconds;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    seconds = ticks / ticks_per_second;

    return seconds;
}

f32 NuTimeMicroSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 micros;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    micros = ticks / ticks_per_second * 1000000.0;

    return micros;
}

f32 NuTimeMilliSeconds(NUTIME *t) {
    u32 low;
    u32 high;
    f64 ticks;
    f64 ticks_per_second;
    f64 millis;

    NuTimeGetTicksPerSecondPS(&low, &high);

    ticks = t->low + t->high * 4.294967295e+09;
    ticks_per_second = low + high * 4.294967295e+09;

    millis = ticks / ticks_per_second * 1000.0;

    return millis;
}

f32 NuTimeScanlines(NUTIME *t) {
    return NuTimeSeconds(t) * 272.0f * 60.0f;
}

void NuKeyboardRead(void) {
}

i32 NuKeyboard(i32 key) {
    return 0;
}

i32 NuKeyboard_db(i32 key) {
    return 0;
}

i32 NuKey_last(void) {
    return 0;
}

i32 NuKey_current(void) {
    return 0;
}

void NuKey_simple(void) {
}

i32 NuKeyGet(u32 *modifiers) {
    return -1;
}

void NuKeyFlush(void) {
}

void NuMouseRead(void) {
}

f32 NuMouseReadX(void) {
    return 0.0f;
}

f32 NuMouseReadY(void) {
    return 0.0f;
}

f32 NuMouseReadZ(void) {
    return 0.0f;
}

f32 NuMouseReadXRel(void) {
    return 0.0f;
}

f32 NuMouseReadYRel(void) {
    return 0.0f;
}

f32 NuMouseReadZRel(void) {
    return 0.0f;
}

f32 NuMouseReadXVel(void) {
    return 0.0f;
}

f32 NuMouseReadYVel(void) {
    return 0.0f;
}

f32 NuMouseReadZVel(void) {
    return 0.0f;
}

u32 NuMouseReadButtons(void) {
    return 0;
}

u32 NuMouseReadButtons_db(void) {
    return 0;
}

extern "C" {
// The Android implementations ignore their inputs and report no button.
extern "C" i32 NuMouseButton(void) {
    return 0;
}

extern "C" i32 NuMouseButton_db(void) {
    return 0;
}

}

void NuAPIInit(void) {
    memset(&nuapi, 0, sizeof(NUAPI));

    nuapi.fps = 60.0f;
    nuapi.video_mode = NUVIDEOMODE_NTSC;
    nuapi.video_swap_mode = NUVIDEO_SWAPMODE_ROLLING;
    nuapi.language = 1;
    nuapi.video_aspect = 0;
    nuapi.disable_os_menu_freeze = 0;
    nuapi.forced_frame_time = nuapi_forced_frame_time;
    nuapi.max_fps = nuapi_max_fps;

    NuTimeGet(&nuapi.time);

    nuapi.pad_record.buf_size = NUAPI_PADREC_DEFAULT_BUFFERSIZE;
    nuapi.pad_record.end_record_buttons = 0;
    nuapi.pad_record.end_play_buttons = 0;

    NuWindInitialise(nuapi.wind);
}

i32 NuInitHardware(VARIPTR *buf, VARIPTR *buf_end, i32 heap_size, ...) {
    static char *dfs_name = "";
    static char *sfxdfs_name = NULL;
    i32 texture_count = 3000;
    i32 material_count = 512;
    i32 hostfs = 0;
    i32 streamsize = 0x200000;
    NUPAD **pad0 = NULL;
    NUPAD **pad1 = NULL;
    NUPAD **pad2 = NULL;
    NUPAD **pad3 = NULL;
    i32 video_mode_argument = 2;
    i32 audio_channels = 4;
    i32 audio_buffer_size = 4096;
    i32 *cd_status = NULL;
    i32 resolution_x = 0;
    i32 resolution_y = 0;
    i32 audio_enabled = 1;
    i32 video_mode = 2;
    i32 unused = 0;
    u32 framebuffer_format = 0;
    i32 flags = 0;
    i32 occlusion_capacity = 0;
    i32 animation_count = 160;
    NUVIDEO_SWAPMODE swapmode = static_cast<NUVIDEO_SWAPMODE>(3);
    i32 framebuffer_flags;
    i32 setup_tok;
    i32 audio_arg;
    i32 dummy;
    i32 reserved;
    i32 audio_first;
    i32 file_status;
    NUDATHDR *dat;
    va_list args;
    VARIPTR buffer_end;

    swapmode = NUVIDEO_SWAPMODE_FIELDSYNC;
    NuAPIInit();
    ParseCommandLine();
    va_start(args, heap_size);
    do {
        setup_tok = va_arg(args, i32);
        switch (setup_tok) {
            case NUAPI_SETUP_HOSTFS:
                hostfs = va_arg(args, i32);
                break;
            case 0x16:
                dfs_name = va_arg(args, char *);
                break;
            case 0x19:
                sfxdfs_name = va_arg(args, char *);
                break;
            case NUAPI_SETUP_STREAMSIZE:
                streamsize = va_arg(args, i32);
                break;
            case NUAPI_SETUP_PAD0:
                pad0 = va_arg(args, NUPAD **);
                break;
            case NUAPI_SETUP_PAD1:
                pad1 = va_arg(args, NUPAD **);
                break;
            case 0x10:
                pad2 = va_arg(args, NUPAD **);
                break;
            case 0x11:
                pad3 = va_arg(args, NUPAD **);
                break;
            case NUAPI_SETUP_VIDEOMODE:
                video_mode_argument = va_arg(args, i32);
                break;
            case NUAPI_SETUP_SWAPMODE:
                swapmode = static_cast<NUVIDEO_SWAPMODE>(va_arg(args, i32));
                break;
            case NUAPI_SETUP_RESOLUTION:
                resolution_x = va_arg(args, i32);
                resolution_y = va_arg(args, i32);
                break;
            case 9:
                audio_arg = va_arg(args, i32);
                dummy = va_arg(args, i32);
                break;
            case NUAPI_SETUP_AUDIO_DISABLED:
                audio_enabled = 0;
                dummy = va_arg(args, i32);
                dummy = va_arg(args, i32);
                dummy = va_arg(args, i32);
                dummy = va_arg(args, i32);
                break;
            case 0x1a:
                texture_count = va_arg(args, i32);
                break;
            case 0x1e:
                audio_channels = va_arg(args, i32);
                audio_buffer_size = va_arg(args, i32);
                break;
            case 0x25:
                material_count = va_arg(args, i32);
                break;
            case 0x3e:
                nuapi_deferspotfxload = va_arg(args, i32);
                break;
            case 0x41:
                nuapi_use_target_manager = va_arg(args, i32);
                break;
            case 0x42:
                nuapi_use_target_manager = va_arg(args, i32);
                nuapi_target_manager_mac_address = va_arg(args, char *);
                break;
            case 0x43:
                nuapi_use_target_manager_for_sound = va_arg(args, i32);
                break;
            case 0x44:
                animation_count = va_arg(args, i32);
                break;
            case 0x52:
                NuPadSetMaxGamePads(va_arg(args, i32));
                break;
            case 0x45:
                switch (va_arg(args, i32)) {
                    case 1:
                        framebuffer_format = 0;
                        break;
                    case 2:
                        framebuffer_format = 1;
                        break;
                    case 4:
                        framebuffer_format = 2;
                        break;
                }
                break;
            case NUAPI_SETUP_0x46:
                flags |= va_arg(args, i32) != 0 ? 4 : 0;
                break;
            case NUAPI_SETUP_0x47:
                flags |= va_arg(args, i32) != 0 ? 8 : 0;
                break;
            case 0x48:
                flags |= va_arg(args, i32) != 0 ? 16 : 0;
                break;
            case NUAPI_SETUP_0x49:
                flags |= va_arg(args, i32) != 0 ? 32 : 0;
                break;
            case 0x4a:
                flags |= va_arg(args, i32) != 0 ? 64 : 0;
                break;
            case NUAPI_SETUP_0x4b:
                flags |= va_arg(args, i32) != 0 ? 128 : 0;
                break;
            case 0x4c:
                occlusion_capacity = va_arg(args, i32);
                break;
            case 0x4d:
                reserved = va_arg(args, i32);
                break;
            default:
                if (NuInitHardwareParseArgsPS(setup_tok, &args) == 0) {
                    switch (setup_tok) {
                        case 0x14:
                            dummy = va_arg(args, i32);
                            dummy = va_arg(args, i32);
                            break;
                        case NUAPI_SETUP_GLASSRPLANE:
                            dummy = va_arg(args, i32);
                            break;
                        case NUAPI_SETUP_AUDIO:
                            audio_first = va_arg(args, i32);
                            dummy = va_arg(args, i32);
                            dummy = va_arg(args, i32);
                            dummy = va_arg(args, i32);
                            break;
                        case NUAPI_SETUP_CDDVDMODE:
                            dummy = va_arg(args, i32);
                            break;
                        case 6:
                            dummy = va_arg(args, i32);
                            break;
                    }
                }
                break;
            case NUAPI_SETUP_END:
                break;
        }
    } while (setup_tok != NUAPI_SETUP_END);
    va_end(args);

    NuVideoSetMode(video_mode);
    NuVideoSetSwapMode(swapmode);
    NuVideoSetResolution(resolution_x, resolution_y);
    NuInitHardwarePS(buf, buf_end, heap_size);
    buffer_end = *buf_end;
    nuapi.video_aspect = NuVideoGetAspectPS();
    NuVideoSetBrightness(1.0f);
    file_status = NuFileInitEx(hostfs, 0, 0);
    if (cd_status != NULL) {
        if (file_status == -1) {
            *cd_status = 1;
        }
        if (file_status == -2) {
            *cd_status = 0;
        }
    }
    NuTrigInit();
    NuRndrInitEx(streamsize, buf, buffer_end);
    NuPrimInit(buf, buffer_end);
    NuVpInit();
    NuTexInitEx(buf, texture_count);
    NuDisplayListInit(buf, buffer_end);
    framebuffer_flags = 1;
    if ((flags & 32) != 0)
        framebuffer_flags |= 2;
    if ((flags & 16) != 0)
        framebuffer_flags |= 4;
    if ((flags & 12) != 0)
        framebuffer_flags |= 8;
    NuFramebufferInitEx(framebuffer_flags, framebuffer_format, buf, buffer_end);
    NuPostEffectInit(flags | 1, buf, buffer_end);
    NuMtlInitEx(buf, material_count);
    NuRndrInitGeneric();
    NuAnimInit(animation_count, buf, buffer_end);
    if (dfs_name != NULL && *dfs_name != '\0') {
        dat = NuDatOpen(dfs_name, buf, NULL);
        NuDatSet(dat);
    }
    NuPadInit();
    NuOnlineInit();
    if (pad0 != NULL)
        *pad0 = NuPadOpen(0, 0);
    if (pad1 != NULL)
        *pad1 = NuPadOpen(1, 0);
    if (pad2 != NULL)
        *pad2 = NuPadOpen(2, 0);
    if (pad3 != NULL)
        *pad3 = NuPadOpen(3, 0);
    NuQFntInit(buf, buffer_end);
    NuTimeBarInitEx(buf, buffer_end);
    NuTimeInitPS();
    NuOcclusionManagerInit(occlusion_capacity, buf, buffer_end);
    NuRndrBeginScene(-1);
    NuRndrClear(0xb00, 0, 1.0f);
    NuRndrEndScene();
    NuRndrSwapScreen(0);
    NuMtlInitOverride(128, buf, &buffer_end);
    bgProcInit();
    return 1;
}

void NuFrameBegin(void) {
    nuapi.nuframe_begin_cnt++;

    NuPortalInit();
    NuPrimReset();
    NuWaterReset();
    NuMouseRead();
    NuKeyboardRead();
    NuOcclusionManagerBeginFrame();
}

i32 NuFrameEndBgLoadPS(i32 minimum_delay) {
    i32 delay = 0;
    NUTIME now;
    NUTIME elapsed;
    NuTimeGet(&now);
    NuTimeSub(&elapsed, &now, &nuapi.time2);
    const i32 scanlines = static_cast<i32>(NuTimeScanlines(&elapsed));
    if (nuapi.fps == 60.0f) {
        delay = 0xff - scanlines;
    } else if (nuapi.fps == 50.0f) {
        delay = 0x131 - scanlines;
    } else if (nuapi.fps == 30.0f) {
        delay = 0x1e0 - scanlines;
    } else if (nuapi.fps == 25.0f) {
        delay = 0x244 - scanlines;
    }
    if (delay >= minimum_delay) {
        bgSuspendMain(delay);
        return 0;
    }
    return 1;
}

extern "C" {
    void NuFrameSetMinDelay(i32 delay) {
        min_delay = delay;
    }

    void NuErrorSleep(void) {
        f32 milliseconds = 0.0f;
        i32 waiting = 1;
        NUPAD pad;
        memset(&pad, 0, sizeof(pad));
        NUTIME start;
        NuTimeGet(&start);
        while (milliseconds < 5000.0f && waiting != 0) {
            f32 y = 500.0f;
            i32 index = 0;
            NUTIME now;
            NUTIME elapsed;
            NuTimeGet(&now);
            NuTimeSub(&elapsed, &now, &start);
            milliseconds = NuTimeMilliSeconds(&elapsed);
            NuFrameBegin();
            NuRndrBeginScene(0);
            NuRndrClear(0xb00, 0xff00ffff, 1.0f);
            NuQFntSet(system_qfont);
            NuQFntPushPrintMode(2);
            NuQFntPushCoordinateSystem(NUQFNT_CSMODE_PS2);
            NuQFntSet(system_qfont);
            char *message;
            while (*(message = NuGetErrN(index++)) != '\0') {
                NuQFntSetColour(system_qfont, 0xff000000);
                NuQFntMove(system_qfont, 500.0f, y, 0.0f);
                // Error entries are already formatted. Retail passed an uninitialized
                // va_list to NuQFntPrintV here; print the text directly instead.
                NuQFntPrintU(system_qfont, message);
                y += 150.0f;
            }
            NuQFntPopCoordinateSystem();
            NuQFntPopPrintMode();
            NuRndrEndScene();
            NuPadRead(&pad);
            if ((pad.digital_buttons & 0x40) != 0 && milliseconds > 1000.0f) {
                waiting = 0;
            }
            NuFrameEnd();
        }
    }

    f32 NuFrameEnd(void) {
        static i32 ShowingError = 0; // _ZZ10NuFrameEndE12ShowingError

        i32 done = 0;
        i32 has_error = NuHasError();

        if (nuapi.max_fps != 0) {
            // Wait until at least 1/max_fps seconds have elapsed since frame
            // begin (nuapi.time), then record the elapsed time.
            f32 target = 1.0f / (f32)nuapi.max_fps;

            NUTIME now;
            NUTIME delta;
            NuTimeGet(&now);
            NuTimeSub(&delta, &now, &nuapi.time);
            while (target > NuTimeSeconds(&delta)) {
                NuTimeGet(&now);
                NuTimeSub(&delta, &now, &nuapi.time);
            }

            nuapi.frametime = NuTimeSeconds(&delta);
        }

        NuMtlAnimate(nuapi.frametime);
        NuTexAnimProcess(nuapi.frametime);
        NuWindAnimate(nuapi.wind, nuapi.frametime);
        NuOcclusionManagerEndFrame();
        NuPadRecordEndFrame();
        NuTimeBarSetRender(-1);
        NuPad_Interface_Render();

        done = 1;

        if (preRenderFlashingHack != NULL) {
            preRenderFlashingHack();
        }

        NuRndrSwapScreenEx(-1, nuapi_endframe_callbackfn);

        NUTIME end;
        NUTIME delta2;
        NuTimeGet(&end);
        NuTimeSub(&delta2, &end, &nuapi.time);
        nuapi.frametime = NuTimeSeconds(&delta2);
        nuapi.time = end;

        if (nuapi.frametime > 0.1f) {
            nuapi.frametime = 0.1f;
        }

        NuTimeGet(&nuapi.time2);

        if (done) {
            bgSuspendMain(min_delay);
        }

        if (postRenderFlashingHack != NULL) {
            postRenderFlashingHack();
        }

        NuPadUpdatePads();
        nuapi.frame_count++;
        nuapi.nuframe_begin_cnt--;

        if (ShowingError == 0 && has_error != 0) {
            ShowingError = 1;
            NuErrorSleep();
            NuClearError();
            ShowingError = 0;
        }

        nuapi.disable_os_menu_freeze = 0;
        return nuapi.frametime;
    }

    void NuRegisterEndFrameCallBackFn(void (*callback)(void)) {
        nuapi_endframe_callbackfn = callback;
    }

}

void NuTerminateHardware() {
    STUBBED();
}

void ParseCommandLine() {
    while (argc > 0) {
        if (*argv == NULL) {
            break;
        }
        if (NuStrICmp(*argv, "PADRECORD") == 0) {
            ++argv;
            --argc;
            nuapi.pad_record.filepath = *argv;
            nuapi.pad_record.mode = NUPAD_RECORD;
        } else if (NuStrICmp(*argv, "PADPLAY") == 0) {
            ++argv;
            --argc;
            nuapi.pad_record.filepath = *argv;
            nuapi.pad_record.mode = NUPAD_PLAY;
        }
        ++argv;
        --argc;
    }
}

void NuDisableOSMenuFreeze(void) {
    nuapi.disable_os_menu_freeze = 1;
}

void NuPadUseCorrectDeadZoning(i32 state) {
    UseCorrectDeadZoning = state;
}

static void DeadZoneValueXYProper(i32 *x_value, i32 *y_value, i32 deadzone) {
    i32 inverse;
    i32 cur_value;

    inverse = ANALOG_CENTER - deadzone;

    cur_value = *x_value;
    if (cur_value >= deadzone) {
        cur_value++;

        *x_value = ((cur_value - deadzone) * 0x7f) / inverse;
    } else if (cur_value < -deadzone) {
        *x_value = ((cur_value + deadzone) * 0x7f) / inverse;
    } else {
        *x_value = 0;
    }

    cur_value = *y_value;
    if (cur_value >= deadzone) {
        cur_value++;

        *y_value = ((cur_value - deadzone) * 0x7f) / inverse;
    } else if (cur_value < -deadzone) {
        *y_value = ((cur_value + deadzone) * 0x7f) / inverse;
    } else {
        *y_value = 0;
    }
}

static void DeadZoneValueXY(i32 *x_value, i32 *y_value, i32 deadzone) {
    f32 x_normal;
    f32 y_normal;
    f32 deadzone_normal;
    f32 magnitude;

    if (UseCorrectDeadZoning) {
        DeadZoneValueXYProper(x_value, y_value, deadzone);

        return;
    }

    x_normal = *x_value / 127.0f;
    y_normal = *y_value / 127.0f;
    deadzone_normal = deadzone / 127.0f;

    if (x_normal > 1.0f) {
        x_normal = 1.0f;
    }

    if (x_normal < -1.0f) {
        x_normal = -1.0f;
    }

    if (y_normal > 1.0f) {
        y_normal = 1.0f;
    }

    if (y_normal < -1.0f) {
        y_normal = -1.0f;
    }

    magnitude = NuFsqrt(x_normal * x_normal + y_normal * y_normal);

    if (magnitude < deadzone_normal) {
        *x_value = 0;
        *y_value = 0;
    }
}

NUPAD *NuPadOpen(i32 port, i32 slot) {
    NUPAD *pad;

    pad = (NUPAD *)NuMemoryGet()->GetThreadMem()->_BlockAlloc(
        sizeof(NUPAD) + 0x800, 0x4, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "", NUMEMORY_CATEGORY_NONE);

    if (pad != NULL) {
        memset(pad, 0, sizeof(NUPAD));

        pad->port = port;
        pad->slot = slot;
        pad->stage = 0;

        pad->analog_right_x = pad->analog_right_y = pad->analog_left_x = pad->analog_left_y = ANALOG_CENTER;
    }

    NuPadOpenPS(pad);

    return pad;
}

void NuPadClose(NUPAD *pad) {
    NuPadClosePS(pad);
    NuMemoryGet()->GetThreadMem()->BlockFree(pad, 0);
}

static f32 CalcRamp(f32 ramp) {
    i32 i;

    ramp = MIN(ramp, 1.0f);

    for (i = 0; ramp > ramp_vals[i][0]; i++) {
    }

    return ramp_vals[i][1];
}

extern "C" {
extern "C" void NuPadSetStatus(i32 pad, i32 status) {
    if (status == NUPAD_STATUS_ACTIVE) {
        if (g_atLeastOnePadBeenActivated == 0) {
            g_profilePlayerPad = g_nupadMapping[pad].port;
        }
        g_atLeastOnePadBeenActivated = 1;
    }
    g_nupadMapping[pad].is_active = status;
}

}

i32 NuPadGetFirstActivePad() {
    return g_profilePlayerPad;
}

void NuPadProcessReadData(NUGENERICPAD *pad) {
    f32 ramp_rate_normal;

    pad->digital_buttons_pressed = pad->digital_buttons & ~pad->digital_buttons_prev;
    pad->digital_buttons_released = ~pad->digital_buttons & pad->digital_buttons_prev;

    pad->digital_buttons_prev = pad->digital_buttons;

    pad->digital_left_x = pad->analog_left_x - ANALOG_CENTER;
    pad->digital_left_y = ANALOG_CENTER - pad->analog_left_y;

    pad->digital_right_x = pad->analog_right_x - ANALOG_CENTER;
    pad->digital_right_y = ANALOG_CENTER - pad->analog_right_y;

    if (pad->deadzone != 0) {
        DeadZoneValueXY(&pad->digital_left_x, &pad->digital_left_y, pad->deadzone);
        DeadZoneValueXY(&pad->digital_right_x, &pad->digital_right_y, pad->deadzone);

        // Keep analog values consistent after deadzone modification.
        pad->analog_left_x = pad->digital_left_x + ANALOG_CENTER;
        pad->analog_left_y = ANALOG_CENTER - pad->digital_left_y;

        pad->analog_right_x = pad->digital_right_x + ANALOG_CENTER;
        pad->analog_right_y = ANALOG_CENTER - pad->digital_right_y;
    }

    pad->left_x_normal = (f32)pad->digital_left_x;
    pad->left_y_normal = (f32)pad->digital_left_y;

    pad->left_x_normal /= 128.0f;
    pad->left_y_normal /= 128.0f;

    if (pad->left_x_normal > 0.0f) {
        pad->left_x_normal = MAX(0.0f, pad->left_x_normal - pad->left_x_unknown2);
    }

    if (pad->left_x_normal < 0.0f) {
        pad->left_x_normal = MIN(0.0f, pad->left_x_normal + pad->left_x_unknown2);
    }

    if (pad->left_y_normal > 0.0f) {
        pad->left_y_normal = MAX(0.0f, pad->left_y_normal - pad->left_y_unknown2);
    }

    if (pad->left_y_normal < 0.0f) {
        pad->left_y_normal = MIN(0.0f, pad->left_y_normal + pad->left_y_unknown2);
    }

    pad->right_x_normal = (f32)pad->digital_right_x;
    pad->right_y_normal = (f32)pad->digital_right_y;

    pad->right_x_normal /= 128.0f;
    pad->right_y_normal /= 128.0f;

    if (pad->right_x_normal > 0.0f) {
        pad->right_x_normal = MAX(0.0f, pad->right_x_normal - pad->right_x_unknown2);
    }

    if (pad->right_x_normal < 0.0f) {
        pad->right_x_normal = MIN(0.0f, pad->right_x_normal + pad->right_x_unknown2);
    }

    if (pad->right_y_normal > 0.0f) {
        pad->right_y_normal = MAX(0.0f, pad->right_y_normal - pad->right_y_unknown2);
    }

    if (pad->right_y_normal < 0.0f) {
        pad->right_y_normal = MIN(0.0f, pad->right_y_normal + pad->right_y_unknown2);
    }

    if ((pad->analog_button_flags & 1) != 0) {
        // The right-hand side values never appear to be written. This may be a
        // platform difference.
        pad->analog_left_pad_left = pad->unknown_7c;
        pad->analog_left_pad_right = pad->unknown_7d;
        pad->analog_left_pad_up = pad->unknown_7e;
        pad->analog_left_pad_down = pad->unknown_7f;
    } else {
        ramp_rate_normal = ramp_rate / 30.0f;

        if ((pad->digital_buttons & PAD_BUTTON_UP) != 0) {
            pad->left_up_ramp = MIN(1.0f, pad->left_up_ramp + ramp_rate_normal);
        } else {
            pad->left_up_ramp = 0.0f;
        }

        if ((pad->digital_buttons & PAD_BUTTON_DOWN) != 0) {
            pad->left_down_ramp = MIN(1.0f, pad->left_down_ramp + ramp_rate_normal);
        } else {
            pad->left_down_ramp = 0.0f;
        }

        if ((pad->digital_buttons & PAD_BUTTON_LEFT) != 0) {
            pad->left_left_ramp = MIN(1.0f, pad->left_left_ramp + ramp_rate_normal);
        } else {
            pad->left_left_ramp = 0.0f;
        }

        if ((pad->digital_buttons & PAD_BUTTON_RIGHT) != 0) {
            pad->left_right_ramp = MIN(1.0f, pad->left_right_ramp + ramp_rate_normal);
        } else {
            pad->left_right_ramp = 0.0f;
        }

        pad->analog_left_pad_left = CalcRamp(pad->left_left_ramp) * 255.0f;
        pad->analog_left_pad_right = CalcRamp(pad->left_right_ramp) * 255.0f;
        pad->analog_left_pad_up = CalcRamp(pad->left_up_ramp) * 255.0f;
        pad->analog_left_pad_down = CalcRamp(pad->left_down_ramp) * 255.0f;
    }

    if ((pad->analog_button_flags & 2) != 0) {
        pad->unknown_82 = pad->unknown_80;
        pad->unknown_83 = pad->unknown_81;
    } else {
        pad->unknown_82 = 0x7f;
        pad->unknown_83 = 0x7f;
    }
}

i32 NuPadGetMaxGamePads() {
    return MaxGamePads;
}

void NuPadSetMaxGamePads(i32 count) {
    MaxGamePads = count;
}

void NuPadSetValid(i32 port, i32 valid) {
    g_nupadScannedPads[port].is_valid = valid;
}

void NuPadUpdatePads() {
    i32 max_interface_devices;
    i32 max_game_pads;
    i32 i;
    i32 j;
    i32 has_active_pad;
    i32 unused;
    i32 unused2;

    max_interface_devices = NuPad_Interface_GetMaxDevices();
    max_game_pads = NuPadGetMaxGamePads();

    NuPad_Interface_InputManagerUpdate(0.0f);

    for (i = 0; i < max_interface_devices; i++) {
        g_nupadScannedPads[i].is_valid = NuPad_Interface_NuPadRead(
            i, &g_nupadScannedPads[i].analog_left_x, &g_nupadScannedPads[i].analog_left_y,
            &g_nupadScannedPads[i].analog_right_x, &g_nupadScannedPads[i].analog_right_y,
            &g_nupadScannedPads[i].analog_l1, &g_nupadScannedPads[i].analog_l2, &g_nupadScannedPads[i].analog_r1,
            &g_nupadScannedPads[i].analog_r2, &g_nupadScannedPads[i].digital_buttons, &g_nupadScannedPads[i].unknown_7c,
            &g_nupadScannedPads[i].analog_button_flags);

        NuPadRecordPlay(g_nupadScannedPads + i);

        if (g_nupadScannedPads[i].is_valid) {
            g_nupadScannedPads[i].deadzone = NuPadGetDeadzoneByPortPS(i);
        }

        NuPadProcessReadData(g_nupadScannedPads + i);
    }

    if (nuapi.pad_record.should_end == 1) {
        NuPadRecordEnd();
    }

    for (i = 0; i < max_interface_devices; i++) {
        has_active_pad = false;

        for (j = 0; j < max_game_pads; j++) {
            if (g_nupadMapping[j].is_active && g_nupadMapping[j].port == i) {
                has_active_pad = true;
            }
        }

        if (has_active_pad) {
            continue;
        }

        g_nupadScannedPads[i].mapped_to_pad = -1;
    }

    if (!g_atLeastOnePadBeenActivated) {
        g_nupadMapping[0].port = -1;

        for (j = 0; j < max_interface_devices; j++) {
            if (g_nupadScannedPads[j].is_valid) {
                if (g_nupadMapping[0].port == -1) {
                    g_nupadMapping[0].port = j;
                    g_nupadScannedPads[j].mapped_to_pad = 0;
                }

                if (g_nupadScannedPads[j].digital_buttons != 0 ||
                    g_nupadScannedPads[j].analog_left_x != ANALOG_CENTER ||
                    g_nupadScannedPads[j].analog_left_y != ANALOG_CENTER ||
                    g_nupadScannedPads[j].analog_right_x != ANALOG_CENTER ||
                    g_nupadScannedPads[j].analog_right_y != ANALOG_CENTER) {
                    unused = 1;
                    g_nupadMapping[0].port = j;
                    g_nupadScannedPads[j].mapped_to_pad = 0;

                    if (enable_touch_controls) {
                        return;
                    }

                    if ((g_nupadScannedPads[j].digital_buttons_pressed & (PAD_BUTTON_START | PAD_BUTTON_JUMP_SELECT)) !=
                        0) {
                        return;
                    }
                }
            }
        }
    }

    if (g_atLeastOnePadBeenActivated) {
        if (g_nupadMapping[0].is_active) {
            unused2 = 0;
        } else {
            unused2 = 1;
        }

        for (i = 0; i < max_game_pads; i++) {
            if (g_nupadMapping[i].is_active == 1 && !g_nupadScannedPads[g_nupadMapping[i].port].is_valid) {
                for (j = 0; j < max_interface_devices; j++) {
                    if (g_nupadScannedPads[j].is_valid &&
                        (g_nupadScannedPads[j].digital_buttons_pressed & PAD_BUTTON_START) != 0 &&
                        (g_nupadScannedPads[j].mapped_to_pad == -1 ||
                         !g_nupadMapping[g_nupadScannedPads[j].mapped_to_pad].is_active)) {
                        g_nupadMapping[i].port = j;
                        g_nupadScannedPads[j].mapped_to_pad = i;
                        g_nupadScannedPads[j].digital_buttons_pressed = 0;

                        return;
                    }
                }
            }

            if (!g_nupadMapping[i].is_active) {
                g_nupadMapping[i].port = -1;

                if (g_nupadMapping[0].is_active + g_nupadMapping[1].is_active == 0) {
                    if (i == 0) {
                        for (j = 0; j < max_interface_devices; j++) {
                            if (g_nupadScannedPads[j].mapped_to_pad != -1 &&
                                g_nupadMapping[g_nupadScannedPads[j].mapped_to_pad].is_active == 1) {
                                continue;
                            }

                            g_nupadScannedPads[j].mapped_to_pad = -1;
                        }
                    }
                } else {
                    for (j = 0; j < max_interface_devices; j++) {
                        if (g_nupadScannedPads[j].mapped_to_pad != -1 &&
                            g_nupadMapping[g_nupadScannedPads[j].mapped_to_pad].is_active == 1) {
                            continue;
                        }

                        g_nupadScannedPads[j].mapped_to_pad = -1;
                    }
                }

                for (j = 0; j < max_interface_devices; j++) {
                    if (g_nupadScannedPads[j].mapped_to_pad == -1 && g_nupadScannedPads[j].is_valid) {
                        g_nupadMapping[i].port = j;
                        g_nupadScannedPads[j].mapped_to_pad = i;

                        if (g_nupadScannedPads[j].digital_buttons != 0 ||
                            g_nupadScannedPads[j].analog_left_x != ANALOG_CENTER ||
                            g_nupadScannedPads[j].analog_left_y != ANALOG_CENTER ||
                            g_nupadScannedPads[j].analog_right_x != ANALOG_CENTER ||

                            // ORIG_BUG: This tests `analog_right_x` twice.
                            g_nupadScannedPads[j].analog_right_x != ANALOG_CENTER ||
                            (g_nupadScannedPads[j].digital_buttons_pressed &
                             (PAD_BUTTON_START | PAD_BUTTON_JUMP_SELECT)) != 0) {
                            break;
                        }
                    }
                }
            }
        }
    }
}

void NuPadMapPortToPS2Port(i32 pad, i32 port) {
    g_nupadMapping[pad].port = port;
    g_nupadScannedPads[port].mapped_to_pad = pad;
    g_nupadScannedPads[port].digital_state.pressed = 0;
}

void NuPadMapPlayerToPort(i32 player, i32 port) {
    NuPadSetStatus(player, NUPAD_STATUS_INACTIVE);
    NuPadMapPortToPS2Port(player, port);
    NuPadSetStatus(player, NUPAD_STATUS_ACTIVE);
    PlayerLastPort[player] = port;
}

i32 NuPadGetPort(i32 player) {
    i32 port = g_nupadMapping[player].port;
    if (port >= 0 && g_nupadScannedPads[port].is_valid)
        return port;
    return -1;
}

i32 NuPadGetPlayer(i32 port) {
    i32 player = -1;
    if (g_nupadScannedPads[port].is_valid)
        player = g_nupadScannedPads[port].mapped_to_pad;
    return player;
}

void NuPadInit() {
    i32 i;

    memset(g_nupadScannedPads, 0, sizeof(g_nupadScannedPads));

    for (i = 0; i < MAX_GAME_PAD_COUNT; i++) {
        g_nupadScannedPads[i].mapped_to_pad = -1;

        NuPadInitPS(g_nupadScannedPads + i);
    }

    for (i = 0; i < MAX_GAME_PAD_COUNT; i++) {
        g_nupadMapping[i].is_active = 0;
        g_nupadMapping[i].pad = -1;
        g_nupadMapping[i].port = -1;
    }

    g_atLeastOnePadBeenActivated = 0;
}

void NuPadSetDirectMappingState(i32 state) {
    g_directpadMapping = state;
}

i32 NuPadRead(NUPAD *pad) {
    i32 port;

    if (g_nupadMapping[pad->port].port != -1 || (g_directpadMapping && g_nupadScannedPads[pad->port].is_valid)) {
        if (g_directpadMapping != 0) {
            port = pad->port;
        } else {
            port = g_nupadMapping[pad->port].port;
        }

        pad->analog_left_x = g_nupadScannedPads[port].analog_left_x;
        pad->analog_left_y = g_nupadScannedPads[port].analog_left_y;
        pad->analog_right_x = g_nupadScannedPads[port].analog_right_x;
        pad->analog_right_y = g_nupadScannedPads[port].analog_right_y;

        pad->analog_l1 = g_nupadScannedPads[port].analog_l1;
        pad->analog_l2 = g_nupadScannedPads[port].analog_l2;
        pad->analog_r1 = g_nupadScannedPads[port].analog_r1;
        pad->analog_r2 = g_nupadScannedPads[port].analog_r2;

        pad->digital_buttons = g_nupadScannedPads[port].digital_buttons;
        pad->digital_buttons_pressed = g_nupadScannedPads[port].digital_buttons_pressed;
        pad->digital_buttons_released = g_nupadScannedPads[port].digital_buttons_released;
        pad->digital_buttons_prev = g_nupadScannedPads[port].digital_buttons_prev;

        pad->has_analog_buttons = g_nupadScannedPads[port].analog_button_flags;

        pad->deadzone = g_nupadScannedPads[port].deadzone;

        pad->digital_left_x = g_nupadScannedPads[port].digital_left_x;
        pad->digital_left_y = g_nupadScannedPads[port].digital_left_y;
        pad->digital_right_x = g_nupadScannedPads[port].digital_right_x;
        pad->digital_right_y = g_nupadScannedPads[port].digital_right_y;

        pad->left_x_normal = g_nupadScannedPads[port].left_x_normal;
        pad->left_y_normal = g_nupadScannedPads[port].left_y_normal;
        pad->left_x_unknown = g_nupadScannedPads[port].left_x_unknown;
        pad->left_y_unknown = g_nupadScannedPads[port].left_y_unknown;
        pad->left_x_unknown2 = g_nupadScannedPads[port].left_x_unknown2;
        pad->left_y_unknown2 = g_nupadScannedPads[port].left_y_unknown2;

        pad->right_x_unknown2 = g_nupadScannedPads[port].right_x_unknown2;
        pad->right_y_unknown2 = g_nupadScannedPads[port].right_y_unknown2;
        pad->right_x_normal = g_nupadScannedPads[port].right_x_normal;
        pad->right_y_normal = g_nupadScannedPads[port].right_y_normal;
        pad->right_x_unknown = g_nupadScannedPads[port].right_x_unknown;
        pad->right_y_unknown = g_nupadScannedPads[port].right_y_unknown;

        pad->analog_left_pad_left = g_nupadScannedPads[port].analog_left_pad_left;
        pad->analog_left_pad_right = g_nupadScannedPads[port].analog_left_pad_right;
        pad->analog_left_pad_up = g_nupadScannedPads[port].analog_left_pad_up;
        pad->analog_left_pad_down = g_nupadScannedPads[port].analog_left_pad_down;

        pad->unknown_b0 = g_nupadScannedPads[port].unknown_82;
        pad->unknown_b1 = g_nupadScannedPads[port].unknown_83;

        pad->is_valid = g_nupadScannedPads[port].is_valid;

        return pad->is_valid;
    }

    pad->is_valid = false;

    return 0;
}

void NuPadSetMotors(NUPAD *pad, i32 motor0, i32 motor1) {
    if (g_directpadMapping && g_nupadScannedPads[pad->port].is_valid) {
        NuPadSetMotorsPS(pad->port, motor0, motor1);
    } else if (g_nupadMapping[pad->port].port != -1) {
        NuPadSetMotorsPS(g_nupadMapping[pad->port].port, motor0, motor1);
    }
}

extern "C" {
extern "C" void NuPadResetState(i32 preserve_scanned_state) {
    g_atLeastOnePadBeenActivated = 0;
    g_profilePlayerPad = -1;
    g_nupadMapping[0].port = -1;
    g_nupadMapping[1].port = -1;

    if (preserve_scanned_state == 0) {
        for (i32 i = 0; i <= 3; i++) {
            g_nupadScannedPads[i].digital_state.pressed = 0;
            g_nupadScannedPads[i].digital_state.previous = g_nupadScannedPads[i].digital_state.pressed;
            g_nupadScannedPads[i].digital_state.buttons = g_nupadScannedPads[i].digital_state.previous;
        }
    }
}

}

void NuPadRecordStart(void) {
    nuapi.pad_record.is_recording = 1;
}

void NuPadRecordEndFrame() {
    VARIPTR *cursor;
    if (nuapi.pad_record.is_recording) {
        cursor = (VARIPTR *)&nuapi.pad_record.buf_start;
        nuapi.pad_record.is_recording = 0;
    }
    cursor = &nuapi.pad_record.record;
    switch (nuapi.pad_record.mode) {
        case NUPAD_PLAY:
            if (cursor->u8_ptr == nuapi.pad_record.buf_end) {
                nuapi.pad_record.mode = NUPAD_NORM;
            } else {
                nuapi.frametime = *cursor->f32_ptr++;
            }
            break;
        case NUPAD_RECORD:
            *cursor->f32_ptr++ = nuapi.frametime;
            if (cursor->u8_ptr > nuapi.pad_record.buf_end - 0x308) {
                nuapi.pad_record.mode = NUPAD_NORM;
            }
            break;
        default:
            break;
    }
}

void NuPadRecordPlay(NUGENERICPAD *pad) {
    VARIPTR *record_ptr;

    if (pad->is_valid && nuapi.pad_record.mode == NUPAD_PLAY &&
        (pad->digital_buttons & nuapi.pad_record.end_play_buttons) != 0) {
        nuapi.pad_record.mode = NUPAD_NORM;
    }

    if (pad->is_valid && nuapi.pad_record.mode == NUPAD_RECORD &&
        (pad->digital_buttons & nuapi.pad_record.end_record_buttons) != 0) {
        nuapi.pad_record.should_end = 1;
    }

    record_ptr = &nuapi.pad_record.record;

    switch (nuapi.pad_record.mode) {
        case NUPAD_RECORD:
            *record_ptr->u32_ptr++ = pad->is_valid;

            if (pad->is_valid) {
                *record_ptr->u8_ptr++ = pad->analog_left_x;
                *record_ptr->u8_ptr++ = pad->analog_left_y;
                *record_ptr->u8_ptr++ = pad->analog_right_x;
                *record_ptr->u8_ptr++ = pad->analog_right_y;

                *record_ptr->u8_ptr++ = pad->analog_l1;
                *record_ptr->u8_ptr++ = pad->analog_l2;
                *record_ptr->u8_ptr++ = pad->analog_r1;
                *record_ptr->u8_ptr++ = pad->analog_r2;

                *record_ptr->u32_ptr++ = pad->digital_buttons;
            }
            break;
        case NUPAD_PLAY:
            pad->is_valid = *record_ptr->u32_ptr++;

            if (pad->is_valid) {
                pad->analog_left_x = *record_ptr->u8_ptr++;
                pad->analog_left_y = *record_ptr->u8_ptr++;
                pad->analog_right_x = *record_ptr->u8_ptr++;
                pad->analog_right_y = *record_ptr->u8_ptr++;

                pad->analog_l1 = *record_ptr->u8_ptr++;
                pad->analog_l2 = *record_ptr->u8_ptr++;
                pad->analog_r1 = *record_ptr->u8_ptr++;
                pad->analog_r2 = *record_ptr->u8_ptr++;

                pad->digital_buttons = *record_ptr->u32_ptr++;
            }
            break;
        default:
            break;
    }
}

void NuPadRecordSave(char *filepath) {
    NUFILE file;
    u8 *record_start;
    usize record_size;
    NUPADREC *record;

    record = &nuapi.pad_record;

    if (record->mode == NUPAD_RECORD && record->record.u8_ptr != record->buf_start) {
        if (filepath != NULL) {
            file = NuFileOpen(filepath, NUFILE_WRITE);

            if (file != 0) {
                record_size = record->record.addr - (usize)record->buf_start;

                record_start = record->buf_start;

                NuFileWriteInt(file, record_size);
                NuFileWrite(file, record_start, record_size);
                NuFileWriteUnsignedInt(file, 0xe0f);

                NuFileClose(file);
            }
        }

        record->should_end = 0;
        record->mode = NUPAD_NORM;
    }
}

void NuPadRecordLoad(char *filepath, VARIPTR *buffer, VARIPTR end) {
    NUPADREC *record = &nuapi.pad_record;
    record->mode = NUPAD_NORM;
    if (filepath != NULL) {
        NUFILE file = NuFileOpen(filepath, NUFILE_READ);
        if (file != 0) {
            i32 size = NuFileReadInt(file);
            record->record.void_ptr = NuMemoryGet()->GetThreadMem()->_BlockAlloc(
                size + 4, 4, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "", NUMEMORY_CATEGORY_NONE);
            record->record.addr = (record->record.addr + 3) & ~(usize)3;
            if (record->record.void_ptr != NULL) {
                record->buf_start = record->record.u8_ptr;
                NuFileRead(file, record->record.void_ptr, size);
                NuFileClose(file);
                record->buf_end = record->record.u8_ptr + size;
                record->mode = NUPAD_PLAY;
            }
        }
    }
}

void NuPadRecordInit(NURECMODE mode, char *filepath, VARIPTR *buffer, VARIPTR end, i32 size) {
    NUPADREC *record = &nuapi.pad_record;
    switch (mode) {
        default:
            record->mode = NUPAD_NORM;
            break;
        case NUPAD_RECORD:
            record->mode = NUPAD_RECORD;
            record->record.void_ptr = NuMemoryGet()->GetThreadMem()->_BlockAlloc(
                size + 4, 4, NuMemoryManager::MEM_ALLOC_SET_TO_ZERO, "", NUMEMORY_CATEGORY_NONE);
            record->record.addr = (record->record.addr + 3) & ~(usize)3;
            record->buf_start = record->record.u8_ptr;
            record->buf_end = record->record.u8_ptr + size;
            record->filepath = filepath;
            break;
        case NUPAD_PLAY:
            NuPadRecordLoad(filepath, buffer, end);
            record->filepath = filepath;
            break;
    }
}

void NuPadRecordEnd() {
    switch (nuapi.pad_record.mode) {
        case NUPAD_RECORD:
            NuPadRecordSave(nuapi.pad_record.filepath);
            break;
        case NUPAD_PLAY:
            nuapi.pad_record.mode = NUPAD_NORM;
            nuapi.pad_record.record.u8_ptr = nuapi.pad_record.buf_start;
            break;
    }

    nuapi.pad_record.should_end = 0;
}

void NuPadRecordSetPlayEndButtons(i32 buttons) {
    nuapi.pad_record.end_play_buttons = buttons;
}

void NuPadRecordSetRecordEndButtons(i32 buttons) {
    nuapi.pad_record.end_record_buttons = buttons;
}

extern "C" {
    void SavePadRecord(void) {
        STUBBED();
    }

    void SetPadRecPtr(void) {
        STUBBED();
    }

    void InitPadPlayRecord(void) {
        STUBBED();
    }

}

void *NuPtrBlockRead(NUFILE file) {
    void *block = NuMemFileAddr(file);
    return NuPtrBlockFix(block);
}

void *NuPtrBlockFix(void *block) {
    VARIPTR buf;
    VARIPTR ptr;
    VARIPTR start;
    i32 count;
    i32 i;

    start.void_ptr = buf.void_ptr = block;
    start.addr += 4;

    buf.addr = *buf.u32_ptr + buf.addr;

    count = *buf.u32_ptr++;

    for (i = 0; i < count; i++) {
        ptr.addr = *buf.u32_ptr + buf.addr;

        if (*ptr.u32_ptr != 0) {
            *ptr.u32_ptr = *ptr.u32_ptr + ptr.addr;
        }

        buf.addr += 4;
    }

    return start.void_ptr;
}

i32 NuVideoGetMode(void) {
    return nuapi.video_mode;
}

i32 NuVideoGetAspect(void) {
    return nuapi.video_aspect;
}

void NuVideoSetMode(i32 mode) {
    nuapi.video_mode = mode;

    switch (nuapi.video_mode) {
        case NUVIDEOMODE_PAL:
        case NUVIDEOMODE_PALFF:
            nuapi.video_is_pal = 1;
            break;
        case NUVIDEOMODE_NTSC:
        case NUVIDEOMODE_NTSCFF:
        case NUVIDEOMODE_NTSC_J:
            nuapi.video_is_pal = 0;
            break;
        default:
            break;
    }
}

void NuVideoSetResolution(i32 width, i32 height) {
    // It's unclear exactly what was here, but this matches and makes a sort of
    // sense.
    if (width == 0 | height == 0) {
    }

    nuapi.screen_width = width;
    nuapi.screen_height = height;
}

NUVIDEO_SWAPMODE NuVideoSetSwapMode(NUVIDEO_SWAPMODE video_swap_mode) {
    NUVIDEO_SWAPMODE swap_mode = nuapi.video_swap_mode;
    nuapi.video_swap_mode = video_swap_mode;

    switch (nuapi.video_swap_mode) {
        // notably ternarys DO NOT MATCH here :)
        case NUVIDEO_SWAPMODE_ASYNC:
            if (nuapi.video_is_pal) {
                nuapi.fps = 50.0f;
            } else {
                nuapi.fps = 60.0f;
            }
            break;
        case NUVIDEO_SWAPMODE_FIELDSYNC:
            if (nuapi.video_is_pal) {
                nuapi.fps = 50.0f;
            } else {
                nuapi.fps = 60.0f;
            }
            break;
        case NUVIDEO_SWAPMODE_FRAMESYNC:
            if (nuapi.video_is_pal) {
                nuapi.fps = 25.0f;
            } else {
                nuapi.fps = 30.0f;
            }
            break;
        case NUVIDEO_SWAPMODE_MIN2FIELD:
            if (nuapi.video_is_pal) {
                nuapi.fps = 25.0f;
            } else {
                nuapi.fps = 30.0f;
            }
            break;
        case NUVIDEO_SWAPMODE_ROLLING:
            if (nuapi.video_is_pal) {
                nuapi.fps = 50.0f;
            } else {
                nuapi.fps = 60.0f;
            }
            break;
        case NUVIDEO_SWAPMODE_ROLLING2FRAMES:
            if (nuapi.video_is_pal) {
                nuapi.fps = 25.0f;
            } else {
                nuapi.fps = 30.0f;
            }
            break;
        default:
            break;
    }

    NuVideoSetSwapModePS(video_swap_mode);
    return swap_mode;
}

NUVIDEO_SWAPMODE NuVideoGetSwapMode(void) {
    return nuapi.video_swap_mode;
}

void NuVideoRollingFrameRateReset(void) {
}

void NuVideoSetBrightness(f32 brightness) {
    nuapi.video_brightness = brightness;

    NuVideoSetBrightnessPS();
}

i32 NuOnlineAchievementAchieved(i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedPS(achievement, callback);
}

i32 NuOnlineAchievementAchievedEx(i32 player, i32 achievement, NUONLINEACHIEVEMENTCALLBACK callback) {
    return NuOnlineAchievementAchievedExPS(player, achievement, callback);
}

extern "C" {
    void NuOnlineSignInPlayer(void) {
        STUBBED();
    }

}

i32 NuOnlineHasPlayerDownloaded(u32 argument) {
    return NuOnlineHasPlayerDownloadedPS(argument);
}

i32 NuOnlineHasPlayerSignedIn(void) {
    return NuOnlineHasPlayerSignedInPS();
}

i32 NuOnlineHasPlayerSignedInEx(void) {
    STUBBED();
    return 0;
}

void NuOnlineInit(void) {
    NuOnlineInitPS();
}

extern "C" {
    // These entry points are empty in the original Android implementation.
    void NuOnlineSetPresenceMode(void) {
        STUBBED();
    }

    void NuOnlineSetDefaultPresenceMode(void) {
        STUBBED();
    }

}

void NuOnlineSetContext(void) {
    STUBBED();
}

void NuOnlineSetDefaultContext(void) {
    STUBBED();
}

extern "C" {
    void NuOnlineSetProperty(void) {
        STUBBED();
    }

    void NuOnlineSetPresenceModeEx(void) {
        STUBBED();
    }

    void NuOnlineSetDefaultPresenceModeEx(void) {
        STUBBED();
    }

    void NuOnlineSetContextEx(void) {
        STUBBED();
    }

    void NuOnlineSetDefaultContextEx(void) {
        STUBBED();
    }

    void NuOnlineSetPropertyEx(void) {
        STUBBED();
    }

    i32 NuLanguageGet(void) {
        return nuapi.language;
    }

    void NuLanguageSet(i32 language) {
        nuapi.language = language;
    }

}

static i32 isnum(char c) {
    if (((u32)c - 0x30) < 10) {
        return 1;
    }

    return 0;
}

// nurdpi.cpp tokens (the unit also holds nurdpf.cpp's).
#define TOK_ADD TOKI_ADD
#define TOK_SUB TOKI_SUB
#define TOK_MUL TOKI_MUL
#define TOK_DIV TOKI_DIV
#define TOK_MOD TOKI_MOD
#define TOK_LPAREN TOKI_LPAREN
#define TOK_RPAREN TOKI_RPAREN
#define TOK_END TOKI_END
#define TOK_NUM TOKI_NUM
#define TOK_OR TOKI_OR
#define TOK_AND TOKI_AND
#define TOK_NOT TOKI_NOT
#define TOK_XOR TOKI_XOR
#define TOK_LOGICAL_OR TOKI_LOGICAL_OR
#define TOK_LOGICAL_AND TOKI_LOGICAL_AND
#define TOK_LOGICAL_NOT TOKI_LOGICAL_NOT
#define TOK_EQ TOKI_EQ
#define TOK_NEQ TOKI_NEQ
#define TOK_LT TOKI_LT
#define TOK_GT TOKI_GT
#define TOK_LT_EQ TOKI_LT_EQ
#define TOK_GT_EQ TOKI_GT_EQ
#define TOK_CNT TOKI_CNT
static i32 ishexnum(char c) {
    switch (c) {
        case '0' ... '9':
        case 'A' ... 'F':
        case 'a' ... 'f':
            return 1;
        default:
            return 0;
    }
}

static i32 hexcodetoint(char c) {
    switch (c) {
        case '0' ... '9':
            return c - 0x30;
        case 'a' ... 'f':
            return c - 0x57;
        case 'A' ... 'F':
            return c - 0x37;
        default:
            return 0;
    }
}

static void get_tok_rdpi(RDPICTXT *ctx) {
    char var_name_buf[256];
    i32 var_name_pos;
    i32 ret;

    while (true) {
        if (ctx->buf[ctx->pos] == '\0') {
            ctx->cur_tok = TOK_END;

            return;
        }

        if (ctx->buf[ctx->pos] == ' ') {
            ctx->buf[ctx->pos]++;
            continue;
        }

        if (ctx->buf[ctx->pos] == '\t') {
            ctx->buf[ctx->pos]++;
            continue;
        }

        break;
    }

    switch (ctx->buf[ctx->pos]) {
        case ';':
            ctx->cur_tok = TOK_END;
            break;
        case '+':
            ctx->cur_tok = TOK_ADD;
            ctx->pos++;
            break;
        case '-':
            ctx->cur_tok = TOK_SUB;
            ctx->pos++;
            break;
        case '*':
            ctx->cur_tok = TOK_MUL;
            ctx->pos++;
            break;
        case '/':
            ctx->cur_tok = TOK_DIV;
            ctx->pos++;
            break;
        case '%':
            ctx->cur_tok = TOK_MOD;
            ctx->pos++;
            break;
        case '(':
            ctx->cur_tok = TOK_LPAREN;
            ctx->pos++;
            break;
        case ')':
            ctx->cur_tok = TOK_RPAREN;
            ctx->pos++;
            break;
        case '|':
            ctx->cur_tok = TOK_OR;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '|') {
                ctx->cur_tok = TOK_LOGICAL_OR;
                ctx->pos++;
            }
            return;
        case '&':
            ctx->cur_tok = TOK_AND;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '&') {
                ctx->cur_tok = TOK_LOGICAL_AND;
                ctx->pos++;
            }
            return;
        case '=':
            ctx->cur_tok = TOK_EQ;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '=') {
                ctx->pos++;
            }
            return;
        case '>':
            ctx->cur_tok = TOK_GT;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '=') {
                ctx->pos++;
                ctx->cur_tok = TOK_GT_EQ;
            }
            return;
        case '<':
            ctx->cur_tok = TOK_LT;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '=') {
                ctx->pos++;
                ctx->cur_tok = TOK_LT_EQ;
            }
            return;
        case '^':
            ctx->cur_tok = TOK_XOR;
            ctx->pos++;
            break;
        case '!':
            ctx->cur_tok = TOK_LOGICAL_NOT;
            ctx->pos++;

            if (ctx->buf[ctx->pos] == '=') {
                ctx->cur_tok = TOK_NEQ;
                ctx->pos++;
            }
            return;
        case '~':
            ctx->cur_tok = TOK_NOT;
            ctx->pos++;
            break;
        case '$':
            ctx->pos++;
            ctx->cur_tok = TOK_NUM;
            ctx->cur_val = 0;

            while (ishexnum(ctx->buf[ctx->pos])) {
                ctx->cur_val <<= 4;
                ctx->cur_val += hexcodetoint(ctx->buf[ctx->pos]);
                ctx->pos++;
            }
            break;
        case '0' ... '9':
            ctx->cur_tok = TOK_NUM;
            ctx->cur_val = 0;

            while (isnum(ctx->buf[ctx->pos])) {
                ctx->cur_val *= 10;
                ctx->cur_val += ctx->buf[ctx->pos] - 0x30;
                ctx->pos++;
            }
            break;
        case 'A' ... 'Z':
        case 'a' ... 'z':
            var_name_pos = 0;

            while (NuIsAlNum(ctx->buf[ctx->pos])) {
                var_name_buf[var_name_pos] = ctx->buf[ctx->pos];
                ctx->pos++;
                var_name_pos++;
            }

            var_name_buf[var_name_pos] = '\0';

            ctx->cur_tok = TOK_NUM;
            ctx->cur_val = 0;

            ret = (*ctx->get_var_fn)(var_name_buf, NULL, &ctx->cur_val);
            break;
        default:
            break;
    }
}

static i32 prim(RDPICTXT *ctx) {
    i32 expr;

    switch (ctx->cur_tok) {
        case TOK_NUM:
            get_tok_rdpi(ctx);

            return ctx->cur_val;
        case TOK_SUB:
            get_tok_rdpi(ctx);

            return -prim(ctx);
        case TOK_NOT:
            get_tok_rdpi(ctx);

            return ~prim(ctx);
        case TOK_LOGICAL_NOT:
            get_tok_rdpi(ctx);

            return !prim(ctx);
        case TOK_LPAREN:
            get_tok_rdpi(ctx);
            expr = i_expr(ctx);
            get_tok_rdpi(ctx);

            return expr;
        case TOK_END:
            return 1;
        default:
            return 0;
    }
}

static i32 term(RDPICTXT *ctx) {
    i32 term = prim(ctx);

    while (true) {
        switch (ctx->cur_tok) {
            case TOK_MUL:
                get_tok_rdpi(ctx);

                term *= prim(ctx);
                break;
            case TOK_DIV:
                get_tok_rdpi(ctx);

                term /= prim(ctx);
                break;
            case TOK_MOD:
                get_tok_rdpi(ctx);

                term %= prim(ctx);
                break;
            case TOK_EQ:
                get_tok_rdpi(ctx);

                term = term == prim(ctx);
                break;
            case TOK_NEQ:
                get_tok_rdpi(ctx);

                term = term != prim(ctx);
                break;
            case TOK_LT:
                get_tok_rdpi(ctx);

                term = term < prim(ctx);
                break;
            case TOK_GT:
                get_tok_rdpi(ctx);

                term = term > prim(ctx);
                break;
            case TOK_LT_EQ:
                get_tok_rdpi(ctx);

                term = term <= prim(ctx);
                break;
            case TOK_GT_EQ:
                get_tok_rdpi(ctx);

                term = term >= prim(ctx);
                break;
            default:
                return term;
        }
    }
}

static i32 i_expr(RDPICTXT *ctx) {
    i32 rhs;
    i32 expr = term(ctx);

    while (true) {
        switch (ctx->cur_tok) {
            case TOK_ADD:
                get_tok_rdpi(ctx);

                expr += term(ctx);
                break;
            case TOK_SUB:
                get_tok_rdpi(ctx);

                expr -= term(ctx);
                break;
            case TOK_AND:
                get_tok_rdpi(ctx);

                expr &= term(ctx);
                break;
            case TOK_OR:
                get_tok_rdpi(ctx);

                expr |= term(ctx);
                break;
            case TOK_XOR:
                get_tok_rdpi(ctx);

                expr ^= term(ctx);
                break;
            case TOK_LOGICAL_AND:
                get_tok_rdpi(ctx);

                rhs = term(ctx);
                expr = expr != 0 && rhs != 0;
                break;
            case TOK_LOGICAL_OR:
                get_tok_rdpi(ctx);

                rhs = term(ctx);
                expr = expr != 0 || rhs != 0;
                break;
            default:
                return expr;
        }
    }
}

i32 NuRDPIVar(char *input, nurdpgetvarfn *get_var_fn) {
    RDPICTXT ctx;
    i32 ret;

    ctx.buf = input;
    ctx.pos = 0;
    ctx.get_var_fn = get_var_fn;

    get_tok_rdpi(&ctx);

    ret = i_expr(&ctx);

    return ret;
}
#undef TOK_ADD
#undef TOK_SUB
#undef TOK_MUL
#undef TOK_DIV
#undef TOK_MOD
#undef TOK_LPAREN
#undef TOK_RPAREN
#undef TOK_END
#undef TOK_NUM
#undef TOK_OR
#undef TOK_AND
#undef TOK_NOT
#undef TOK_XOR
#undef TOK_LOGICAL_OR
#undef TOK_LOGICAL_AND
#undef TOK_LOGICAL_NOT
#undef TOK_EQ
#undef TOK_NEQ
#undef TOK_LT
#undef TOK_GT
#undef TOK_LT_EQ
#undef TOK_GT_EQ
#undef TOK_CNT

i32 NuRDPI(char *input) {
    return NuRDPIVar(input, NULL);
}

extern "C" {
    void NuSetErrorMsgHandler(void (*handler)(char *)) {
        nuerror_handler = handler;
    }

    void NuSetWarningMsgHandler(void (*handler)(char *)) {
        nuwarning_handler = handler;
    }

    void NuSetDebugMsgHandler(void (*handler)(char *)) {
        nudebug_handler = handler;
    }

}

extern "C" {
extern "C++" {
        static void NuErrorFunction(char *format, ...) {
            sprintf(captxt, "NuError - %s(%d) : ", nufile, nuline);
            va_list args;
            va_start(args, format);
            vsprintf(txt, format, args);
            va_end(args);
            NuStrCat(captxt, txt);
            NuStrCat(captxt, "\n");
            if (nuerror_handler != NULL) {
                nuerror_handler(captxt);
            }
            NuErrorPrint(captxt);
        }

}
}

extern "C" {
    NuErrorFunctionPtr NuErrorProlog(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuErrorFunction;
    }

}

extern "C" {
extern "C++" {
        static void NuWarningFunction(char *format, ...) {
            if (!DisableDebugMsg) {
                sprintf(captxt, "NuWarning - %s(%d) : ", nufile, nuline);
                va_list args;
                va_start(args, format);
                vsprintf(txt, format, args);
                va_end(args);
                NuStrCat(captxt, txt);
                NuStrCat(captxt, "\n");
                if (nuwarning_handler != NULL)
                    nuwarning_handler(captxt);
                NuWarningPrint(captxt);
            }
        }

}
}

extern "C" {
    NuErrorFunctionPtr NuWarningProlog(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuWarningFunction;
    }

}

extern "C" {
extern "C++" {
        static void NuDebugMsgFunction(char *format, ...) {
            char captxt[16384];
            if (!DisableDebugMsg) {
                if (err_msg_filter[0] == 0 || strstr(nufile, err_msg_filter) != NULL) {
                    sprintf(captxt, "NuDebugMsg - %s(%d) :", nufile, nuline);
                    va_list args;
                    va_start(args, format);
                    vsprintf(txt, format, args);
                    va_end(args);
                    NuStrCat(captxt, txt);
                    NuStrCat(captxt, "\n");
                    if (nudebug_handler != NULL)
                        nudebug_handler(captxt);
                    NuDebugMsgPrint(captxt);
                }
            }
        }

        static void NuDebugMsgFunctionTTY(i32 channel, char *format, ...) {
            if (!DisableDebugMsg) {
                // The original Android build has no TTY output body.
                return;
            }
        }

}
}

extern "C" {
    NuErrorFunctionPtr NuDebugMsgProlog(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuDebugMsgFunction;
    }

    NuDebugTTYFunctionPtr NuDebugMsgPrologTTY(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuDebugMsgFunctionTTY;
    }

    void NuErrorCheck(void) {
        if (nuerror_status) {
            // The original Android build has no error-reporting body here.
        }
    }

    void NuErrorSetFilter(char *filter) {
        if (filter == NULL) {
            NuStrCpy(err_msg_filter, "");
        } else {
            NuStrNCpy(err_msg_filter, filter, 32);
        }
    }

    char *NuGetErrN(i32 entry) {
        i32 offset = 0;
        i32 current = 0;
        while (current < entry && offset < bHaveErr) {
            if (*(ErrMsg + offset++) == '\0') {
                ++current;
            }
        }
        return ErrMsg + offset;
    }

    char *NuGetError(void) {
        return bHaveErr != 0 ? ErrMsg : NULL;
    }

    void NuSevereWarning(const char *format, ...) {
        sprintf(captxt, " ");
        va_list args;
        va_start(args, format);
        vsprintf(txt, format, args);
        va_end(args);
        NuStrCat(captxt, txt);
        NuStrCat(captxt, "\n");
        if (nudebug_handler != NULL) {
            nudebug_handler(captxt);
        }
        NuDebugMsgPrint(captxt);
        char *message = captxt;
        while (*message != '\0' && bHaveErr < 1023) {
            ErrMsg[bHaveErr++] = *message++;
        }
        ErrMsg[bHaveErr++] = '\0';
    }

    i32 NuHasError(void) {
        return bHaveErr;
    }

    void NuClearError(void) {
        bHaveErr = 0;
    }

}

void ImplodeError(char *msg, ...) {
    va_list args;
    va_start(args, msg);

    putc('\n', stderr);
    vfprintf(stderr, msg, args);
    putc('\n', stderr);

    exit(1);
}

i32 ImplodeGetI(void *buf, i32 size) {
    unsigned char *char_ptr;
    u32 buf_reversed;

    char_ptr = (unsigned char *)buf;
    buf_reversed = *char_ptr | *(char_ptr + 1) << 0x8 | *(char_ptr + 2) << 0x10 | *(char_ptr + 3) << 0x18;

    return buf_reversed & getmasktbl[size];
}

void *ImplodePutI(void *buf, u32 value, i32 size) {
    unsigned char *cursor = static_cast<unsigned char *>(buf);
    while (size != 0) {
        i32 byte = value;
        *cursor = byte;
        ++cursor;
        value >>= 8;
        --size;
    }
    return buf;
}

unsigned char ImplodeGetByteFromMem() {
    return *implode_inbuffer++;
}

void ImplodePutByteToMem(unsigned char value) {
    *implode_outbuffer++ = value;
}

void ImplodeFillBuf(i32 count) {
    implode_bitbuf <<= count;

    while (bitcount < count) {
        implode_bitbuf |= subbitbuf << (count -= bitcount);

        if (implode_compsize != 0) {
            implode_compsize--;

            subbitbuf = ImplodeGetByteFromMem() & 0xff;
        } else {
            subbitbuf = 0;
        }

        bitcount = 8;
    }

    implode_bitbuf |= subbitbuf >> (bitcount -= count);
}

u32 ImplodeGetBits(i32 count) {
    u32 value;

    if (count == 0) {
        return 0;
    }

    value = implode_bitbuf >> (0x20u - count);

    ImplodeFillBuf(count);

    return value;
}

usize ImplodeFReadMem(unsigned char *buffer, i32 size) {
    i32 count = size < inlen ? size : inlen;
    memcpy(buffer, implode_inbuffer, count);
    implode_inbuffer += count;
    implode_origsize += count;
    inlen -= count;
    return count;
}

void ImplodeInitGetBits() {
    implode_bitbuf = 0;
    subbitbuf = 0;
    bitcount = 0;

    ImplodeFillBuf(0x20);
}

extern "C" {
    void ExplodeExit(void) {
        if (gExplodeInitialised != 0)
            gExplodeInitialised = 0;
    }

    isize ExplodeBuffer(char *in_buf, char *out_buf) {
        if (gExplodeInitialised == 0)
            return 0;

        isize orig_size = ExplodeBufferSize(in_buf);
        if (orig_size != 0) {
            isize compressed_size = ImplodeGetI(in_buf + 8, sizeof(i32));
            return ExplodeBufferNoHeader(in_buf + 12, out_buf, compressed_size, orig_size);
        }
        return 0;
    }

}

isize ExplodeBufferNoHeader(char *in_buf, char *out_buf, isize compressed_size, isize orig_size) {
    i32 total_read;
    u32 read_size;

    if (gExplodeInitialised == 0) {
        return 0;
    }

    implode_inbuffer = in_buf;
    implode_outbuffer = out_buf;
    implode_origsize = orig_size;
    implode_compsize = compressed_size;

    decode_start();

    total_read = 0;
    while (implode_origsize != 0) {
        read_size = MIN(implode_origsize, 0x2000);

        decode(read_size, buffer);

        memcpy(implode_outbuffer, buffer, read_size);

        implode_outbuffer += read_size;
        implode_origsize -= read_size;
        total_read += read_size;
    }

    return total_read;
}

isize ExplodeBufferSize(char *buf) {
    char magic[] = "LZ2K";

    for (i32 i = 0; i < 4; i++) {
        i32 does_not_match;
        if ((does_not_match = *(buf++) != magic[i])) {
            return 0;
        }
    }

    i32 size = ImplodeGetI(buf, sizeof(i32));
    return size;
}

isize ExplodeCompressedSize(char *buf) {
    char magic[] = "LZ2K";

    for (i32 i = 0; i < 4; i++) {
        i32 does_not_match;
        if ((does_not_match = *(buf++) != magic[i])) {
            return 0;
        }
    }

    buf += 4;
    i32 size = ImplodeGetI(buf, sizeof(i32));
    if (size != 0) {
        size += 0xc;
    }

    return size;
}

static void decode_start() {
    ImplodeHufDecodeStart();

    bytes_to_copy = 0;
}

static void decode(u32 size, unsigned char *buf) {
    static u32 i;

    u32 n;

    n = 0;
    u32 c;

    while (--bytes_to_copy >= 0) {
        buf[n] = buf[i];
        i = (i + 1) & 0x1fff;

        if (++n == size) {
            return;
        }
    }

    while (true) {
        c = ImplodeDecodeC();

        if (c < 0x100) {
            buf[n] = c;

            if (++n == size) {
                return;
            }
        } else {
            bytes_to_copy = c - 0xfd;
            i = (n - ImplodeDecodeOffset() - 1) & 0x1fff;

            while (--bytes_to_copy >= 0) {
                buf[n] = buf[i];
                i = (i + 1) & 0x1fff;

                if (++n == size) {
                    return;
                }
            }
        }
    }
}

static void read_offset_len(i32 table_size, i32 bit_count, i32 zero_value) {
    i32 n;
    i32 i;
    i32 value;

    n = ImplodeGetBits(bit_count);
    if (n == 0) {
        value = ImplodeGetBits(bit_count);

        for (i = 0; i < table_size; i++) {
            offset_len[i] = 0;
        }

        for (i = 0; i < 0x100; i++) {
            offset_table[i] = value;
        }

        return;
    }

    i = 0;
    while (i < n) {
        value = implode_bitbuf >> 0x1d;

        if (value == 7) {
            for (u32 k = 0x10000000; (implode_bitbuf & k) != 0; k >>= 1, value++) {
            }
        }

        ImplodeFillBuf(value >= 7 ? value - 3 : 3);

        offset_len[i] = value;
        i++;

        if (i == zero_value) {
            value = ImplodeGetBits(2);

            while (--value >= 0) {
                offset_len[i] = 0;
                i++;
            }
        }
    }

    for (; i < table_size; i++) {
        offset_len[i] = 0;
    }

    ImplodeMakeTable(table_size, offset_len, 8, offset_table);
}

static void read_literal_len() {
    i32 n;
    i32 i;
    i32 value;

    n = ImplodeGetBits(9);
    if (n == 0) {
        value = ImplodeGetBits(9);

        for (i = 0; i < sizeof(literal_len); i++) {
            literal_len[i] = 0;
        }

        for (i = 0; i < 0x1000; i++) {
            literal_table[i] = value;
        }

        return;
    }

    i = 0;
    while (i < n) {
        SELECT(value, offset_table, 0x18, 0x12, 0x800000, offset_len);

        if (value < 3) {
            if (value == 0) {
                value = 1;
            } else if (value == 1) {
                value = ImplodeGetBits(4) + 0x3;
            } else {
                value = ImplodeGetBits(9) + 0x14;
            }

            while (--value >= 0) {
                literal_len[i] = 0;
                i++;
            }
        } else {
            literal_len[i] = value - 2;
            i++;
        }
    }

    for (; i < sizeof(literal_len); i++) {
        literal_len[i] = 0;
    }

    ImplodeMakeTable(sizeof(literal_len), literal_len, 0xc, literal_table);
}

u32 ImplodeDecodeC() {
    u32 value;

    if (blocksize == 0) {
        blocksize = ImplodeGetBits(0x10);

        read_offset_len(0x13, 5, 3);
        read_literal_len();
        read_offset_len(0xe, 4, -1);
    }

    blocksize--;

    SELECT(value, literal_table, 0x14, sizeof(literal_len) - 1, 0x80000, literal_len);

    return value;
}

u32 ImplodeDecodeOffset() {
    u32 offset;

    SELECT(offset, offset_table, 0x18, 0xd, 0x800000, offset_len);

    if (offset != 0) {
        offset = (1u << (offset - 1)) + ImplodeGetBits(offset - 1);
    }

    return offset;
}

void ImplodeHufDecodeStart() {
    ImplodeInitGetBits();

    blocksize = 0;
}

void ImplodeMakeTable(i32 size, u8 *len, i32 unknown, u16 *table) {
    u16 symbol_count[17];
    u16 masks[17];
    u16 offsets[18];
    u32 symbol;
    i32 shift;
    u32 length;
    u32 i;
    u32 j;
    u32 k;
    u32 unknown2;
    u32 unknown3;
    u32 unknown4;
    u16 *cursor;

    for (i = 1; i < 0x11; i++) {
        symbol_count[i] = 0;
    }

    for (i = 0; i < size; i++) {
        symbol_count[len[i]]++;
    }

    offsets[1] = 0;

    for (i = 1; i < 0x11; i++) {
        offsets[i + 1] = offsets[i] + (symbol_count[i] << (0x10 - i));
    }

    if (offsets[0x11] != 0) {
        ImplodeError("Bad table");
    }

    shift = 0x10 - unknown;

    for (i = 1; i <= unknown; i++) {
        offsets[i] >>= shift;
        masks[i] = 1 << (unknown - i);
    }

    for (; i < 0x11; i++) {
        masks[i] = 1 << (0x10 - i);
    }

    i = (i32)offsets[unknown + 1] >> shift;
    if (i != 0) {
        for (j = 1 << unknown; i != j; i++) {
            table[i] = 0;
        }
    }

    unknown2 = size;
    unknown3 = 1 << (0xf - unknown);

    for (symbol = 0; symbol < size; symbol++) {
        if ((length = len[symbol]) == 0) {
            continue;
        }

        unknown4 = offsets[length] + masks[length];
        if (unknown >= length) {
            for (i = offsets[length]; i < unknown4; i++) {
                table[i] = symbol;
            }
        } else {
            j = offsets[length];
            cursor = table + (j >> shift);

            for (k = length - unknown; k != 0; k--) {
                if (*cursor == 0) {
                    implode_left[unknown2] = 0;
                    implode_right[unknown2] = implode_left[unknown2];
                    *cursor = unknown2;

                    unknown2++;
                }

                if ((j & unknown3) != 0) {
                    cursor = implode_right + *cursor;
                } else {
                    cursor = implode_left + *cursor;
                }

                j <<= 1;
            }

            *cursor = symbol;
        }

        offsets[length] = unknown4;
    }
}

static void count_len(i32 node) {
    static i32 depth;
    if (node < n) {
        ++len_cnt[depth < 16 ? depth : 16];
    } else {
        ++depth;
        count_len(implode_left[node]);
        count_len(implode_right[node]);
        --depth;
    }
}

static void make_len(i32 root) {
    i32 i;
    u32 count;
    u32 total;
    for (i = 0; i <= 16; ++i)
        len_cnt[i] = 0;
    count_len(root);
    total = 0;
    for (i = 16; i > 0; --i)
        total += static_cast<u32>(len_cnt[i]) << (16 - i);
    while (total != 65536) {
        --len_cnt[16];
        for (i = 15; i > 0; --i) {
            if (len_cnt[i] != 0) {
                --len_cnt[i];
                len_cnt[i + 1] += 2;
                break;
            }
        }
        --total;
    }
    for (i = 16; i > 0; --i) {
        count = len_cnt[i];
        while (static_cast<i32>(--count) >= 0)
            len[*sortptr++] = static_cast<unsigned char>(i);
    }
}

static void downheap(i32 parent) {
    i32 child;
    i32 node = heap[parent];
    while ((child = parent * 2) <= heapsize) {
        if (child < heapsize && freq[heap[child]] > freq[heap[child + 1]])
            ++child;
        if (freq[node] <= freq[heap[child]])
            break;
        heap[parent] = heap[child];
        parent = child;
    }
    heap[parent] = node;
}

static void make_code(i32 symbols, unsigned char *lengths, u16 *codes) {
    u16 start[18];
    i32 i;
    // Zero-length symbols are unused, but the original still assigns a code.
    // Initialize their otherwise indeterminate running counter.
    start[0] = 0;
    start[1] = 0;
    for (i = 1; i <= 16; ++i)
        start[i + 1] = (start[i] + len_cnt[i]) << 1;
    for (i = 0; i < symbols; ++i)
        codes[i] = start[lengths[i]]++;
}

i32 ImplodeMakeTree(i32 symbols, u16 *frequencies, unsigned char *lengths, u16 *codes) {
    i32 i, next_node, j, root;
    n = symbols;
    freq = frequencies;
    len = lengths;
    next_node = n;
    heapsize = 0;
    heap[1] = 0;
    for (i = 0; i < n; ++i) {
        len[i] = 0;
        if (freq[i] != 0)
            heap[++heapsize] = static_cast<i16>(i);
    }
    if (heapsize < 2) {
        codes[heap[1]] = 0;
        return heap[1];
    }
    for (i = heapsize / 2; i > 0; --i)
        downheap(i);
    sortptr = codes;
    do {
        i = heap[1];
        if (i < n)
            *sortptr++ = static_cast<u16>(i);
        heap[1] = heap[heapsize--];
        downheap(1);
        j = heap[1];
        if (j < n)
            *sortptr++ = static_cast<u16>(j);
        root = next_node++;
        freq[root] = freq[i] + freq[j];
        heap[1] = static_cast<i16>(root);
        downheap(1);
        implode_left[root] = static_cast<u16>(i);
        implode_right[root] = static_cast<u16>(j);
    } while (heapsize > 1);
    sortptr = codes;
    make_len(root);
    make_code(symbols, lengths, codes);
    return root;
}

