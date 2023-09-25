#!/usr/bin/python3

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
                    tool == "gcc" and "c++" not in f):
                tool_path = os.path.join(folder, f)
            if ("w64" in f and (("-gcc" in f and "c++" in f) or
                    "-g++" in f) and "mingw" in f and\
                    tool == "g++"):
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

