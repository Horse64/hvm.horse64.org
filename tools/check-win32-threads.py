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
import shutil
import subprocess
import tempfile
import textwrap


def detect_thread_model():
    CC = os.environ.get("CC", "")

    if CC is None or len(CC) == 0:
        raise ValueError("CC not set")

    testdir = tempfile.mkdtemp()
    try:
        os.chdir(testdir)
        with open(os.path.join(testdir, "test.c"), "w",
                encoding="utf-8") as f:
            f.write(textwrap.dedent("""\
            #include <stdio.h>
            #include <windows.h>

            int main(int argc, const char **argv) {
                #if (defined(_WIN32) || defined(_WIN64)) && \
                    !defined(__WINPTHREADS_VERSION)
                #warning [[win32 threads]]
                #else
                #warning [[posix threads]]
                #endif
                fflush(stdout);
            }
            """))
        result = None
        try:
            result = subprocess.check_output([
                CC, "-v"
            ], cwd=testdir, stderr=subprocess.STDOUT)
            try:
                result = result.decode("utf-8", "replace")
            except AttributeError:
                pass
            lines = result.splitlines()
            for line in lines:
                if "thread model" in line.lower():
                    if "posix" in line.lower():
                        print("posix threads")
                    else:
                        print("win32 threads")
                    return
        except subproces.CalledProcessError:
            raise ValueError("cannot run compiler: " + CC)
        try:
            result = subprocess.check_output([
                CC, "-no-pthread", "-mconsole",
                "-o", "test.exe", "test.c"
            ], cwd=testdir, stderr=subprocess.STDOUT)
            try:
                result = result.decode("utf-8", "replace")
            except AttributeError:
                pass
            if "[[win32 threads]]" in result:
                print("win32 threads")
            elif "[[posix threads]]" in result:
                print("posix threads")
        except subprocess.CalledProcessError as e:
            output = e.output
            try:
                output = output.decode("utf-8", "replace")
            except AttributeError:
                pass
            raise ValueError("compile failed, likely not " +
                "a Windows compiler: " + CC + "\n\n" + output)
    finally:
        shutil.rmtree(testdir)

if __name__ == "__main__":
    detect_thread_model()

