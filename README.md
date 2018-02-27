Zlib library build for Staticlibs
=================================

[![travis](https://travis-ci.org/staticlibs/external_zlib.svg?branch=master)](https://travis-ci.org/staticlibs/external_zlib)
[![appveyor](https://ci.appveyor.com/api/projects/status/github/staticlibs/external_zlib?svg=true)](https://ci.appveyor.com/project/staticlibs/external-zlib)

This project is a part of [Staticlibs](http://staticlibs.net/).

This project contains a CMake build file for building the [Zlib library](http://www.zlib.net/) that
can be used to build sources imported from [CentOS lookaside](https://github.com/staticlibs/lookaside_zlib.git).

Link to the [documentation](http://www.zlib.net/manual.html).

How to build
------------

[CMake](http://cmake.org/) is required for building.

To build the library on Windows using Visual Studio 2013 Express run the following commands using
Visual Studio development command prompt 
(`C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\Shortcuts\VS2013 x86 Native Tools Command Prompt`):

    git clone --recursive https://github.com/staticlibs/external_zlib.git
    cd external_zlib
    mkdir build
    cd build
    cmake ..
    msbuild external_zlib.sln

See [StaticlibsToolchains](https://github.com/staticlibs/wiki/wiki/StaticlibsToolchains) for 
more information about the toolchain setup and cross-compilation.

License information
-------------------

This project is released under the [Apache License 2.0](http://www.apache.org/licenses/LICENSE-2.0).

Changelog
---------

**2018-02-28**
 * version 1.2.7-3
 * drop submodule

**2017-12-24**
 * version 1.2.7-2
 * vs2017 support

**2016-10-30**

 * version 1.2.7-1
 * use Zlib sources from CentOS

**2016-01-17**

 * version 1.2.8.6
 * minor CMake changes

**2015-12-03**

 * version 1.2.8.5
 * deplibs cache support

**2015-11-05**

 * version 1.2.8.4 - `zlib_install` dir added

**2015-11-01**

 * version 1.2.8.3 - `pkg-config` integration

**2015-07-08**

 * version 1.2.8.2 - toolchains update

**2015-06-30**

 * version 1.2.8.1 - toolchains update

**2015-05-20**

 * 1.2.8, initial public version
