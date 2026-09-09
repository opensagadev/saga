#include "nu2api/nucore/nustring.h"
#include <stdio.h>
#include <string.h>

i32 global_tr;

void NuStrTrap(const char *text, const char *match) {
    if (NuStrCmp(text, match) == 0) {
        global_tr++;
    }
}

i32 NuVSPrintf(char *dest, const char *format, va_list args) {
    return vsprintf(dest, format, args);
}

i32 NuSPrintfW(NUWCHAR *dest, NUWCHAR *format, ...) {
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

#include "decomp.h"
#include "nu2api/nufile/nufile.h"
#include "nu2api/nufile/nufpar.h"
#include "nu2api/nuplatform/nuplatform.h"
#include <stdarg.h>

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

void NuStrToLower(char *str) {
    char *cursor = str;
    while (*cursor != 0) {
        *cursor = NuToLower(*cursor);
        ++cursor;
    }
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

NUWCHAR *NuStrChrW(NUWCHAR *str, NUWCHAR c) {
    while (*str != 0) {
        if (*str == c)
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

void NuStrLwr(char *dst, const char *src) {
    while (*src != '\0') {
        *dst = NuToLower(*src);
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

void NuStrUprW(NUWCHAR *dst, const NUWCHAR *src) {
    while (*src != 0) {
        *dst = NuToUpperW(*src);
        ++dst;
        ++src;
    }
    *dst = *src;
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

NUWCHAR8 *NuUnicodeCharFromUTF8(NUWCHAR16 *dst, NUWCHAR8 *src) {
    NUWCHAR16 c0 = *src++;
    if (c0 <= 0x7f) {
        *dst = c0;
    } else if ((c0 & 0xe0) == 0xc0) {
        NUWCHAR16 c1 = *src++;
        *dst = ((c0 & 0x1f) << 6) | (c1 & 0x3f);
    } else if ((c0 & 0xf0) == 0xe0) {
        NUWCHAR16 c1 = *src++;
        NUWCHAR16 c2 = *src++;
        *dst = (c0 << 12) | ((c1 & 0x3f) << 6) | (c2 & 0x3f);
    }
    return src;
}

NUWCHAR8 *NuUTF8CharFromUnicode(NUWCHAR8 *dst, NUWCHAR16 character) {
    if (character <= 0x7f) {
        *dst++ = character;
    } else if (character <= 0x7ff) {
        *dst++ = (character >> 6) | 0xc0;
        *dst++ = (character & 0x3f) | 0x80;
    } else {
        *dst++ = (character >> 12) | 0xe0;
        *dst++ = ((character >> 6) & 0x3f) | 0x80;
        *dst++ = (character & 0x3f) | 0x80;
    }
    return dst;
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

char *NuStrChr(char *src, char c) {
    while (*src != '\0') {
        if (*src == c) {
            return src;
        }

        src++;
    }

    return NULL;
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

void NuStrFixExt(char *dst, char *src, char *ext, i32 dst_size) {
    if (src == NULL || ext == NULL || dst == NULL)
        return;
    NuStrFixExtPlatform(dst, src, ext, dst_size, g_platformName);
    return;
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

i32 NuStrLen(const char *str) {
    i32 count = 0;

    while (*str != '\0') {
        count++;
        str++;
    }

    return count;
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

u8 NuToLower(u8 c) {
    if (c >= 'A' && c <= 'Z') {
        c += 0x20;
    } else if (c > 0xbf && c < 0xe0) {
        c += 0x20;
    }

    return c;
}

u8 NuToUpper(u8 c) {
    if (c >= 'a' && c <= 'z') {
        c -= 0x20;
    } else if (c > 0xdf) {
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

NUWCHAR NuToUpperW(NUWCHAR c) {
    if (c >= 0x61 && c <= 0x7a) {
        c -= 0x20;
    } else if (c > 0xdf && c < 0x100) {
        c -= 0x20;
    }

    return c;
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

void NuStrUpr(char *dst, const char *src) {
    for (; *src != '\0'; src++) {
        *dst = NuToUpper(*src);
        dst++;
    }

    *dst = *src;
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

i32 NuStrLenW(const NUWCHAR *str) {
    i32 count = 0;

    while (*str != 0) {
        count++;
        str++;
    }

    return count;
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

static NUSTRINGBANK StringBank[3];
static i16 CurrentStringBank;
static i32 nustring_format = 1;
char *NuBlankString = " ";

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
    NUSTRING *strings = NULL;
    NUSTRINGBANK *bank = &StringBank[CurrentStringBank];
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

i32 NuStringTableLoad(char *filepath, VARIPTR *buf, VARIPTR buf_end) {
    i32 is_csv = 0;
    char *extension = NuStrRChr(filepath, '.');
    if (extension != NULL) {
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

static u16 bad_equiv[][2] = {{'a', '@'}, {'a', '4'}, {'e', '3'}, {'i', '1'},
                             {'l', '1'}, {'o', '0'}, {'t', '7'}, {0, 0}};
static u16 bad_fluff[] = {'!', 0xffa3, '$', '%', '^', '&', '*', '(', ')', '_', '+', '-', '=', '{',
                          '}', '[',    ']', '@', '~', '#', '?', '<', '>', ',', '.', ' ', 0};
static NUWCHAR **BadWords;
static i32 NumBadWords;

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

void NuStringTableSetBank(i32 bank) {
    if (bank >= 0 && bank < 3) {
        CurrentStringBank = bank;
    }
}

void NuStringTableSetFormat(i32 format) {
    nustring_format = format;
}

i32 NuStringTableGetFormat(void) {
    return nustring_format;
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
