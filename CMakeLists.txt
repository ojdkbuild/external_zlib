
cmake_minimum_required ( VERSION 2.8.12 )

# toolchain setup
set ( STATICLIB_TOOLCHAIN linux_amd64_clang CACHE STRING "toolchain triplet" )
set ( CMAKE_TOOLCHAIN_FILE ${CMAKE_CURRENT_LIST_DIR}/cmake/toolchains/${STATICLIB_TOOLCHAIN}.cmake CACHE INTERNAL "" )

# project setup
project ( external_zlib CXX C )
# common include
include ( ${CMAKE_CURRENT_LIST_DIR}/cmake/CMakeLists_common.cmake )

# build steps
# workaround for zlib tests
set ( CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/zlib )
# workaround for zlib modifies file in src
if ( NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/zlib_src )
    file ( COPY ${CMAKE_CURRENT_LIST_DIR}/zlib DESTINATION ${CMAKE_CURRENT_BINARY_DIR} )
    file ( RENAME ${CMAKE_CURRENT_BINARY_DIR}/zlib ${CMAKE_CURRENT_BINARY_DIR}/zlib_src )
endif ( )
add_subdirectory ( ${CMAKE_CURRENT_BINARY_DIR}/zlib_src ${CMAKE_CURRENT_BINARY_DIR}/zlib )
# copy back zconf.h in post build step
add_custom_command( OUTPUT ${PROJECT_NAME}_zconf_command
        COMMAND ${CMAKE_COMMAND} -E copy 
        ${CMAKE_CURRENT_BINARY_DIR}/zlib/zconf.h ${CMAKE_CURRENT_BINARY_DIR}/zlib_src/zconf.h )
add_custom_target ( ${PROJECT_NAME}_zconf DEPENDS ${PROJECT_NAME}_zconf_command  )
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
# LOCATION property does not work and TARGET_FILE generator expression is problematic here
if ( STATICLIB_TOOLCHAIN MATCHES "windows_(i386|amd64)_msvc" )
    set ( zlibstatic_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/zlibstaticd${CMAKE_STATIC_LIBRARY_SUFFIX} )
elseif ( STATICLIB_TOOLCHAIN MATCHES "ios_[^_]+_clang" )
    set ( zlibstatic_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libzlibstatic${CMAKE_STATIC_LIBRARY_SUFFIX} )
else ( )
    set ( zlibstatic_LOCATION ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libz${CMAKE_STATIC_LIBRARY_SUFFIX} )
endif ( )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${zlibstatic_LOCATION} )
add_dependencies( ${PROJECT_NAME}_zconf zlibstatic )
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_zconf )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/zlib_src )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
target_include_directories ( example BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
if ( TARGET example64 )
    target_include_directories ( example64 BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
endif ( )
target_include_directories ( minigzip BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
if ( TARGET minigzip64 )
    target_include_directories ( minigzip64 BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
endif ( )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
