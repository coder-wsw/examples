project(spdlog_test
    VERSION 1.0.0
    LANGUAGES C CXX
)

# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")
set(SRC_FILES
    src/main.cpp
    include/test.hpp
    include/threadpool.hpp
)

find_package(Boost 1.82.0
    REQUIRED
    COMPONENTS
    system
)

if(Boost_FOUND)
    message("boost found")
    message("boost include dir: ${Boost_INCLUDE_DIRS}")
    message("boost lib dir: ${Boost_LIBRARY_DIRS}")
    message("boost libs: ${Boost_LIBRARIES}")
endif()

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIRS}
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
    spdlog::spdlog_header_only
    ${Boost_LIBRARIES}
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
    SPDLOG_ACTIVE_LEVEL=$<IF:$<CONFIG:Debug>,SPDLOG_LEVEL_TRACE,SPDLOG_LEVEL_TRACE>
)
