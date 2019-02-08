cmake_minimum_required(VERSION 2.8)

project(espeak)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

try_compile(HAVE_DIRENT ${CMAKE_BINARY_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests/test_dirent.c)
if (${HAVE_DIRENT})
set(HAVE_DIRENT 1)
else ()
set(HAVE_DIRENT 0)
endif ()
message("HAVE_DIRENT = ${HAVE_DIRENT}")

find_package (Threads)

# Default to debug build
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

if (CMAKE_BUILD_TYPE_UPPER STREQUAL "RELEASE")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -ffast-math -funroll-all-loops -march=native")
endif()
else()
set(CMAKE_C_FLAGS "${CMAKE_CXX_FLAGS} -g -O0")
endif()

set(DATADIR "${CMAKE_CURRENT_SOURCE_DIR}/espeak-data")

file(GLOB_RECURSE lib${PROJECT_NAME}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/libespeak/*.c")

add_library(lib${PROJECT_NAME} SHARED ${lib${PROJECT_NAME}_SRC})
set_target_properties(lib${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(lib${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(lib${PROJECT_NAME} PRIVATE HAVE_DIRENT=${HAVE_DIRENT} bool=int true=1 false=0 LIBRARY PATH_ESPEAK_DATA=\"${DATADIR}\")
target_link_libraries(lib${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT} m)

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/espeak.c")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_DIRENT=${HAVE_DIRENT} bool=int true=1 false=0)
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

add_executable(speak "${CMAKE_CURRENT_SOURCE_DIR}/src/speak.c")
target_include_directories(speak PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(speak PRIVATE HAVE_DIRENT=${HAVE_DIRENT} bool=int true=1 false=0)
target_link_libraries(speak lib${PROJECT_NAME})

