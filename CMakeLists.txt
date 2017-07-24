cmake_minimum_required(VERSION 2.8.7)
project(masterlib C CXX)

if (NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message(STATUS "No build type selected, default to Debug")
  set(CMAKE_BUILD_TYPE "Debug")
endif()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

file(GLOB_RECURSE masterlib_HEADER_FILES "src/*.h")
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
#
#file(GLOB_RECURSE masterlib_HEADERS "src/*.h")
#
#
#file(GLOB liberation_SRC
#   "src/*.cc"
#   "src/*.c"
#   "src/*.h"
#)
#
#if (NOT DEFINED IOS_PLATFORM)
#    file(GLOB liberation_arch_specific_SRC
#        "src/x64/*.cc"
#        "src/x64/*.h"
#        "src/x86/*.cc"
#        "src/x86/*.h"
#    )
#else()
#    file(GLOB liberation_arch_specific_SRC
#        "src/aarch64/*.cc"
#        "src/arch64/*.h"
#        "src/armv7/*.cc"
#        "src/armv7/*.h"
#    )
#endif()
#
#file(GLOB liberation_test_SRC
#        "tests/main.cc"
#)
#
#add_subdirectory("${CMAKE_SOURCE_DIR}/third_party/capstone")
#
#add_library(masterlib STATIC ${liberation_SRC} ${liberation_arch_specific_SRC} src/process.h)
#
#add_executable(masterlib_test ${liberation_test_SRC})
#
#target_include_directories(masterlib PUBLIC
#  $<BUILD_INTERFACE:${masterlib_INCLUDE_DIRS}>
#  $<INSTALL_INTERFACE:${masterlib_INCLUDE_DIRS}>)
#
#set(CMAKE_MACOSX_BUNDLE YES)
#
#set(MACOSX_BUNDLE_GUI_IDENTIFIER "org.satorify")
#set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO")
#set(CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY "Don't Code Sign")
#
#target_link_libraries(masterlib capstone-static)
#target_link_libraries(masterlib_test masterlib)
#
## add_custom_target(install
##     COMMAND make
##     COMMAND ldid -S${CMAKE_SOURCE_DIR}/ent.plist ${CMAKE_SOURCE_DIR}/build/masterlib_test
##     COMMAND scp -P $ENV{THEOS_DEVICE_PORT} ${CMAKE_SOURCE_DIR}/build/masterlib_test root@$ENV{THEOS_DEVICE_IP}:/usr/bin/masterlib
## )
