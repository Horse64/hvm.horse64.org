/* Copyright (c) 2024, ellie/@ell1e & HVM Team (see AUTHORS.md).

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

#include <string.h>

#include "hvm_version.h"

int hasm_main(int argc, const char **argv) {
    int i = 1;
    while (i < argc) {
        if (s3dstrcasecmp(argv[i], "--help") == 0 ||
                strcmp(argv[i], "-h") == 0) {
            printf("Usage:\n");
            printf("   hasm options…[optional] action action_args…[optional]\n");
            printf("\n");
            printf("Arguments:\n");
            printf("Arguments:\n");
            printf("   action        Pick an available actions: "
                   "compile, run. To see details for\n");
            printf("                 an action, use it with --help"
                   "like e.g.: horse compile --help\n");
            printf("   action_args   List of arguments for the "
                   "specified action\n");
            printf("\n");
            printf("Available Options:\n");
            printf("   --help      Show this help text.\n");
            printf("   --license   Print out the program's licensing.\n");
            printf("   --version   Print out the program version.\n");
            printf("\n");
            return 0;
        } else if (strcmp(argv[i], "--version") == 0 ||
                strcmp(argv[i], "-V") == 0) {
            printf("hasm v%s, built by:\n", HVM_VERSION);
            #ifdef __VERSION__
            printf("    Unknown C compiler:\n    %s\n", __VERSION__);
            #else
            printf("    Unknown C compiler.\n");
            #endif
            printf("%s\n", HVM_COPYRIGHT);
            return 0;
        }
        i++;
    }
}

