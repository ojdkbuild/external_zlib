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

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_gcc CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_zlib CXX C )
set ( ${PROJECT_NAME}_STATICLIB_VERSION 1.2.8.2 )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/staticlibs_common.cmake )

# build steps
if ( CMAKE_BUILD_TYPE MATCHES "Debug" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_DEBUG}" )
endif ( )
if ( CMAKE_BUILD_TYPE MATCHES "Release" )
    set ( ${PROJECT_NAME}_CFLAGS "${CMAKE_C_FLAGS} ${CMAKE_C_FLAGS_RELEASE}" )
endif ( )
configure_file ( ${CMAKE_CURRENT_LIST_DIR}/Makefile-${STATICLIB_TOOLCHAIN}
        ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_custom_target( ${PROJECT_NAME}_build
        COMMAND ${CMAKE_MAKE_PROGRAM} -f ${CMAKE_CURRENT_BINARY_DIR}/Makefile-${STATICLIB_TOOLCHAIN})
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_build )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION 
        ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libz${CMAKE_STATIC_LIBRARY_SUFFIX} )

set ( ${PROJECT_NAME}_INCLUDES 
        ${CMAKE_CURRENT_BINARY_DIR}/zlib
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip )

# build minizip
set ( ${PROJECT_NAME}_minizip_SOURCES
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/ioapi.c 
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/unzip.c
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/zip.c )
if ( STATICLIB_TOOLCHAIN MATCHES "windows_[^_]+_[^_]+" )
    set ( ${PROJECT_NAME}_minizip_SOURCES ${${PROJECT_NAME}_minizip_SOURCES}
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/iowin32.c )
endif ( )
add_library( ${PROJECT_NAME}_minizip STATIC ${${PROJECT_NAME}_minizip_SOURCES} )
target_link_libraries ( ${PROJECT_NAME}_minizip ${PROJECT_NAME} )
if ( STATICLIB_TOOLCHAIN MATCHES "android_[^_]+_[^_]+" )
    target_compile_definitions ( ${PROJECT_NAME}_minizip PRIVATE -DUSE_FILE32API )
endif ( )
target_include_directories ( ${PROJECT_NAME}_minizip BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} ${PROJECT_NAME}_minizip )
set ( ${PROJECT_NAME}_TEST_OPTS "" )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME}_TEST_OPTS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
