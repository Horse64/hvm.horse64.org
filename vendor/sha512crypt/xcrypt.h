
// File written/added to fix build for HVM,
// consider this file public domain.

#include <string.h>


#if !defined(_WIN32) && !defined(_WIN64)
static void *mempcpy(void *dest, const void *src, size_t n) {
    memcpy(dest, src, n);
    return ((char *)dest) + n;
}
#else
#ifndef __USE_MINGW_ANSI_STDIO
#define __USE_MINGW_ANSI_STDIO 1
#endif
static char *stpncpy(char *dest, const char *src, size_t n) {
    size_t srclen = strlen(src);
    if (srclen >= n) {
        memcpy(dest, src, n);
        return dest + n;
    }
    memcpy(dest, src, srclen);
    memset(dest + srclen, 0, n - srclen);
    return dest + srclen;
}
#endif

