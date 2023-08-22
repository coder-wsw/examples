#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "spdlog/sinks/rotating_file_sink.h"
#include "test.hpp"

int main(int argc, char *argv[])
{
    try 
    {
        // Create basic file logger (not rotated)
        auto my_logger = spdlog::basic_logger_mt("basic_logger", "logs/basic.txt");
        
        // create a file rotating logger with 5mb size max and 3 rotated files
        std::vector<spdlog::sink_ptr> sinks;
        sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        sinks.push_back(std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
            "logs/rotate", 1024 * 5, 10, true));

        // 调整各个输出端的日志等级
        sinks[0]->set_level(spdlog::level::trace);
        sinks[1]->set_level(spdlog::level::trace);

        auto logger = std::make_shared<spdlog::logger>("", std::begin(sinks),
                                                   std::end(sinks));
        
        spdlog::set_default_logger(logger);
        spdlog::set_level(spdlog::level::trace);

        Test test;
        for(int i = 0; i < 1; i++){
            SPDLOG_INFO("Welcome to spdlog!");
            SPDLOG_DEBUG("Some debug message with param {}", 42);
            SPDLOG_WARN("Easy padding in numbers like {:08d}", 12);
            SPDLOG_ERROR("Support for int: {0:d};  hex: {0:x};  oct: {0:o}; bin: {0:b}", 42);
            SPDLOG_CRITICAL("Support for floats {:03.2f}", 1.23456);
            SPDLOG_TRACE("Positional args are {1} {0}..", "too", "supported");
            test.print(i);
        }
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}