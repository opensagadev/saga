#include "nu2api/nucore/nustring.h"
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
void NuErrorPrint(char *);
void NuDebugMsgPrint(char *);
void NuWarningPrint(char *);
extern "C" {
    typedef void (*NuErrorFunctionPtr)(char *, ...);
    typedef void (*NuDebugTTYFunctionPtr)(i32, char *, ...);
    static i32 bHaveErr;
    static i32 nuerror_status;
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

    void NuErrorCheck(void) {
        if (nuerror_status) {
            // The original Android build has no error-reporting body here.
            return;
        }
    }

    void NuSetWarningMsgHandler(void (*handler)(char *)) {
        nuwarning_handler = handler;
    }

    void NuSetErrorMsgHandler(void (*handler)(char *)) {
        nuerror_handler = handler;
    }

    void NuSetDebugMsgHandler(void (*handler)(char *)) {
        nudebug_handler = handler;
    }

    void NuErrorSetFilter(char *filter) {
        if (filter == NULL) {
            NuStrCpy(err_msg_filter, "");
        } else {
            NuStrNCpy(err_msg_filter, filter, 32);
        }
    }

    extern "C++" {
        static void NuDebugMsgFunctionTTY(i32 channel, char *format, ...) {
            if (!DisableDebugMsg) {
                // The original Android build has no TTY output body.
                return;
            }
        }

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
    NuErrorFunctionPtr NuWarningProlog(char *file, i32 line) {
        nufile = file;
        nuline = line;
        return NuWarningFunction;
    }
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
