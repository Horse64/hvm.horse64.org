
SDL_PATH:=./vendor/SDL/
SPEW3D_PATH:=./vendor/Spew3D/

BINNAME:=HVM
ifneq (,$(findstring lhvmSDL,$(CFLAGS)))
BINHEADLESSNAME:=
GRAPHICAL_BUILD:=yes
else
BINHEADLESSNAME:=-headless
GRAPHICAL_BUILD:=no
endif
TEST_OBJECTS:=$(patsubst %.c, %.o, $(wildcard ./src/test_*.c))
ALL_OBJECTS:=$(patsubst %.c, %.o, $(wildcard ./src/*.c)) vendor/md5.o vendor/sha512crypt/sha512crypt.o vendor/sha2/sha2.o
TEST_BINARIES:=$(patsubst %.o, %.bin, $(TEST_OBJECTS))
PROGRAM_OBJECTS:=$(filter-out $(TEST_OBJECTS),$(ALL_OBJECTS))
PROGRAM_OBJECTS_NO_MAIN:=$(filter-out ./src/main.o,$(PROGRAM_OBJECTS))

CFLAGS+=-fPIC
ifneq ($(RELEASE_BUILD),yes)
CFLAGS_OPTIMIZATION:=-O1 -g -msse2 -msse3 -march=core2 -fno-omit-frame-pointer
else
CFLAGS_OPTIMIZATION:=-Ofast -s -ftree-vectorize -flto -msse2 -msse3 -march=core2 -fno-finite-math-only -fomit-frame-pointer -DNDEBUG
endif
ifeq ($(WIN_BUILD),yes)
BINEXT:=.exe
LIBEXT:=.dll
else
BINEXT:=.bin
LIBEXT:=.so
CFLAGS+= -pthread
PLATFORM:=linux
HOSTOPTION:=
LDFLAGS+= -lm -ldl
STRIPTOOL:=strip
endif
CFLAGS+= -I$(SPEW3D_PATH)/include/ -I./vendor/sha512crypt/ -I./vendor/sha2/
ifeq ($(GRAPHICAL_BUILD),yes)
CFLAGS+= -L$(SDL_PATH)/build/.libs/
endif

build-both: clean build-headless clean build-graphical	

build-headless: check-submodules
	$(MAKE) build-default
build-graphical: check-submodules-graphical
	CFLAGS='$(CFLAGS) -lhvmSDL' $(MAKE) build-default
build-default: reassemble-spew3d $(ALL_OBJECTS)
	$(CC) $(CFLAGS) -o ./"$(BINNAME)$(BINHEADLESSNAME)$(BINEXT)" $(PROGRAM_OBJECTS) $(LDFLAGS)
	$(CC) $(CFLAGS) -shared -o ./"$(BINNAME)$(BINHEADLESSNAME)$(LIBEXT)" $(PROGRAM_OBJECTS_NO_MAIN) $(LDFLAGS)	

%.o: %.c $.h
	$(CC) $(CFLAGS) -c -o $@ $<

reassemble-spew3d:
	cd $(SPEW3D_PATH) && make amalgamate

check-submodules:
	@if [ ! -e "$(SDL_PATH)/README.md" ]; then echo ""; echo -e '\033[0;31m$$(SDL_PATH)/README.md missing. Did you download the submodules?\033[0m'; echo "Try this:"; echo ""; echo "    git submodule init && git submodule update"; echo ""; exit 1; fi
	@echo "Submodules appear to exist."
	@if [ ! -e "$(SPEW3D_PATH)/include/spew3d.h" ]; then echo "Warning, graphical dependencies appear to be not build. Automatically running build-deps target."; $(MAKE) build-deps; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps' to build them again.)"

check-submodules-graphical: check-submodules
	@if [ ! -e "$(SDL_PATH)/build/.libs/libhvmSDL.a" ]; then echo "Warning, graphical dependencies appear to be not build. Automatically running build-deps target."; $(MAKE) build-deps-graphical; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps-graphical' to build them again.)"

build-deps: amalgamate-spew3d

build-deps-graphical: build-sdl amalgamate-spew3d

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


clean:
	rm -f $(ALL_OBJECTS)
	rm -f $(BINNAME).bin $(BINNAME).exe $(BINNAME).so $(BINNAME).dll
	rm -f $(BINNAME)-headless.bin $(BINNAME)-headless.exe $(BINNAME)-headless.so $(BINNAME)-headless.dll
