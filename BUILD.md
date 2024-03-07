
Build instructions
==================

**Important:** Building HVM is supported on Linux and possibly
most other Unix systems with GNU make. *Building is not
supported on MS Windows.*


Build steps
-----------

*(This is the regular build procedure if you already have
a fully built and working Horse64 on your system.)*

To build HVM without docker, go through the following steps:

1. Install all build dependencies, which are:

   1. Python 3, for now, won't be needed in the future.
   2. GCC including libc headers.
   3. X11 and/or Wayland headers for SDL2 if you're not
      just building the headless versions.
   4. Horse64 SDK, specifically `horserun`. Make sure
      `horserun` is available in your path or symlink
      it into this repository's root via:

      ```bash
      ln -s /path/to/your/binary/of/horserun ./horserun
      ```
   5. autotools and autoconf and libtool and cmake, for
      some of the dependencies.
   6. git.

2. Run the following commands to build:

   ```bash
   make veryclean
   make build-all
   ```

   The resulting binaries will be in the `output` folder.

(In theory, these build steps might work on Windows
inside a MinGW/MSYS bash to give you a Windows version.
But this isn't tested much, the official Windows release
is built using cross-compilers on Linux.)


Bootstrapping HVM
-----------------

*(This is the **advanced approach to bootstrap from
scratch without a prebuilt Horse64 already available.**
You will need to do this if e.g. you're on a new,
different Unix-like where no prebuilt Horse64 is available
yet. Please not all Unix systems are supported by HVM,
but most should work. HVM expects a little endian CPU
and 32-bit or 64-bit pointers and will break otherwise.)*

If you want to bootstrap Horse64 without having it
already prebuilt, you'll need to use its Python translator
to run the parts of HVM's build to run Horse64.

The steps for that are as follows:

1. Clone the core.horse64.org package into some **separate
   folder outside of the HVM repo,**
   via `git clone https://codeberg.org/Horse64/core.horse64.org/`

2. Install all the dependencies needed for the Python
   bootstrap translator, these are listed in
   `translator/translator-manual.md`. **DON'T RUN
   `make bootstrap`, ONLY INSTALL THE DEPENDENCIES!!!**
   (Running `make bootstrap` would just require the entirety of
   HVM indirectly again, you'd be entering a needless circle.)

3. Verify that the translator works by running the following
   command inside the core.horse64.org package's repo folder:

   ```bash
   translator/horsec.py --help
   ```

   (Run this in the core repository root, not any subfolders.)

   If it works, you should be seeing the horsec help listing.

4. Now do the HVM build as usual, but prefix any `make`
   commands with setting the environment variable `HORSERUN`
   such that it points to the temporary Python-based version:

   ```bash
   export HORSERUN="python3 /path/to/your/core.horse64.org/translator/horsec_run.py"
   export HORP="python3 /path/to/your/core.horse64.org/translator/horp.py"
   make
   ```

   (You need to **adjust this to your core.horse64.org package
   repo clone path,** obviously.)

   If you did something wrong, it should abort fairly early
   by complaining it can't create `src/hvm_version.h`.


Build a release version
-----------------------

To build a full release version, it's recommend you use docker
if you're on Linux. You'll need to install docker and
docker-compose.

### Build a release for x64 Linux and x64 Windows on x64 Linux

1. (optional) Clear out your `output` folder to discard
   previous binaries.

2. Run these commands:

   1. If you ran it previously, stop the previous docker
      container if needed and clean it up:

      ```bash
      docker-compose -f docker-compose.build-x64.yml stop build-x64
      docker-compose -f docker-compose.build-x64.yml rm build-x64
      ```

   2. Update the docker container's base image:

      ```bash
      docker-compose -f docker-compose.build-x64.yml pull build-x64
      docker-compose -f docker-compose.build-x64.yml build build-x64
      ```

   3. Run the build:

      ```bash
      docker-compose -f docker-compose.build-x64.yml run -e DONT_MARK_AS_DEV_VERSION=yes build-x64
      ```

      The resulting files should end up in your `output` folder.

(Note: the `DONT_MARK_AS_DEV_VERSION` is optional, if you want
to just get a test build then omit it.)

### Build a release for your local Unix-like system on any Unix-like:

1. (optional) Clear your `output` folder to discard previous
   binaries.

3. Run these comands:

   ```bash
   make veryclean
   RELEASE_BUILD=yes DONT_MARK_AS_DEV_VERSION=yes make build-all
   ```

   For this approach, don't forget to also specify `HORSERUN...`
   for all make commands if you're bootstrapping without a prebuilt
   Horse64.

