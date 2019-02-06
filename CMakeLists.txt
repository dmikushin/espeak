cmake_minimum_required(VERSION 2.8)

project(espeak)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

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

file(GLOB_RECURSE lib${PROJECT_NAME}_SRC "${CMAKE_CURRENT_SOURCE_DIR}/src/libespeak/*.cpp")

add_library(lib${PROJECT_NAME} SHARED ${lib${PROJECT_NAME}_SRC})
set_target_properties(lib${PROJECT_NAME} PROPERTIES PREFIX "")
target_include_directories(lib${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_compile_definitions(lib${PROJECT_NAME} PRIVATE LIBRARY PATH_ESPEAK_DATA=\"${DATADIR}\")
target_link_libraries(lib${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})

add_executable(${PROJECT_NAME} "${CMAKE_CURRENT_SOURCE_DIR}/src/espeak.cpp")
target_include_directories(${PROJECT_NAME} PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(${PROJECT_NAME} lib${PROJECT_NAME})

add_executable(speak "${CMAKE_CURRENT_SOURCE_DIR}/src/speak.cpp")
target_include_directories(speak PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/include")
target_link_libraries(speak lib${PROJECT_NAME})

