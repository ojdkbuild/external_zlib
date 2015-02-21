
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
get_property ( zlib_location TARGET zlibstatic PROPERTY LOCATION )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${zlib_location} )
add_dependencies( ${PROJECT_NAME}_zconf zlibstatic )
add_dependencies( ${PROJECT_NAME} ${PROJECT_NAME}_zconf )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/zlib_src )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
target_include_directories ( example BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
target_include_directories ( example64 BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
target_include_directories ( minigzip BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
target_include_directories ( minigzip64 BEFORE PRIVATE ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
