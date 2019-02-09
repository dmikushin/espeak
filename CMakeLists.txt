cmake_minimum_required(VERSION 2.8)

project(espeak)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

try_compile(HAVE_DIRENT ${CMAKE_BINARY_DIR}/test_dirent ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests/test_dirent.c)
if (${HAVE_DIRENT})
set(HAVE_DIRENT 1)
else ()
set(HAVE_DIRENT 0)
endif ()

try_compile(HAVE_SEMAPHORE ${CMAKE_BINARY_DIR}/test_semaphore ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests/test_semaphore.c)
if (${HAVE_SEMAPHORE})
set(HAVE_SEMAPHORE 1)
else ()
set(HAVE_SEMAPHORE 0)
endif ()

try_compile(HAVE_POLL ${CMAKE_BINARY_DIR}/test_poll ${CMAKE_CURRENT_SOURCE_DIR}/cmake/tests/test_poll.c)
if (${HAVE_POLL})
set(HAVE_POLL 1)
else ()
set(HAVE_POLL 0)
endif ()

find_package (Threads)

# Default to debug build
#set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_BUILD_TYPE Release)

set(CMAKE_C_FLAGS "-mmcu=msp430f5529 --machine=large -fdata-sections -ffunction-sections")
if (CMAKE_BUILD_TYPE STREQUAL "Release")
if ("${CMAKE_C_COMPILER_ID}" STREQUAL "GNU")
set(CMAKE_C_FLAGS_RELEASE "-DNDEBUG -Os -ffast-math")
endif()
else()
set(CMAKE_C_FLAGS_DEBUG "-g -O0")
endif()

set(DATADIR "${CMAKE_CURRENT_SOURCE_DIR}/espeak-data")

file(GLOB_RECURSE lib${PROJECT_NAME}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/libespeak/*.c")

add_library(lib${PROJECT_NAME} ${lib${PROJECT_NAME}_SRC})
set_target_properties(lib${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(lib${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_include_directories
#target_compile_definitions(lib${PROJECT_NAME} PRIVATE HAVE_DIRENT=${HAVE_DIRENT} HAVE_SEMAPHORE=${HAVE_SEMAPHORE} HAVE_POLL=${HAVE_POLL})
target_compile_definitions(lib${PROJECT_NAME} PRIVATE PLATFORM_MSP430 bool=int true=1 false=0)
target_compile_definitions(lib${PROJECT_NAME} PRIVATE LIBRARY PATH_ESPEAK_DATA=\"${DATADIR}\")

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/espeak.c")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
#target_compile_definitions(${PROJECT_NAME} PRIVATE HAVE_DIRENT=${HAVE_DIRENT} HAVE_SEMAPHORE=${HAVE_SEMAPHORE} HAVE_POLL=${HAVE_POLL})
target_compile_definitions(${PROJECT_NAME} PRIVATE PLATFORM_MSP430 bool=int true=1 false=0)
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME} m)

add_executable(speak "${CMAKE_CURRENT_SOURCE_DIR}/src/speak.c")
target_include_directories(speak PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
#target_compile_definitions(speak PRIVATE HAVE_DIRENT=${HAVE_DIRENT} HAVE_SEMAPHORE=${HAVE_SEMAPHORE} HAVE_POLL=${HAVE_POLL})
target_compile_definitions(speak PRIVATE PLATFORM_MSP430 bool=int true=1 false=0)
target_link_libraries(speak lib${PROJECT_NAME} m)

