cmake_minimum_required(VERSION 3.15.0)

project(example 
    VERSION 1.0 
    LANGUAGES C CXX
)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_VERBOSE_MAKEFILE  TRUE)

add_subdirectory(spdlog_test)
add_subdirectory(thirdparty)