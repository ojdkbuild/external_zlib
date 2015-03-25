
cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_clang CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_zlib CXX C )
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
add_library( ${PROJECT_NAME}_minizip STATIC         
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/ioapi.c 
#        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/iowin32.c 
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/unzip.c
        ${CMAKE_CURRENT_LIST_DIR}/zlib/contrib/minizip/zip.c )
target_link_libraries ( ${PROJECT_NAME}_minizip ${PROJECT_NAME} )
target_include_directories ( ${PROJECT_NAME}_minizip BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} ${PROJECT_NAME}_minizip )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
