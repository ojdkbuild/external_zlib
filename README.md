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

[TODO]

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0)

Changelog
---------

**2015-07-08**

 * version 1.2.8.2 - toolchains update

**2015-06-30**

 * version 1.2.8.1 - toolchains update

**2015-05-20**

 * 1.2.8, initial public version
