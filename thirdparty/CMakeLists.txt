
find_package(Threads REQUIRED)

add_library(spdlog_header_only INTERFACE)
add_library(spdlog::spdlog_header_only ALIAS spdlog_header_only)

target_include_directories(spdlog_header_only INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/spdlog>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(spdlog_header_only INTERFACE Threads::Threads)

install(TARGETS spdlog_header_only EXPORT spdlogTargets
    INCLUDES DESTINATION include
)

install(DIRECTORY include/spdlog DESTINATION include)
