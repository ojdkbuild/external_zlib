Zlib library build for Staticlibs
=================================

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake wrapper for the [Zlib library](https://github.com/madler/zlib). 
Wrapper uses `Makefile`s for each supported platform, supports toolchain specification
with `STATICLIB_TOOLCHAIN` option and exports Zlib headers to be used from dependent projects.
Wrapper doesn't use Zlib's own CMake script directly as it appeared too hard to integrate.

Zlib GitHub repository is used as a git submodule of this project. Zlib is pinned to version v1.2.8.

Link to [documentation](http://www.zlib.net/manual.html).

How to build
------------

[CMake](http://cmake.org/) is required for building.

Staticlib toolchain name must be specified as a `STATICLIB_TOOLCHAIN` parameter to `cmake` command
unless you are using GCC on Linux x86_64 (`linux_amd64_gcc` toolchain) that is default.

List of [supported toolchains](https://github.com/staticlibs/cmake/tree/master/toolchains).

Example build for Windows x86_64 with Visual Studio 2013 Express, run the following commands 
from the development shell `C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x64 Cross Tools Command Prompt` :

    git clone https://github.com/staticlibs/external_zlib.git
    cd external_zlib
    git submodule update --init --recursive
    mkdir build
    cd build
    cmake .. -DSTATICLIB_TOOLCHAIN=windows_amd64_msvc -G "NMake Makefiles"
    nmake
    nmake test

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Changelog
---------

**2015-06-30**

 * version 1.2.8.1 - toolchains update

**2015-05-20**

 * 1.2.8, initial public version
