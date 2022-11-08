
SDL_PATH:=./vendor/SDL/
SPEW3D_PATH:=./vendor/Spew3D/

BINNAME:=HVM
TEST_OBJECTS:=$(patsubst %.c, %.o, $(wildcard ./src/test_*.c))
ALL_OBJECTS:=$(patsubst %.c, %.o, $(wildcard ./src/*.c)) vendor/md5.o vendor/sha512crypt/sha512crypt.o vendor/sha2/sha2.c
TEST_BINARIES:=$(patsubst %.o, %.bin, $(TEST_OBJECTS))
PROGRAM_OBJECTS:=$(filter-out $(TEST_OBJECTS),$(ALL_OBJECTS))
PROGRAM_OBJECTS_NO_MAIN:=$(filter-out ./src/main.o,$(PROGRAM_OBJECTS))

ifneq ($(RELEASE_BUILD),true)
CFLAGS_OPTIMIZATION:=-O1 -g -msse2 -msse3 -march=core2 -fno-omit-frame-pointer
else
CFLAGS_OPTIMIZATION:=-Ofast -s -ftree-vectorize -flto -msse2 -msse3 -march=core2 -fno-finite-math-only -fomit-frame-pointer -DNDEBUG
endif
ifeq ($(WIN_BUILD),true)
BINEXT:=.exe
else
BINEXT:=.bin
CFLAGS+= -pthread
PLATFORM:=linux
HOSTOPTION:=
LDFLAGS+= -lm -ldl
STRIPTOOL:=strip
endif
CFLAGS+= -I$(SPEW3D_PATH)/include/ -I./vendor/sha512crypt/ -I./vendor/sha2/

default: check-submodules $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o ./"$(BINNAME)$(BINEXT)" $(PROGRAM_OBJECTS) $(LDFLAGS)

%.o: %.c $.h
	$(CC) $(CFLAGS) -c -o $@ $<

check-submodules:
	@if [ ! -e "$(SDL_PATH)/README.md" ]; then echo ""; echo -e '\033[0;31m$$(SDL_PATH)/README.md missing. Did you download the submodules?\033[0m'; echo "Try this:"; echo ""; echo "    git submodule init && git submodule update"; echo ""; exit 1; fi
	@echo "Submodules appear to exist."
	@if [ ! -e "$(SDL_PATH)/build/.libs/libhvmSDL.a" ]; then echo "Warning, dependencies appear to be not build. Automatically running build-deps target."; make build-deps; fi
	@echo "Submodules appear to have been built some time. (Run build-deps to build them again.)"

build-deps: build-sdl amalgamate-spew3d

amalgamate-spew3d:
	cd "$(SPEW3D_PATH)" && git submodule update --init && make

build-sdl:
	rm -f "$(SDL_PATH)/Makefile"
	rm -rf "$(SDL_PATH)/gen"
	rm -rf "$(SDL_PATH)/build/*"
	cp "$(SDL_PATH)/include/SDL_config.h" "$(SDL_PATH)/include/SDL_config.h.OLD"
ifeq ($(PLATFORM),linux)
	cd "$(SDL_PATH)" && bash ./autogen.sh && ./configure --disable-video-opengles1 --enable-assertions=release --disable-video-vulkan --enable-sse3 --enable-ssemath --disable-oss --disable-jack --enable-static --disable-shared --enable-ssemath --disable-libsamplerate
else
ifeq ($(PLATFORM),windows)
	cd "$(SDL_PATH)" && ./configure $(HOSTOPTION) --disable-wasapi --enable-assertions=release --enable-sse --enable-sse2 --enable-sse3 --enable-static --disable-shared --disable-pthreads --enable-threads --enable-ssemath --disable-video-opengl --disable-video-opengles --disable-video-vulkan --enable-render-d3d --disable-libsamplerate
else
	echo "ERROR: Unsupported platform for SDL build."
	exit 1
endif
endif
	cd "$(SDL_PATH)" && make clean && make
	cp "$(SDL_PATH)/build/.libs/libSDL2.a" "$(SDL_PATH)/build/.libs/libhvmSDL.a"
	cp "$(SDL_PATH)/include/SDL_config.h.OLD" "$(SDL_PATH)/include/SDL_config.h"



