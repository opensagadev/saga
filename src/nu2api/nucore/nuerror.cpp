#include "nu2api/nucore/nustring.h"
#include <stdio.h>
#include <stdarg.h>
void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
extern "C" {
    typedef void (*NuErrorFunctionPtr)(char *, ...);
    static i32 bHaveErr;
    static char ErrMsg[1024];
    static char *nufile;
    static i32 nuline;
    static char txt[16384];
    static char captxt[16384];
    void (*nuerror_handler)(char *);
    void (*nudebug_handler)(char *);

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

    void NuClearError(void) {
        bHaveErr = 0;
    }
    NuErrorFunctionPtr NuErrorProlog(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuErrorFunction;
    }
    i32 NuHasError(void) {
        return bHaveErr;
    }
    char *NuGetErrN(i32 entry) {
        i32 offset = 0;
        i32 current = 0;
        while (current < entry && offset < bHaveErr) {
            if (ErrMsg[offset++] == '\0') {
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
}
