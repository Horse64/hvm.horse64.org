/* Copyright (c) 2020-2023, ellie/@ell1e & HVM Team (see AUTHORS.md).

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:

1. Redistributions of source code must retain the above copyright notice,
   this list of conditions and the following disclaimer.
2. Redistributions in binary form must reproduce the above copyright notice,
   this list of conditions and the following disclaimer in the documentation
   and/or other materials provided with the distribution.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
POSSIBILITY OF SUCH DAMAGE.

Alternatively, at your option, this file is offered under the Apache 2
license, see accompanied LICENSE.md.
*/


#include "compileconfig.h"

#include <assert.h>
#if !defined(_WIN32) && !defined(_WIN64)
#include <signal.h>
#endif
#include <stdio.h>
#include <string.h>
#if defined(_WIN32) || defined(_WIN64)
#define _O_RDONLY 0x0000
#define _O_WRONLY 0x0001
#define _O_TEXT 0x4000
#include <windows.h>
#if defined(HVM_USE_SDL)
#include <SDL2/SDL.h>  // for SDL_main()
#endif
#endif

#include "hasm/main.h"

int main_consolewasspawned = 0;
_Atomic volatile int main_gotsigintsigterm = 0;

#if !defined(_WIN32) && !defined(_WIN64)
void sigint_handler(int signo) {
    assert(signo == SIGINT);
    main_gotsigintsigterm = 1;
}
#endif

#if !defined(_WIN32) && !defined(_WIN64)
void sigterm_handler(int signo) {
    assert(signo == SIGTERM);
    main_gotsigintsigterm = 1;
}
#endif

#if defined(_WIN32) || defined(_WIN64)
static int main_havewinconsole = 0;
static int main_triedconsolealloc = 0;
#endif

int main_WinEnsureStdoutStderr() {
    #if !defined(_WIN32) && !defined(_WIN64)
    return 1;
    #else
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    int fd = _open_osfhandle((intptr_t)handle, _O_WRONLY | _O_TEXT);
    if (fd < 0) {
        CloseHandle(handle);
        return 0;
    }
    FILE *fresult = _fdopen(fd, "w");
    if (!fresult) {
        _close(fd);
        return 0;
    }
    *stdout = *fresult;
    handle = GetStdHandle(STD_ERROR_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    fd = _open_osfhandle((intptr_t)handle, _O_WRONLY | _O_TEXT);
    if (fd < 0) {
        CloseHandle(handle);
        return 0;
    }
    fresult = _fdopen(fd, "w");
    if (!fresult) {
        _close(fd);
        return 0;
    }
    *stderr = *fresult;
    handle = GetStdHandle(STD_INPUT_HANDLE);
    if (handle == INVALID_HANDLE_VALUE)
        return 0;
    fd = _open_osfhandle((intptr_t)handle, _O_RDONLY | _O_TEXT);
    if (fd < 0) {
        CloseHandle(handle);
        return 0;
    }
    fresult = _fdopen(fd, "r");
    if (!fresult) {
        _close(fd);
        return 0;
    }
    *stdin = *fresult;

    setvbuf(stdin, (char *)NULL, _IONBF, 0);
    fflush(stdin);
    setvbuf(stderr, (char *)NULL, _IONBF, 0);
    fflush(stderr);
    setvbuf(stdout, (char *)NULL, _IONBF, 0);
    fflush(stdout);
    return 1;
    #endif
}

int main_EnsureConsole() {
    #if !defined(_WIN32) && !defined(_WIN64)
    return 1;
    #else
    if (main_havewinconsole)
        return 1;
    if (main_triedconsolealloc)
        return 0;
    main_triedconsolealloc = 1;
    if (AllocConsole()) {
        main_WinEnsureStdoutStderr();
        main_consolewasspawned = 1;
        main_havewinconsole = 1;
    }
    return main_havewinconsole;
    #endif
}

int main_ConsoleWasSpawned() {
    #if !defined(_WIN32) && !defined(_WIN64)
    return 0;
    #else
    return main_consolewasspawned;
    #endif
}

int main(int argc, char **argv) {
    #if defined(DEBUG_STARTUP)
    printf("src/main.c: debug: Entering main(), let's go!\n");
    #endif
    #if !defined(_WIN32) && !defined(_WIN64)
    #if defined(DEBUG_STARTUP)
    printf("src/main.c: debug: Installing signal handlers.\n");
    #endif
    signal(SIGINT, sigint_handler);
    signal(SIGTERM, sigterm_handler);
    #endif

    #if BUILD_HASM_BIN
    return hasm_main(argc, (const char **)argv);
    #else
    // Do something here later...
    #endif

    return 0;
}

static int str_is_spaces(const char *s) {
    if (!*s)
        return 0;
    while (*s) {
        if (*s == ' ') {
            s++;
            continue;
        }
        return 0;
    }
    return 1;
}

#if defined(_WIN32) || defined(_WIN64)
int WINAPI WinMain(
        ATTR_UNUSED HINSTANCE hInst, ATTR_UNUSED HINSTANCE hPrev,
        ATTR_UNUSED LPSTR szCmdLine, ATTR_UNUSED int sw) {
    if (AttachConsole(ATTACH_PARENT_PROCESS)) {
        main_WinEnsureStdoutStderr();
        main_havewinconsole = 1;
    }
    #if defined(DEBUG_STARTUP)
    printf("src/main.c: debug: WinMain() entered, "
        "doing argument reformatting.\n");
    #endif
    int argc = 0;
    char **argv = malloc(sizeof(*argv));
    if (!argv)
        return 1;
    char *execfullpath = spew3d_fs_GetOwnExecutablePath();
    char *execname = NULL;
    if (execfullpath) {
        char *execname = spew3d_fs_Basename(execfullpath);
        free(execfullpath);
        execfullpath = NULL;
    }
    if (execname) {
        argv[0] = strdup(execname);
    } else {
        argv[0] = strdup("HVM.exe");
    }
    if (!argv[0])
        return 1;
    argc = 1;

    int _winSplitCount = 0;
    LPWSTR *_winSplitList = NULL;
    _winSplitList = CommandLineToArgvW(
        GetCommandLineW(), &_winSplitCount
    );
    if (!_winSplitList) {
        oom: ;
        if (_winSplitList) LocalFree(_winSplitList);
        int i = 0;
        while (i < argc) {
            free(argv[i]);
            i++;
        }
        free(argv);
        fprintf(
            stderr, "src/main.c: error: "
            "Arg alloc or convert failure."
        );
        return -1;
    }
    if (_winSplitCount > 1) {
        char **argv_new = realloc(
            argv, sizeof(*argv) * (argc + _winSplitCount - 1)
        );
        if (!argv_new)
            goto oom;
        argv = argv_new;
        memset(&argv[argc], 0, sizeof(*argv) * (_winSplitCount - 1));
        argc += _winSplitCount - 1;
        int k = 1;
        while (k < _winSplitCount) {
            assert(sizeof(wchar_t) == sizeof(uint16_t));
            char *argbuf = NULL;
            argbuf = AS_U8_FROM_U16(
                _winSplitList[k]
            );
            if (!argbuf)
                goto oom;
            assert(k < argc);
            argv[k] = argbuf;
            k++;
        }
    }
    LocalFree(_winSplitList);
    _winSplitList = NULL;
    #ifdef HVM_USE_SDL
    SDL_SetMainReady();
    int result = SDL_main(argc, argv);
    #else
    int result = main(argc, argv);
    #endif
    int k = 0;
    while (k < argc) {
        free(argv[k]);
        k++;
    }
    free(argv);
    return result;
}
#endif

