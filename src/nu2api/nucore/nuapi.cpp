#include "nu2api/nucore/nuapi.h"

#include <stdarg.h>
#include <string.h>

extern "C" i32 Nu360GetCommandLine(char **arguments, i32 capacity) {
    i32 i;
    i32 count;
    i32 done;
    char *dest;
    char *source;
    // The Android original has no command-line provider, but retains this parser.
    char *command_line = NULL;
    i32 length;
    count = 0;
    if (command_line != NULL) {
        i = 0;
        done = 0;
        dest = arguments[0];
        source = command_line;
        if (*source == '\0') {
            return 0;
        }
        count = 0;
        length = strlen(source);
        while (i < length && !done) {
            if (*source == '\0' || *source == ' ') {
                if (*source == '\0') {
                    done = 1;
                    *dest = '\0';
                } else if (count >= capacity - 1) {
                    done = 1;
                    *dest = '\0';
                } else {
                    count++;
                    dest = arguments[count];
                }
            } else {
                *dest = *source;
                dest++;
            }
            source++;
            i++;
        }
        *dest = '\0';
    }
    return count + 1;
}

#include "decomp.h"

#include "nu2api/nu3d/numtl.h"
#include "nu2api/nu3d/nuprim.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nupostresources.h"
#include "nu2api/nu3d/nuvport.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nucore/nuanim3.h"
#include "nu2api/nucore/nustring.h"
#include "nu2api/numath/nutrig.h"
#include "nu2api/nu3d/nuqfnt.h"
#include "nu2api/nu3d/nuocclusion.h"
#include "nu2api/nu3d/android/nutimebar_plain.h"
#include "nu2api/nucore/nuonline.h"

// Nucore bootstrap helpers; display-list and framebuffer initialization are
// declared by their owning public headers.
extern "C" {
    void NuPostEffectInit(u32, void *, void *);
}

void NuRndrInitGeneric(void); // nurndr.cpp (C++ linkage)
void bgProcInit(void);        // bgproc_android.cpp

NUAPI nuapi;

i32 nuapi_use_target_manager;
char *nuapi_target_manager_mac_address;
i32 nuapi_deferspotfxload;
i32 nuapi_use_target_manager_for_sound;

// Frame-end hooks (original bss @0x6bdaec / 0x6bdaf0 / 0x6bdad0).
void (*preRenderFlashingHack)(void) = NULL;
void (*postRenderFlashingHack)(void) = NULL;
void (*nuapi_endframe_callbackfn)(void) = NULL;

float nuapi_forced_frame_time;
i32 nuapi_max_fps = 60;

static i32 NUAPI_PADREC_DEFAULT_BUFFERSIZE = 0x500000;
// The Android command-line provider is empty. These are still the original
// private cursors consumed by ParseCommandLine, not host process arguments.
static i32 argc;
static char **argv;

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

void NuCommandLine(i32 *argc, char ***argv) {
}

void NuDisableOSMenuFreeze(void) {
    nuapi.disable_os_menu_freeze = 1;
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

i32 NuInitHardware(VARIPTR *buf, VARIPTR *buf_end, i32 heap_size, ...) {
    static char *dfs_name = "";
    static char *sfxdfs_name = NULL;
    i32 texture_count = 3000;
    i32 material_count = 512;
    i32 hostfs = 0;
    i32 streamsize = 0x200000;
    NUPAD **pad0 = NULL, **pad1 = NULL, **pad2 = NULL, **pad3 = NULL;
    i32 video_mode_argument = 2;
    i32 audio_channels = 4, audio_buffer_size = 4096;
    i32 resolution_x = 0, resolution_y = 0;
    NUVIDEO_SWAPMODE swapmode = NUVIDEO_SWAPMODE_FIELDSYNC;
    u32 flags = 0, framebuffer_format = 0;
    u32 occlusion_capacity = 0;
    i32 animation_count = 160;

    NuAPIInit();
    ParseCommandLine();
    va_list args;
    va_start(args, heap_size);
    i32 setup_tok;
    do {
        setup_tok = va_arg(args, i32);
        switch (setup_tok) {
            case NUAPI_SETUP_END:
                break;
            case NUAPI_SETUP_HOSTFS:
                hostfs = va_arg(args, i32);
                break;
            case NUAPI_SETUP_STREAMSIZE:
                streamsize = va_arg(args, i32);
                break;
            case 9:
                va_arg(args, i32);
                va_arg(args, i32);
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
            case 0x16:
                dfs_name = va_arg(args, char *);
                break;
            case 0x19:
                sfxdfs_name = va_arg(args, char *);
                break;
            case 0x1a:
                texture_count = va_arg(args, i32);
                break;
            case NUAPI_SETUP_AUDIO_DISABLED:
                va_arg(args, i32);
                va_arg(args, i32);
                va_arg(args, i32);
                va_arg(args, i32);
                break;
            case 0x1e:
                audio_channels = va_arg(args, i32);
                audio_buffer_size = va_arg(args, i32);
                break;
            case NUAPI_SETUP_RESOLUTION:
                resolution_x = va_arg(args, i32);
                resolution_y = va_arg(args, i32);
                break;
            case NUAPI_SETUP_SWAPMODE:
                swapmode = static_cast<NUVIDEO_SWAPMODE>(va_arg(args, i32));
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
            case 0x45: {
                i32 format = va_arg(args, i32);
                if (format == 2)
                    framebuffer_format = 1;
                else if (format == 4)
                    framebuffer_format = 2;
                else if (format == 1)
                    framebuffer_format = 0;
                break;
            }
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
                occlusion_capacity = va_arg(args, u32);
                break;
            case 0x4d:
                va_arg(args, i32);
                break;
            case 0x52:
                NuPadSetMaxGamePads(va_arg(args, i32));
                break;
            default:
                if (NuInitHardwareParseArgsPS(setup_tok, &args) == 0) {
                    switch (setup_tok) {
                        case NUAPI_SETUP_CDDVDMODE:
                            va_arg(args, i32 *);
                            break;
                        case 6:
                        case NUAPI_SETUP_GLASSRPLANE:
                            va_arg(args, i32);
                            break;
                        case NUAPI_SETUP_AUDIO:
                            va_arg(args, i32);
                            va_arg(args, i32);
                            va_arg(args, i32);
                            va_arg(args, i32);
                            break;
                        case 0x14:
                            va_arg(args, i32);
                            va_arg(args, i32);
                            break;
                    }
                }
                break;
        }
    } while (setup_tok != NUAPI_SETUP_END);
    va_end(args);

    // The retained generic bootstrap selects mode 2; the legacy mode token is consumed but unused.
    NuVideoSetMode(2);
    NuVideoSetSwapMode(swapmode);
    NuVideoSetResolution(resolution_x, resolution_y);
    NuInitHardwarePS(buf, buf_end, heap_size);
    VARIPTR buffer_end = *buf_end;
    nuapi.video_aspect = NuVideoGetAspectPS();
    NuVideoSetBrightness(1.0f);
    NuFileInitEx(hostfs, 0, 0);
    NuTrigInit();
    NuRndrInitEx(streamsize, buf);
    NuPrimInit(buf, buffer_end);
    NuVpInit();
    NuTexInitEx(buf, texture_count);
    NuDisplayListInit(buf, buffer_end);
    u32 framebuffer_flags = (flags & 32) != 0 ? 3 : 1;
    if ((flags & 16) != 0)
        framebuffer_flags |= 4;
    if ((flags & 12) != 0)
        framebuffer_flags |= 8;
    NuFramebufferInitEx(framebuffer_flags, framebuffer_format, buf, buffer_end);
    NuPostEffectInit(flags | 1, buf, buffer_end.void_ptr);
    NuMtlInitEx(buf, material_count);
    NuRndrInitGeneric();
    NuAnimInit(animation_count, buf, buffer_end);
    if (dfs_name != NULL && *dfs_name != '\0')
        NuDatSet(NuDatOpen(dfs_name, buf, NULL));
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
