cmake_minimum_required(VERSION 2.8.7)
project(masterlib C CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -stdlib=libc++")

file(GLOB_RECURSE masterlib_HEADER_FILES  "src/*.h")
file(GLOB_RECURSE masterlib_SRC_FILES "src/*.cc")

set (masterlib_INCLUDE_DIRS "")
foreach (_headerFile ${masterlib_HEADER_FILES})
  get_filename_component(_dir ${_headerFile} PATH)
  list (APPEND masterlib_INCLUDE_DIRS ${_dir})
endforeach()
list(REMOVE_DUPLICATES masterlib_INCLUDE_DIRS)

add_library(masterlib_STATIC STATIC ${masterlib_HEADER_FILES} ${masterlib_SRC_FILES})

target_include_directories(masterlib_STATIC PUBLIC
        $<BUILD_INTERFACE:${masterlib_INCLUDE_DIRS}>
        $<INSTALL_INTERFACE:${masterlib_INCLUDE_DIRS}>)


add_executable(masterlib_TESTS tests/main.cc)
target_link_libraries(masterlib_TESTS masterlib_STATIC)
