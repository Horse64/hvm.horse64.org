
SDL_PATH:=./vendor/SDL/
SPEW3D_PATH:=./vendor/Spew3D/
COMMONMARK_PATH=./vendor/commonmark/

ifeq ($(PLATFORM),)
ifneq (,$(findstring mingw,$(CC)))
PLATFORM:=windows
endif
endif
BINNAME:=HVM
ifneq (,$(findstring lhvmSDL,$(CFLAGS_ADDEDINTERNAL)))
BINHEADLESSNAME:=
else
BINHEADLESSNAME:=-headless
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
ifeq ($(PLATFORM),windows)
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
LDFLAGS+= -lhvmcmark
CFLAGS+= -I./built_deps/include/ -I./vendor/sha512crypt/ -I./vendor/sha2/ -L./built_deps/

build-both: clean build-headless clean build-graphical	

build-headless: check-submodules
	$(MAKE) build-default
build-graphical: check-submodules-graphical
	CFLAGS_ADDEDINTERNAL="$(CFLAGS_ADDEDINTERNAL) -lhvmSDL" $(MAKE) build-default
build-default: amalgamate-spew3d $(ALL_OBJECTS)
	$(CC) $(CFLAGS) $(CFLAGS_ADDEDINTERNAL) -o ./"$(BINNAME)$(BINHEADLESSNAME)$(BINEXT)" $(PROGRAM_OBJECTS) $(LDFLAGS)
	$(CC) $(CFLAGS) $(CFLAGS_ADDEDINTERNAL) -shared -o ./"$(BINNAME)$(BINHEADLESSNAME)$(LIBEXT)" $(PROGRAM_OBJECTS_NO_MAIN) $(LDFLAGS)	

%.o: %.c %.h
	$(CC) $(CFLAGS) $(CFLAGS_OPTIMIZATION) -c -o $@ $<

check-submodules:
	@if [ ! -e "$(SDL_PATH)/README.md" ]; then echo ""; echo -e '\033[0;31m$$(SDL_PATH)/README.md missing. Did you download the submodules?\033[0m'; echo "Try this:"; echo ""; echo "    git submodule init && git submodule update"; echo ""; exit 1; fi
	@echo "Submodules appear to exist."
	@if [ ! -e "$(SPEW3D_PATH)/include/spew3d.h" ]; then echo "Warning, graphical dependencies appear to be not build. Automatically running build-deps target."; $(MAKE) build-deps; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps' to build them again.)"

check-submodules-graphical: check-submodules
	@if [ ! -e "$(SDL_PATH)/build/.libs/libhvmSDL.a" ]; then echo "Warning, graphical dependencies appear to be not build. Automatically running build-deps target."; $(MAKE) build-deps-graphical; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps-graphical' to build them again.)"

build-deps: amalgamate-spew3d build-commonmark

build-deps-graphical: build-commonmark build-sdl amalgamate-spew3d

amalgamate-spew3d:
	cd "$(SPEW3D_PATH)" && git submodule update --init && make clean && make amalgamate
	mkdir -p ./built_deps/include/
	cp "$(SPEW3D_PATH)/include/spew3d.h" ./built_deps/include/

build-windows-x64:
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" HOSTOPTION="--host `tools/find-mingw.py --platform x64 --print-host`" CXX="`tools/find-mingw.py --platform x64 --tool g++`" build-deps-graphical
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" HOSTOPTION="--host `tools/find-mingw.py --platform x64 --print-host`" CXX="`tools/find-mingw.py --platform x64 --tool g++`" build-default
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" HOSTOPTION="--host `tools/find-mingw.py --platform x64 --print-host`" CXX="`tools/find-mingw.py --platform x64 --tool g++`" build-graphical

build-sdl:
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
	cd "$(SDL_PATH)" && make
	mkdir -p ./built_deps/include
	rm -rf ./built_deps/include/SDL2/
	cp -R "$(SDL_PATH)/include/" ./built_deps/include/SDL2/
	cp "$(SDL_PATH)/build/.libs/libSDL2.a" "built_deps/libhvmSDL.a"
	cp "$(SDL_PATH)/include/SDL_config.h.OLD" "$(SDL_PATH)/include/SDL_config.h"

build-commonmark:
	mkdir -p ./built_deps/
ifeq ($(PLATFORM),windows)
	mkdir -p "$(COMMONMARK_PATH)"/build
	cd "$(COMMONMARK_PATH)"/build && cmake CC="$(CC) CXX="$(CXX) CMAKE_SYSTEM_NAME=Windows CMAKE_FIND_ROOT_PATH_MODE_PROGRAM=NEVER CMAKE_FIND_ROOT_PATH_MODE_LIBRARY=ONLY CMAKE_FIND_ROOT_PATH_MODE_INCLUDE=ONLY ..
	cd "$(COMMONMARK_PATH)"/build && make CC="$(CC) CXX="$(CXX)
	cp "$(COMMONMARK_PATH)/build/src/libcmark.a" "./built_deps/libhvmcmark.a"
else
	mkdir -p "$(COMMONMARK_PATH)"/build
	cd "$(COMMONMARK_PATH)"/build && cmake ..
	cd "$(COMMONMARK_PATH)/build/" && make
	cp "$(COMMONMARK_PATH)/build/src/libcmark.a" "./built_deps/libhvmcmark.a"
endif

clean:
	rm -f $(ALL_OBJECTS)
	rm -f $(BINNAME).bin $(BINNAME).exe $(BINNAME).so $(BINNAME).dll
	rm -f $(BINNAME)-headless.bin $(BINNAME)-headless.exe $(BINNAME)-headless.so $(BINNAME)-headless.dll

veryclean: clean
	rm -rf ./built_deps/
	cd "$(COMMONMARK_PATH)" && rm -rf ./build && rm -rf ./build-mingw/
	cd "$(COMMONMARK_PATH)" && rm -f ./CMakeCache.txt
	cd "$(SDL_PATH)" && rm -rf "$(SDL_PATH)/build/"
	rm -f "$(SDL_PATH)/Makefile"
	rm -rf "$(SDL_PATH)/gen"
	rm -rf "$(SDL_PATH)/build/*"
	cp "$(SDL_PATH)/include/SDL_config.h" "$(SDL_PATH)/include/SDL_config.h.OLD"
	cd "$(SPEW3D_PATH)" && make clean

