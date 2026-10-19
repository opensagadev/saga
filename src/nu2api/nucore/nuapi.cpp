#include "nu2api/nucore/nuapi.h"

#include <stdarg.h>
#include <string.h>
#include "decomp.h"
#include "nu2api/nu3d/nudlist.h"
#include "nu2api/nu3d/nurndr.h"
#include "nu2api/nu3d/nutex.h"
#include "nu2api/nucore/nuanim3.h"

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

// Frame-end hooks (original bss @0x6bdaec / 0x6bdaf0 / 0x6bdad0).
void (*preRenderFlashingHack)(void) = NULL;
void (*postRenderFlashingHack)(void) = NULL;
void (*nuapi_endframe_callbackfn)(void) = NULL;

// The Android command-line provider is empty. These are still the original
// private cursors consumed by ParseCommandLine, not host process arguments.
static i32 argc;
static char **argv;

void NuCommandLine(i32 *argc, char ***argv) {
}

