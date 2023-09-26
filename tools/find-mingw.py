#!/usr/bin/python3
# Copyright (c) 2020-2023, ellie/@ell1e & HVM Team (see AUTHORS.md).
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
# AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
# ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
# LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
# CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
# SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
# ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
# POSSIBILITY OF SUCH DAMAGE.
#
# Alternatively, at your option, this file is offered under the Apache 2
# license, see accompanied LICENSE.md.

import os
import platform
import sys

def main():
    if platform.system().lower() != "linux":
        print("This tool is only really implemented and "
            "tested for mainstream linux distributions, sorry!",
            file=sys.stderr)
        sys.exit(1)
        return
    platform_name = "x86"
    tool = "gcc"
    do_print_flags = False
    do_print_host = False
    args = sys.argv[1:]
    i = 0
    while i < len(args):
        if args[i] == "--platform" and i + 1 < len(args):
            platform_name = args[i + 1]
        if args[i] == "--print-cflags":
            do_print_flags = True
        if args[i] == "--print-host":
            do_print_host = True
        if args[i] == "--tool" and i + 1 < len(args):
            tool = args[i + 1]
        i += 1
    if do_print_flags:
        if platform_name == "x86":
            print("-m32")
            sys.exit(0)
            return
        elif platform_name in ["x64", "x86_64"]:
            print("-m64")
            sys.exit(0)
            return
        print("Sadly, this platform is unsupported.",
            file=sys.stderr)
        sys.exit(1)
        return
    search_folder = ["/usr/bin", "/usr/local/bin"]
    for folder in search_folder:
        try:
            files_list = sorted(os.listdir(folder))
        except (OSError, PermissionError):
            continue
        for f in files_list:
            if os.path.isdir(os.path.join(folder, f)):
                continue
            tool_path = None
            if ("w64" in f and "-gcc" in f and "mingw" in f and\
                    tool == "gcc" and "c++" not in f) and \
                    not "posix" in f:
                tool_path = os.path.join(folder, f)
            if ("w64" in f and (("-gcc" in f and "c++" in f) or
                    "-g++" in f) and "mingw" in f and\
                    tool == "g++") and \
                    not "posix" in f:
                tool_path = os.path.join(folder, f)
            if tool_path != None and (
                    "x64" in f or "x86-64" in f or
                    "x86_64" in f) and platform_name in ["x86",
                    "x64", "x86_64"]:
                if do_print_host:
                    result = f
                    if result.endswith("-gcc"):
                        print(result.rpartition("-gcc")[0])
                    elif result.endswith("-gcc-c++"):
                        print(result.rpartition("-gcc-c++")[0])
                    elif result.endswith("-g++"):
                        print(result.rpartition("-g++")[0])
                    elif result.endswith("-cc"):
                        print(result.rpartition("-cc")[0])
                    else:
                        continue
                else:
                    print(tool_path)
                sys.exit(0)
    print("No MinGW found, sorry.")
    sys.exit(1)

if __name__ == "__main__":
    main()

