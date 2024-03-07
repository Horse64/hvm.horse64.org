
SDL_PATH:=./vendor/SDL/
SPEW3D_PATH:=./vendor/Spew3D/
SPEW3DWEB_PATH:=./vendor/Spew3D-Web/
SPEW3DNET_PATH:=./vendor/Spew3D-Net/
REPO_PATH:=$(shell pwd)

ifeq ($(PLATFORM),)
ifneq (,$(findstring mingw,$(CC)))
PLATFORM:=windows
endif
endif

UNITTEST_SOURCES_NOSDL=$(sort $(wildcard ./src/test_*_nosdl.c ./tests-standalone/*.c))
UNITTEST_SOURCES_ALL=$(sort $(wildcard ./src/test_*.c ./tests-standalone/*.c))
UNITTEST_SOURCES_WITHSDL=$(sort $(filter-out $(UNITTEST_SOURCES_ALL), $(UNITTEST_SOURCES_NOSDL)))
UNITTEST_BASENAMES=$(sort $(patsubst %.c, %, $(UNITTEST_SOURCES_ALL)))
UNITTEST_BASENAMES_NOSDL=$(sort $(patsubst %.c, %, $(UNITTEST_SOURCES_NOSDL)))
UNITTEST_BASENAMES_WITHSDL=$(sort $(patsubst %.c, %, $(UNITTEST_SOURCES_WITHSDL)))
ALL_OBJECTS:=$(patsubst %.c, %.o, $(wildcard ./src/*.c ./src/hasm/*.c)) vendor/md5.o vendor/sha512crypt/sha512crypt.o vendor/sha2/sha2.o
PROGRAM_OBJECTS:=$(filter-out $(UNITTEST_SOURCES),$(ALL_OBJECTS))
PROGRAM_OBJECTS_NO_MAIN:=$(filter-out ./src/main.o,$(PROGRAM_OBJECTS))

# Detect horserun tool:
ifeq ($(HORSERUN),)
ifneq ("$(wildcard ./horserun)","")
HORSERUN:=./horserun
else
HORSERUN:=horserun
endif
endif

# Detect horp tool:
ifeq ($(HORP),)
ifneq ("$(wildcard ./horp)","")
HORP:=./horp
else
HORP:=horp
endif
endif

# Detect various build-related variables:
HVM_VERSION_CONTENTS:=$(shell PATH=`pwd`:$(PATH) $(HORSERUN) "tools/echo_hvm_version_h.h64")
ifneq (,$(findstring lhvmSDL,$(LDFLAGS_ADDEDINTERNAL)))
BINHEADLESSNAME:=
else
ifeq ($(BUILD_HASM),)
BINHEADLESSNAME:=-headless
else
BINHEADLESSNAME:=
endif
endif
ifneq ($(BUILD_HASM),)
HASMBUILDFLAG:=-DBUILD_HASM_BIN
BINNAME:=hasm
else
HASMBUILDFLAG:=
BINNAME:=HVM
endif
POSIX_THREADS=$(shell CC="$(CC)" python3 tools/check-win32-threads.py)
CFLAGS+=-fPIC $(HASMBUILDFLAG)
ifeq ($(DONT_MARK_AS_DEV_VERSION),yes)
CFLAGS+=-DDONT_MARK_AS_DEV_VERSION
endif
ifneq ($(RELEASE_BUILD),yes)
CFLAGS_OPTIMIZATION:=-O1 -g `tools/get-gcc-optimize-flags.py` -fno-omit-frame-pointer
else
CFLAGS_OPTIMIZATION:=-Ofast -s -ftree-vectorize -flto `tools/get-gcc-optimize-flags.py` -fno-finite-math-only -fomit-frame-pointer -DNDEBUG
endif
ifeq ($(PLATFORM),windows)
BINEXT:=.exe
LIBEXT:=.dll
CFLAGS+= -no-pthread -static-libgcc -mwindows
LDFLAGS+= -Wl,-Bdynamic -lole32
else
BINEXT:=.bin
LIBEXT:=.so
CFLAGS+= -pthread
PLATFORM:=linux
HOSTOPTION:=
LDFLAGS+= -Wl,-Bdynamic -lm -ldl
STRIPTOOL:=strip
endif
LDFLAGS+= -Wl,-Bstatic -Wl,-Bdynamic
CFLAGS+= -I./output/built_deps/include/ -I./src/ -I./vendor/sha512crypt/ -I./vendor/sha2/ -L./output/built_deps/

.PHONY: build-all build-headless build-graphical build-hasm forbid-winpthread check-submodules check-submodules-graphical build-deps build-deps-graphical amalgamate-spew3d amalgamate-spew3dweb build-windows-x64 build-default built-default-no-lib build-sdl clean objectclean veryclean depsclean create-version-header

build-all:
	@echo "--- C compiler used: (start) ---"
	@$(CC) --version
	@echo "--- C compiler used (end) ---"
	@echo "--- HVM version to be built: (start) ---"
	$(HORSERUN) "tools/echo_hvm_version_h.h64"
	@echo "--- HVM vesion to be built (end) ---"
	@echo "--- Horp used: (start) ---"
	$(HORP) --version
	@echo "--- Horp used (end) ---"
	$(MAKE) clean
	$(MAKE) objectclean build-headless
	$(MAKE) objectclean build-graphical
	$(MAKE) objectclean build-hasm

build-tests: amalgamate-spew3d
	echo "TESTS: $(UNITTEST_SOURCES) | $(UNITTEST_BASENAMES)"
	for x in $(UNITTEST_BASENAMES_WITHSDL); do $(CC) -g -Ofast -ftree-vectorize -flto `tools/get-gcc-optimize-flags.py` $(CFLAGS) -Iinclude/ $(CXXFLAGS) -pthread -o ./$$x$(BINEXT) ./$$x.c -lSDL2 -lcheck -lrt -lm $(LDFLAGS) || { exit 1; }; done
	for x in $(UNITTEST_BASENAMES_NOSDL); do $(CC) -g -Ofast -ftree-vectorize -flto `tools/get-gcc-optimize-flags.py` $(CFLAGS) -Iinclude/ $(CXXFLAGS) -pthread -o ./$$x$(BINEXT) ./$$x.c -lcheck -lrt -lm $(LDFLAGS) || { exit 1; }; done

run-tests:
	echo "TESTS: $(UNITTEST_SOURCES) | $(UNITTEST_BASENAMES)"
	for x in $(UNITTEST_BASENAMES_WITHSDL); do time ./$$x$(BINEXT) || { exit 1; }; done
	for x in $(UNITTEST_BASENAMES_NOSDL); do time ./$$x$(BINEXT) || { exit 1; }; done
	test: build-tests run-tests

test: build-tests run-tests

build-headless: forbid-winpthread check-submodules
	$(MAKE) build-default
build-hasm: forbid-winpthread check-submodules
	BUILD_HASM=yes $(MAKE) build-default-no-lib
build-graphical: forbid-winpthread check-submodules-graphical
	CFLAGS_ADDEDINTERNAL="$(CFLAGS_ADDEDINTERNAL) -DHVM_USE_SDL" LDFLAGS_ADDEDINTERNAL="$(LDFLAGS_ADDEDINTERNAL) -Wl,-Bstatic -lhvmSDL -Wl,-Bdynamic" $(MAKE) build-default
build-default: amalgamate-spew3d amalgamate-spew3dweb horp-build create-version-header $(ALL_OBJECTS)
	mkdir -p output
	$(CC) $(CFLAGS) $(CFLAGS_ADDEDINTERNAL) -o ./output/"$(BINNAME)$(BINHEADLESSNAME)$(BINEXT)" $(PROGRAM_OBJECTS) $(LDFLAGS)
	$(CC) $(CFLAGS) $(CFLAGS_ADDEDINTERNAL) -shared -o ./output/"$(BINNAME)$(BINHEADLESSNAME)$(LIBEXT)" $(PROGRAM_OBJECTS_NO_MAIN) $(LDFLAGS) $(LDFLAGS_ADDEDINTERNAL)
build-default-no-lib: amalgamate-spew3d amalgamate-spew3dweb $(ALL_OBJECTS)
	mkdir -p output
	$(CC) $(CFLAGS) $(CFLAGS_ADDEDINTERNAL) -o ./output/"$(BINNAME)$(BINHEADLESSNAME)$(BINEXT)" $(PROGRAM_OBJECTS) $(LDFLAGS)
create-version-header:
	echo "Creating src/hvm_version.h..."
	if [ -z '$(HVM_VERSION_CONTENTS)' ]; then echo "Failed to get HVM version, did you install horserun correctly? Check BUILD.md for details."; exit 1; fi
	echo '$(HVM_VERSION_CONTENTS)' > ./src/hvm_version.h
	echo "Done, src/hvm_version.h was written."

%.o: %.c %.h
	$(CC) $(CFLAGS) $(CFLAGS_OPTIMIZATION) -c -o $@ $<

horp-build:
	@echo "Invoking horp (HORP='$(HORP)') to apply templates..."
	$(HORP) template apply .

forbid-winpthread:
ifeq ($(PLATFORM),windows)
	@if [ "x$(POSIX_THREADS)" = "xposix threads" ]; then echo "ERROR: POSIX THREADS MINGW NOT SUPPORTED."; exit 1; fi
endif

check-submodules:
	@if [ ! -e "$(SDL_PATH)/README.md" ]; then echo ""; echo -e '\033[0;31m$$(SDL_PATH)/README.md missing. Did you download the submodules?\033[0m'; echo "Try this:"; echo ""; echo "    git submodule init && git submodule update"; echo ""; exit 1; fi
	@echo "Submodules appear to exist."
	@if [ ! -e "output/built_deps/include/spew3d.h" ]; then echo "Warning, headless dependencies appear to be not build. Automatically running build-deps target."; $(MAKE) build-deps; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps' to build them again.)"

check-submodules-graphical: check-submodules
	@if [ ! -e "output/built_deps/libhvmSDL.a" ]; then echo "Warning, graphical dependencies appear to be not build. Automatically running build-deps-graphical target."; $(MAKE) build-deps-graphical; fi
	@echo "Submodules appear to have been built some time. (Run 'make build-deps-graphical' to build them again.)"

build-deps: amalgamate-spew3d amalgamate-spew3dweb amalgamate-spew3dnet

build-deps-graphical: build-sdl amalgamate-spew3d amalgamate-spew3dweb amalgamate-spew3dnet

amalgamate-spew3d:
	cd "$(SPEW3D_PATH)" && git submodule update --init && make clean && make amalgamate
	mkdir -p ./output/built_deps/include/
	cp "$(SPEW3D_PATH)/include/spew3d.h" ./output/built_deps/include/

amalgamate-spew3dweb:
	cd "$(SPEW3DWEB_PATH)" && git submodule update --init && make clean && make amalgamate
	mkdir -p ./output/built_deps/include/
	cp "$(SPEW3DWEB_PATH)/include/spew3dweb.h" ./output/built_deps/include/

amalgamate-spew3dnet:
	cd "$(SPEW3DNET_PATH)" && git submodule update --init && make clean && make amalgamate
	mkdir -p ./output/built_deps/include/
	cp "$(SPEW3DNET_PATH)/include/spew3dnet.h" ./output/built_deps/include/

build-windows-x64:
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" forbid-winpthread
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" HOSTOPTION="--host `tools/find-mingw.py --platform x64 --print-host`" CXX="`tools/find-mingw.py --platform x64 --tool g++`" build-deps-graphical
	CFLAGS="`tools/find-mingw.py --platform x64 --print-cflags`" $(MAKE) CC="`tools/find-mingw.py --platform x64`" HOSTOPTION="--host `tools/find-mingw.py --platform x64 --print-host`" CXX="`tools/find-mingw.py --platform x64 --tool g++`" build-all

build-sdl:
	cd "$(SDL_PATH)" && python3 "$(REPO_PATH)/tools/disable-sdl-dynamic-api.py"
ifeq ($(PLATFORM),linux)
	cd "$(SDL_PATH)" && bash ./autogen.sh && ./configure --disable-video-opengles1 --enable-assertions=release --disable-video-vulkan --enable-sse3 --enable-ssemath --disable-oss --disable-jack --enable-static --disable-shared --enable-ssemath --disable-libsamplerate
else
ifeq ($(PLATFORM),windows)
	cd "$(SDL_PATH)" && bash ./autogen.sh && ./configure $(HOSTOPTION) --disable-wasapi --enable-assertions=release --enable-sse --enable-sse2 --enable-sse3 --enable-static --disable-shared --disable-pthreads --enable-threads --enable-ssemath --disable-video-opengl --disable-video-opengles --disable-video-vulkan --enable-render-d3d --disable-libsamplerate
else
	@echo "ERROR: Unsupported platform for SDL build."
	exit 1
endif
endif
	cd "$(SDL_PATH)" && make
	mkdir -p ./output/built_deps/include
	rm -rf ./output/built_deps/include/SDL2/
	cp -R "$(SDL_PATH)/include/" ./output/built_deps/include/SDL2/
	cp "$(SDL_PATH)/build/.libs/libSDL2.a" "./output/built_deps/libhvmSDL.a"
	if [ -e "$(SDL_PATH)/include/SDL_config.h.OLD" ]; then cp "$(SDL_PATH)/include/SDL_config.h.OLD" "$(SDL_PATH)/include/SDL_config.h"; fi

objectclean:
	rm -f $(ALL_OBJECTS)

reset-deps:
	git submodule foreach --recursive git reset --hard && git submodule foreach --recursive git clean -xfd && git submodule update --init
	cd "$(SPEW3D_PATH)" && $(MAKE) reset-deps && $(MAKE) clean
	cd "$(SPEW3DWEB_PATH)" && $(MAKE) reset-deps && $(MAKE) clean
	cd "$(SPEW3DNET_PATH)" && $(MAKE) reset-deps && $(MAKE) clean

clean: objectclean
	rm -f output/$(BINNAME)-$(BINHEADLESSNAME).$(BINEXT)
	rm -rf output/$(BINNAME)-$(BINHEADLESSNAME).$(LIBEXT)

veryclean: depsclean
	rm -rf ./output/built_deps/
	rm -f ./output/*.bin ./output/*.dll ./output/*.exe ./output/*.so

depsclean:
	cd "$(SDL_PATH)" && rm -rf "$(SDL_PATH)/build/" && git checkout src/dynapi/SDL_dynapi.h
	cd "$(SDL_PATH)" && bash -c "make clean || echo 'Clean failed but who cares!'"
	rm -f "$(SDL_PATH)/Makefile"
	rm -rf "$(SDL_PATH)/gen"
	rm -rf "$(SDL_PATH)/build/*"
	cp "$(SDL_PATH)/include/SDL_config.h" "$(SDL_PATH)/include/SDL_config.h.OLD"
	cd "$(SPEW3D_PATH)" && make clean
	cd "$(SPEW3DWEB_PATH)" && make clean
	cd "$(SPEW3DNET_PATH)" && make clean

