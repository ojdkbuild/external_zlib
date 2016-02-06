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

# based on https://github.com/madler/zlib/blob/50893291621658f355bc5b4d450a8d06a563053d/CMakeLists.txt
cmake_minimum_required ( VERSION 2.8.12 )

project ( z C )

set ( VERSION "1.2.8" )

set ( ${PROJECT_NAME}_SRCDIR ${CMAKE_CURRENT_LIST_DIR}/../../zlib )

if ( NOT ${PROJECT_NAME}_SRCDIR STREQUAL CMAKE_CURRENT_BINARY_DIR )
    # If we're doing an out of source build and the user has a zconf.h
    # in their source tree...
    if ( EXISTS ${${PROJECT_NAME}_SRCDIR}/zconf.h )
        message ( STATUS "Renaming" )
        message ( STATUS "    ${${PROJECT_NAME}_SRCDIR}/zconf.h" )
        message ( STATUS "to 'zconf.h.included' because this file is included with zlib" )
        message ( STATUS "but CMake generates it automatically in the build directory." )
        file ( RENAME ${${PROJECT_NAME}_SRCDIR}/zconf.h ${${PROJECT_NAME}_SRCDIR}/zconf.h.included )
  endif ( )
endif ( )

configure_file(	${${PROJECT_NAME}_SRCDIR}/zconf.h.cmakein
		${CMAKE_CURRENT_BINARY_DIR}/zconf.h @ONLY )

# parse the full version number from zlib.h and include in ZLIB_FULL_VERSION
file(READ ${${PROJECT_NAME}_SRCDIR}/zlib.h _zlib_h_contents)
string(REGEX REPLACE ".*#define[ \t]+ZLIB_VERSION[ \t]+\"([-0-9A-Za-z.]+)\".*"
    "\\1" ZLIB_FULL_VERSION ${_zlib_h_contents})

#============================================================================
# zlib
#============================================================================

set(ZLIB_PUBLIC_HDRS
        ${CMAKE_CURRENT_BINARY_DIR}/zconf.h
        ${${PROJECT_NAME}_SRCDIR}/zlib.h
)
set ( ZLIB_PRIVATE_HDRS
        ${${PROJECT_NAME}_SRCDIR}/crc32.h
        ${${PROJECT_NAME}_SRCDIR}/deflate.h
        ${${PROJECT_NAME}_SRCDIR}/gzguts.h
        ${${PROJECT_NAME}_SRCDIR}/inffast.h
        ${${PROJECT_NAME}_SRCDIR}/inffixed.h
        ${${PROJECT_NAME}_SRCDIR}/inflate.h
        ${${PROJECT_NAME}_SRCDIR}/inftrees.h
        ${${PROJECT_NAME}_SRCDIR}/trees.h
        ${${PROJECT_NAME}_SRCDIR}/zutil.h
)
set ( ZLIB_SRCS
        ${${PROJECT_NAME}_SRCDIR}/adler32.c
        ${${PROJECT_NAME}_SRCDIR}/compress.c
        ${${PROJECT_NAME}_SRCDIR}/crc32.c
        ${${PROJECT_NAME}_SRCDIR}/deflate.c
        ${${PROJECT_NAME}_SRCDIR}/gzclose.c
        ${${PROJECT_NAME}_SRCDIR}/gzlib.c
        ${${PROJECT_NAME}_SRCDIR}/gzread.c
        ${${PROJECT_NAME}_SRCDIR}/gzwrite.c
        ${${PROJECT_NAME}_SRCDIR}/inflate.c
        ${${PROJECT_NAME}_SRCDIR}/infback.c
        ${${PROJECT_NAME}_SRCDIR}/inftrees.c
        ${${PROJECT_NAME}_SRCDIR}/inffast.c
        ${${PROJECT_NAME}_SRCDIR}/trees.c
        ${${PROJECT_NAME}_SRCDIR}/uncompr.c
        ${${PROJECT_NAME}_SRCDIR}/zutil.c
)

FILE ( COPY ${ZLIB_PUBLIC_HDRS} DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/../zlib_install/include )

# standalone build
if ( DEFINED ZLIB_LIBRARY_OUTPUT_DIRECTORY )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
    set ( CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
    set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
    set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
    set ( CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${ZLIB_LIBRARY_OUTPUT_DIRECTORY} )
endif ( )

# check deplibs cache
if ( STATICLIB_USE_DEPLIBS_CACHE )
    set ( ${PROJECT_NAME}_CACHED_LIB_PATH ${STATICLIB_DEPLIBS_CACHE_DIR}/${CMAKE_STATIC_LIBRARY_PREFIX}${PROJECT_NAME}${CMAKE_STATIC_LIBRARY_SUFFIX} )
    if ( EXISTS ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
        set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${${PROJECT_NAME}_CACHED_LIB_PATH} )
        message ( STATUS "Using cached library: [${${PROJECT_NAME}_CACHED_LIB_PATH}]" )
        return ( )
    endif ( )
endif ( )

add_library ( ${PROJECT_NAME} STATIC ${ZLIB_SRCS} ${ZLIB_PUBLIC_HDRS} ${ZLIB_PRIVATE_HDRS})
target_include_directories ( ${PROJECT_NAME} BEFORE PRIVATE ${CMAKE_CURRENT_BINARY_DIR} ${${PROJECT_NAME}_SRCDIR})
target_compile_definitions ( ${PROJECT_NAME} PRIVATE -DZLIB_CONST)

if ( ${CMAKE_C_COMPILER_ID}x MATCHES "MSVCx" )
    target_compile_definitions ( ${PROJECT_NAME} PRIVATE
            -D_CRT_SECURE_NO_DEPRECATE  
            -D_CRT_NONSTDC_NO_DEPRECATE )
endif ( ) 
