cmake_minimum_required(VERSION 3.15.0)

project(example
    VERSION 1.0
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE TRUE)

SET(CMAKE_TOOLCHAIN_FILE "F:/workSpace/vcpkg-export-20230826-101656/scripts/buildsystems/vcpkg.cmake")
set(CMAKE_PREFIX_PATH "F:/workSpace/vcpkg-export-20230826-101656/installed/x64-windows-static" ${CMAKE_PREFIX_PATH})

add_subdirectory(spdlog_test)
add_subdirectory(thirdparty)