project(spdlog_test
    VERSION 1.0.0
    LANGUAGES C CXX
)

set(SRC_FILES 
    src/main.cpp
    include/test.hpp
    include/threadpool.hpp
)

add_executable(${PROJECT_NAME} ${SRC_FILES})

target_include_directories(${PROJECT_NAME} 
    PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME} 
    PRIVATE 
    spdlog::spdlog_header_only
)

target_compile_definitions(${PROJECT_NAME}
    PRIVATE
        SPDLOG_ACTIVE_LEVEL=$<IF:$<CONFIG:Debug>,SPDLOG_LEVEL_TRACE,SPDLOG_LEVEL_TRACE>
)
