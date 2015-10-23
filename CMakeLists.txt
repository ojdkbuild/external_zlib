# Copyright 2015, alex at staticlibs.net
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

cmake_minimum_required ( VERSION 2.8.12 )

# project
project ( external_zlib CXX C )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 1.2.8.3 )
set ( ${PROJECT_NAME}_DESCRIPTION "Zlib library build for Staticlibs" )
set ( ${PROJECT_NAME}_URL https://github.com/staticlibs/external_zlib.git )

# variabled for makefiles
if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
endif ( )
if ( CMAKE_BUILD_TYPE MATCHES "Release" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
endif ( )

# zlib library
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/Makefile-${STATICLIB_TOOLCHAIN}
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
if ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_MAKE_PROGRAM nmake )
else ( )
    set ( ${PROJECT_NAME}_MAKE_PROGRAM make )
endif ( )
add_custom_target( ${PROJECT_NAME}
        COMMAND ${${PROJECT_NAME}_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_library( z STATIC IMPORTED GLOBAL )
add_dependencies( z ${PROJECT_NAME} )
set_target_properties( z PROPERTIES IMPORTED_LOCATION 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libz${CMAKE_STATIC_LIBRARY_SUFFIX} )
set ( ${PROJECT_NAME}_INCLUDES 
        ${CMAKE_CURRENT_BINARY_DIR}/zlib
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib )
# minizip
set ( ${PROJECT_NAME}_minizip_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/ioapi.c 
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/unzip.c
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/zip.c )
if ( WIN32 )
    set ( ${PROJECT_NAME}_minizip_SOURCES ${${PROJECT_NAME}_minizip_SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/iowin32.c )
endif ( )
add_library( minizip STATIC ${${PROJECT_NAME}_minizip_SOURCES} )
if ( STATICLIB_TOOLCHAIN MATCHES "android_[^_]+_[^_]+" )
    target_compile_definitions ( minizip PRIVATE -DUSE_FILE32API )
endif ( )
target_include_directories ( minizip BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
add_dependencies ( minizip ${PROJECT_NAME} )

# pkg-config
if ( NOT DEFINED CMAKE_LIBRARY_OUTPUT_DIRECTORY )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR} )
endif ( )
# zlib
set ( ${PROJECT_NAME}_MODULE zlib )
set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_BINARY_DIR}/zlib" )
set ( ${PROJECT_NAME}_PC_LIBS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -lz" )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/zlib.pc )
# minizip
set ( ${PROJECT_NAME}_MODULE minizip )
set ( ${PROJECT_NAME}_PC_CFLAGS "-I${CMAKE_CURRENT_LIST_DIR}/zlib/contrib" )
set ( ${PROJECT_NAME}_PC_LIBS "-L${CMAKE_LIBRARY_OUTPUT_DIRECTORY} -lminizip" )
set ( ${PROJECT_NAME}_PC_REQUIRES "zlib" )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/resources/pkg-config.in 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/pkgconfig/minizip.pc )
