#!/usr/bin/python3

import os

if __name__ == "__main__":
    contents = None
    with open(os.path.join(
            "src", "dynapi", "SDL_dynapi.h"), "r",
            encoding="utf-8") as f:
        contents = f.read()
        lines = [l for l in contents.splitlines()
            if not "nope" in l.lower() or not "#error" in l.lower()]
        contents = "\n".join(lines)
    if not "/* disable dynapi unconditionally */" in contents:
        contents = (
            "#define SDL_DYNAMIC_API 0  " +
                "/* disable dynapi unconditionally */\n" +
            contents
        )
    with open(os.path.join(
            "src", "dynapi", "SDL_dynapi.h"), "w",
            encoding="utf-8") as f:
        f.write(contents)

