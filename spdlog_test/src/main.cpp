#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <functional>
#include "spdlog/spdlog.h"
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include "spdlog/sinks/stdout_sinks.h"
#include "spdlog/sinks/basic_file_sink.h" 
#include "spdlog/sinks/rotating_file_sink.h"
#include "test.hpp"
#include "threadpool.hpp"

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

        threadpool pool(4);
        
        Test test;
        for(int i = 0; i < 10; i++){
            // pool.addTask(std::bind(test.print, i));
            pool.addTask([i](){
                SPDLOG_INFO("test {}",i);
            });
        }
        pool.start();
    }
    catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    return EXIT_SUCCESS;
}