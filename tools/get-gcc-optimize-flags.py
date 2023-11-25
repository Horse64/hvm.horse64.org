#!/usr/bin/python3

import os
import shutil
import subprocess
import tempfile

def test_flags(flags):
    test_dir = os.path.abspath(
        tempfile.mkdtemp(prefix="hvm-tools")
    )
    try:
        os.chdir(test_dir)
        CC_name = os.getenv("CC", "gcc")
        with open("test.c", "w") as f:
            f.write("int main(int argc, const char** argv) {return 0;}")
        subprocess.check_output([
            CC_name] + flags + ["test.c"
        ], stderr=subprocess.STDOUT)
    except subprocess.CalledProcessError:
        try:
            shutil.rmtree(test_dir)
        except FileNotFoundError:
            pass
        return False
    finally:
        try:
            shutil.rmtree(test_dir)
        except FileNotFoundError:
            pass
    return True

flags="-msse2 -msse3 -march=core2"
if not test_flags(["-msse2"]):
    flags=""
    if test_flags(["-march=armv8-a"]):
        flags=("-mfix-cortex-a53-835769 " +
            " -mfix-cortex-a53-843419 " +
            "-march=armv8-a -mtune=generic -mabi=lp64")
print(flags)
