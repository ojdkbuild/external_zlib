
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
include_directories ( BEFORE ${CMAKE_CURRENT_LIST_DIR}/zlib )
add_subdirectory ( zlib )
add_library( ${PROJECT_NAME} STATIC IMPORTED GLOBAL )
get_property ( zlib_location TARGET zlib PROPERTY LOCATION )
set_target_properties( ${PROJECT_NAME} PROPERTIES IMPORTED_LOCATION ${zlib_location} )
add_dependencies( ${PROJECT_NAME} zlib )
set ( ${PROJECT_NAME}_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/zlib 
        ${CMAKE_CURRENT_BINARY_DIR}/zlib )

# test
set ( ${PROJECT_NAME}_TEST_INCLUDES ${${PROJECT_NAME}_INCLUDES} )
set ( ${PROJECT_NAME}_TEST_LIBS ${PROJECT_NAME} )
staticlib_enable_testing ( ${PROJECT_NAME}_TEST_INCLUDES ${PROJECT_NAME}_TEST_LIBS )

# export headers
set ( ${PROJECT_NAME}_INCLUDES ${${PROJECT_NAME}_INCLUDES} CACHE INTERNAL "" )
